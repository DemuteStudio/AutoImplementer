// Copyright Epic Games, Inc. All Rights Reserved.

#include "MetasoundAttenuationNode.h" 

#include "MetasoundAudioBuffer.h"
#include "Internationalization/Text.h"
#include "Math/UnrealMathUtility.h"

#include "MetasoundDataFactory.h"
#include "MetasoundFacade.h"
#include "MetasoundOperatorData.h"
#include "MetasoundNodeRegistrationMacro.h"
#include "MetasoundExecutableOperator.h"
#include "MetasoundParamHelper.h"
#include "MetasoundPrimitives.h"
#include "MetasoundTrigger.h"
#include "DSP/FloatArrayMath.h"

#define LOCTEXT_NAMESPACE "MetasoundAttenuationNode"

namespace Metasound::Experimental
{
	namespace MetasoundAttenuationNodePrivate
	{
		const FLazyName InputBaseName{"In"};
		const FLazyName DistanceName{"Distance"};
		const FText InputTooltip = METASOUND_LOCTEXT("In_ToolTip", "A trigger");

		FName MakeInputVertexName(int32 InIndex)
		{
			FName Name = InputBaseName;
			Name.SetNumber(InIndex);
			return Name;
		}

		FInputDataVertex MakeInputDataVertex(int32 InIndex)
		{
			FName InputName = MakeInputVertexName(InIndex);
			const FText InputDisplayName = METASOUND_LOCTEXT_FORMAT("In_DisplayName", "In {0}", InIndex);
			return TInputDataVertex<FAudioBuffer>{InputName, FDataVertexMetadata{InputTooltip, InputDisplayName}};
		}

		static FInputDataVertex MakeInputDistanceVertex(const FName InName, const FString& InFriendlyName)
		{
			const FText InputDisplayName = METASOUND_LOCTEXT_FORMAT("In_DisplayName", "{0}", FText::FromString(FString::Printf(TEXT("%s"), *InName.ToString())));
			return TInputDataVertex<float>{InName, FDataVertexMetadata{ FText::FromString(*InFriendlyName), InputDisplayName }, 1.0f};
		}

		FLazyName OutputBaseName{"Out"};
		const FText OutputTooltip = METASOUND_LOCTEXT("Out_ToolTip", "A trigger");

		FName MakeOutputVertexName(int32 OutOutdex)
		{
			FName Name = OutputBaseName;
			Name.SetNumber(OutOutdex);
			return Name;
		}

		FOutputDataVertex MakeOutputDataVertex(int32 OutOutdex)
		{
			FName OutputName = MakeOutputVertexName(OutOutdex);
			const FText OutputDisplayName = METASOUND_LOCTEXT_FORMAT("Out_DisplayName", "Out {0}", OutOutdex);
			return TOutputDataVertex<FAudioBuffer>{OutputName, FDataVertexMetadata{OutputTooltip, OutputDisplayName}};
		}


		// Create the node's vertex interface based upon the number of inputs and outputs
		// desired. 
		FVertexInterface GetVertexInterface(int32 InNumInputs)
		{
			FInputVertexInterface InputInterface;
			for (int32 i = 0; i < InNumInputs; i++)
			{
				InputInterface.Add(MakeInputDataVertex(i));
			}
			InputInterface.Add(MakeInputDistanceVertex(DistanceName, TEXT("Distance Input")));

			FOutputVertexInterface OutputInterface;
			for (int32 i = 0; i < InNumInputs; i++)
			{
				OutputInterface.Add(MakeOutputDataVertex(i));
			}

			return FVertexInterface
				{
					MoveTemp(InputInterface),
					MoveTemp(OutputInterface)
				};
		}

		METASOUND_PARAM(OutFloat, "Out", "Float output");
	}

	/** To send data from a FMetaSoundFrontendNodeConfiguration to an IOperator, it should
	 * be encapsulated in the form of a IOperatorData.  
	 *
	 * The use of the TOperatorData provides some safety mechanisms for downcasting node configurations.
	 */
	class FAttenuationOperatorData : public TOperatorData<FAttenuationOperatorData>
	{
	public:
		// The OperatorDataTypeName is used when downcasting an IOperatorData to ensure
		// that the downcast is valid.
		static const FLazyName OperatorDataTypeName;

		FAttenuationOperatorData(const TObjectPtr<USoundAttenuation>& InSoundAttenuation)
		: SoundAttenuation(InSoundAttenuation)
		{
		}

		const TObjectPtr<USoundAttenuation>& GetSoundAttenuation() const
		{
			return SoundAttenuation;
		}

	private:

		TObjectPtr<USoundAttenuation> SoundAttenuation;
	};

	const FLazyName FAttenuationOperatorData::OperatorDataTypeName = "AttenuationOperatorData";

	class FAttenuationOperator : public TExecutableOperator<FAttenuationOperator>
	{
	public:

		FAttenuationOperator(const TObjectPtr<USoundAttenuation>& InSoundAttenuation, FFloatReadRef&& InDistance, TArray<FAudioBufferReadRef> InInputTriggers, TArray<FAudioBufferWriteRef> InOutputTriggers)
		: Inputs(MoveTemp(InInputTriggers))
		, Outputs(MoveTemp(InOutputTriggers))
		, SoundAttenuation(InSoundAttenuation)
		, Distance(InDistance)
		{
			PrevGain = 0.f;
		}

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override
		{
			using namespace MetasoundAttenuationNodePrivate;

			for (int32 i = 0; i < Inputs.Num(); i++)
			{
				InOutVertexData.BindReadVertex(MakeInputVertexName(i), Inputs[i]);
			}
			InOutVertexData.BindReadVertex(DistanceName, Distance);
		}

		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override
		{
			using namespace MetasoundAttenuationNodePrivate;

			for (int32 i = 0; i < Outputs.Num(); i++)
			{
				InOutVertexData.BindReadVertex(MakeOutputVertexName(i), Outputs[i]);
			}
		}

		void Execute()
		{
			// initialize the output using the first set of input channels
			float NextGain = 1.f;
			if (SoundAttenuation)
			{
				NextGain = SoundAttenuation->Attenuation.AttenuationEval(*Distance, SoundAttenuation->Attenuation.FalloffDistance);
			}


			// initialize the output buffers w/ the first set of input buffers 
			for (int32 i = 0; i < Inputs.Num(); ++i)
			{
				TArrayView<const float> InputView = *Inputs[i];
				TArrayView<float> OutputView = *Outputs[i];
	
				check(InputView.Num() == OutputView.Num());
				Audio::ArrayFade(InputView, PrevGain, NextGain, OutputView);
			}

			PrevGain = NextGain;
		}

		void Reset(const IOperator::FResetParams& InParams)
		{
			for (const FAudioBufferWriteRef& OutputTrigger : Outputs)
			{
				OutputTrigger->Zero();
			}
			PrevGain = 0.f;
		}

		static FNodeClassMetadata GetNodeInfo()
		{
			using namespace MetasoundAttenuationNodePrivate;
			return FNodeClassMetadata
			{
				FNodeClassName{ "Experimental", "AttenuationOperator", "" },
				1, // Major version
				0, // Minor version
				METASOUND_LOCTEXT("AttenuationNodeName", "Attenuation Node"),	
				METASOUND_LOCTEXT("AttenuationNodeDescription", "A node which applies an attenuation asset to the signal."),	
				TEXT("Demute"), // Author
				METASOUND_LOCTEXT("AttenuationPromptIfMissing", "Enable the MetaSoundExperimental Plugin"), // Prompt if missing
				MetasoundAttenuationNodePrivate::GetVertexInterface(1 /* default num inputs */),
				{}
			};
		}

		static TUniquePtr<IOperator> CreateOperator(const FBuildOperatorParams& InParams, FBuildResults& OutResults) 
		{
			using namespace MetasoundAttenuationNodePrivate;

			// If your node configuration contains data that needs to be accessed by
			// an operator, it can be accessed in the CreateOperator call. The 
			// TSharedPtr<const IOperatorData> could even be stored on the IOperator. 
			//
			// In this example, the function `GetOperatorDataAs<>` safely downcasts
			// the TSharedPtr<const IOperatorData> retrieved from the node. 
			TObjectPtr<USoundAttenuation> SoundAttenuation = nullptr;
			if (const FAttenuationOperatorData* ExampleConfig = CastOperatorData<const FAttenuationOperatorData>(InParams.Node.GetOperatorData().Get()))
			{
				SoundAttenuation = ExampleConfig->GetSoundAttenuation();
			}

			// If the node configuration overrides the default class interface, 
			// the node's vertex interface will reflect the override. The vertex
			// interface can be queried to see which inputs and outputs exist.
			//
			// For more complex scenarios, developers may want to pass other data
			// through the IOperatorData. Alternatively, this node could have put
			// the number of inputs and outputs into the IOperator data. 
			const FVertexInterface& NodeInterface = InParams.Node.GetVertexInterface();

			// Build the correct data references based upon what vertices exist
			// on the NodeInterface
			TArray<FAudioBufferReadRef> InputTriggers;
			int32 InputIndex = 0;
			while (true)
			{
				FVertexName VertexName = MakeInputVertexName(InputIndex);
				if (NodeInterface.ContainsInputVertex(VertexName))
				{
					InputTriggers.Add(InParams.InputData.GetOrCreateDefaultDataReadReference<FAudioBuffer>(VertexName, InParams.OperatorSettings));
				}
				else
				{
					break;
				}
				InputIndex++;
			}

			TArray<FAudioBufferWriteRef> OutputTriggers;
			int32 OutputIndex = 0;
			while (true)
			{
				FVertexName VertexName = MakeOutputVertexName(OutputIndex);
				if (NodeInterface.ContainsOutputVertex(VertexName))
				{
					OutputTriggers.Add(TDataWriteReferenceFactory<FAudioBuffer>::CreateExplicitArgs(InParams.OperatorSettings));
				}
				else
				{
					break;
				}
				OutputIndex++;
			}
			FFloatReadRef Distance = InParams.InputData.GetOrCreateDefaultDataReadReference<float>(DistanceName, InParams.OperatorSettings);


			return MakeUnique<FAttenuationOperator>(
					SoundAttenuation,
					MoveTemp(Distance),
					MoveTemp(InputTriggers),
					MoveTemp(OutputTriggers)
					);
		}

	private:
		TArray<FAudioBufferReadRef> Inputs;
		TArray<FAudioBufferWriteRef> Outputs;
		TObjectPtr<USoundAttenuation> SoundAttenuation;
		FFloatReadRef Distance;
		float PrevGain;
	};

	using FAttenuationNode = TNodeFacade<FAttenuationOperator>;

	// The node extension must be registered along with the node. 
	METASOUND_REGISTER_NODE_AND_CONFIGURATION(FAttenuationNode, FMetaSoundAttenuationNodeConfiguration);
}

FMetaSoundAttenuationNodeConfiguration::FMetaSoundAttenuationNodeConfiguration()
:NumInputs(1)
{
}

TInstancedStruct<FMetasoundFrontendClassInterface> FMetaSoundAttenuationNodeConfiguration::OverrideDefaultInterface(const FMetasoundFrontendClass& InNodeClass) const
{
	using namespace Metasound::Experimental::MetasoundAttenuationNodePrivate;
	// Override the interface based upon the number of inputs and outputs desired. 
	return TInstancedStruct<FMetasoundFrontendClassInterface>::Make(FMetasoundFrontendClassInterface::GenerateClassInterface(GetVertexInterface(NumInputs)));
}

TSharedPtr<const Metasound::IOperatorData> FMetaSoundAttenuationNodeConfiguration::GetOperatorData() const 
{
	// Any data the node configuration wishes to share with the operators can be produced here. 
	return MakeShared<Metasound::Experimental::FAttenuationOperatorData>(SoundAttenuation);
}

#undef LOCTEXT_NAMESPACE // MetasoundExperimentalRuntime
