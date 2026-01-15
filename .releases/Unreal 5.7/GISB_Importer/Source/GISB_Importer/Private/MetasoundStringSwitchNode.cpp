// Copyright Epic Games, Inc. All Rights Reserved.

#include "MetasoundStringSwitchNode.h" 

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

#define LOCTEXT_NAMESPACE "MetasoundStringSwitchNode"

namespace Metasound::Experimental
{
	namespace MetasoundStringSwitchNodePrivate
	{
		const FLazyName InputBaseName{"In"};
		const FLazyName StringSwitchName{"String Switch"};
		const FText InputTooltip = METASOUND_LOCTEXT("In_ToolTip", "A trigger");
		
		FInputDataVertex MakeInputDataVertex()
		{
			const FText InputDisplayName = METASOUND_LOCTEXT_FORMAT("In_DisplayName", "{0}", FText::FromString(FString::Printf(TEXT("%s"), *InputBaseName.ToString())));
			return TInputDataVertex<FTrigger>{InputBaseName, FDataVertexMetadata{InputTooltip, InputDisplayName}};
		}

		static FInputDataVertex MakeInputStringSwitchVertex(const FName InName, const FString& InFriendlyName)
		{
			const FText InputDisplayName = METASOUND_LOCTEXT_FORMAT("In_DisplayName", "{0}", FText::FromString(FString::Printf(TEXT("%s"), *InName.ToString())));
			return TInputDataVertex<FString>{InName, FDataVertexMetadata{ FText::FromString(*InFriendlyName), InputDisplayName }};
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
			return TOutputDataVertex<FTrigger>{OutputName, FDataVertexMetadata{OutputTooltip, OutputDisplayName}};
		}


		// Create the node's vertex interface based upon the number of inputs and outputs
		// desired. 
		FVertexInterface GetVertexInterface(int32 InNumOutputs)
		{
			FInputVertexInterface InputInterface;
			InputInterface.Add(MakeInputDataVertex());
			InputInterface.Add(MakeInputStringSwitchVertex(StringSwitchName, TEXT("String Input")));

			FOutputVertexInterface OutputInterface;
			for (int32 i = 0; i < InNumOutputs; i++)
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
	class FStringSwitchOperatorData : public TOperatorData<FStringSwitchOperatorData>
	{
	public:
		// The OperatorDataTypeName is used when downcasting an IOperatorData to ensure
		// that the downcast is valid.
		static const FLazyName OperatorDataTypeName;

		FStringSwitchOperatorData(const TArray<FString>& InSwitchValues, const uint32 InDefaultOut)
		: SwitchValues(InSwitchValues), DefaultOut(InDefaultOut)
		{
		}

		const TArray<FString>& GetSwitchValues() const
		{
			return SwitchValues;
		}

		uint32 GetDefaultOut() const
		{
			return DefaultOut;
		}

	private:

		TArray<FString> SwitchValues;
		uint32 DefaultOut;
	};

	const FLazyName FStringSwitchOperatorData::OperatorDataTypeName = "StringSwitchOperatorData";

	class FStringSwitchOperator : public TExecutableOperator<FStringSwitchOperator>
	{
	public:

		FStringSwitchOperator(
			TDataReadReference<FTrigger>&& InInput,
			TArray<TDataWriteReference<FTrigger>> InOutputs,
			const TArray<FString>& InSwitchValues,
			uint32 InDefaultOut,
			FStringReadRef&& InStringSwitch)
		: Input(MoveTemp(InInput))
		, Outputs(MoveTemp(InOutputs))
		, SwitchValues(InSwitchValues)
		, DefaultOut(InDefaultOut)
		, StringSwitch(MoveTemp(InStringSwitch))
		{
		}

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override
		{
			using namespace MetasoundStringSwitchNodePrivate;

			InOutVertexData.BindReadVertex(InputBaseName, Input);
			InOutVertexData.BindReadVertex(StringSwitchName, StringSwitch);
		}

		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override
		{
			using namespace MetasoundStringSwitchNodePrivate;

			for (int32 i = 0; i < Outputs.Num(); i++)
			{
				InOutVertexData.BindReadVertex(MakeOutputVertexName(i), Outputs[i]);
			}
		}

		void Execute()
		{
			// This node randomly rearranges all input triggers across all the 
			// output triggers.
			if (Outputs.Num() > 0)
			{
				for (const TDataWriteReference<FTrigger>& OutputTrigger : Outputs)
				{
					OutputTrigger->AdvanceBlock();
				}
				Input->ExecuteBlock(
						[](int32, int32) {},
						[this](int32 InStartFrame, int32 InEndFrame)
						{
							int32 OutputIndex = DefaultOut;
							int32 switchIndex = SwitchValues.Find(*StringSwitch);
							if (switchIndex != INDEX_NONE)
							{
								OutputIndex = switchIndex;
							}
							Outputs[OutputIndex]->TriggerFrame(InStartFrame);
						}
				);
			}
		}

		void Reset(const IOperator::FResetParams& InParams)
		{
			for (const TDataWriteReference<FTrigger>& OutputTrigger : Outputs)
			{
				OutputTrigger->Reset();
			}
		}

		static FNodeClassMetadata GetNodeInfo()
		{
			using namespace MetasoundStringSwitchNodePrivate;
			return FNodeClassMetadata
			{
				FNodeClassName{ "Experimental", "StringSwitchOperator", "" },
				1, // Major version
				0, // Minor version
				METASOUND_LOCTEXT("StringSwitchNodeName", "String Switch Node"),	
				METASOUND_LOCTEXT("StringSwitchNodeDescription", "A node which routes the execution based on a string value."),	
				TEXT("Demute"), // Author
				METASOUND_LOCTEXT("StringSwitchPromptIfMissing", "Enable the MetaSoundExperimental Plugin"), // Prompt if missing
				MetasoundStringSwitchNodePrivate::GetVertexInterface(0 /* default num outputs */),
				{}
			};
		}

		static TUniquePtr<IOperator> CreateOperator(const FBuildOperatorParams& InParams, FBuildResults& OutResults) 
		{
			using namespace MetasoundStringSwitchNodePrivate;

			// If your node configuration contains data that needs to be accessed by
			// an operator, it can be accessed in the CreateOperator call. The 
			// TSharedPtr<const IOperatorData> could even be stored on the IOperator. 
			//
			// In this example, the function `GetOperatorDataAs<>` safely downcasts
			// the TSharedPtr<const IOperatorData> retrieved from the node. 
			TArray<FString> SwitchValues;
			uint32 DefaultOut = 0;
			if (const FStringSwitchOperatorData* ExampleConfig = CastOperatorData<const FStringSwitchOperatorData>(InParams.Node.GetOperatorData().Get()))
			{
				SwitchValues = ExampleConfig->GetSwitchValues();
				DefaultOut = ExampleConfig->GetDefaultOut();
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
			TDataReadReference<FTrigger> InputTrigger = InParams.InputData.GetOrCreateDefaultDataReadReference<FTrigger>(InputBaseName, InParams.OperatorSettings);


			TArray<TDataWriteReference<FTrigger>> OutputTriggers;
			int32 OutputIndex = 0;
			while (true)
			{
				FVertexName VertexName = MakeOutputVertexName(OutputIndex);
				if (NodeInterface.ContainsOutputVertex(VertexName))
				{
					OutputTriggers.Add(TDataWriteReferenceFactory<FTrigger>::CreateExplicitArgs(InParams.OperatorSettings));
				}
				else
				{
					break;
				}
				OutputIndex++;
			}
			FStringReadRef StringSwitch = InParams.InputData.GetOrCreateDefaultDataReadReference<FString>(StringSwitchName, InParams.OperatorSettings);


			return MakeUnique<FStringSwitchOperator>(
					MoveTemp(InputTrigger), MoveTemp(OutputTriggers), SwitchValues, DefaultOut, MoveTemp(StringSwitch)
					);
		}

	private:
		TDataReadReference<FTrigger> Input;
		TArray<TDataWriteReference<FTrigger>> Outputs;
		TArray<FString> SwitchValues;
		uint32 DefaultOut;
		FStringReadRef StringSwitch;
	};

	using FStringSwitchNode = TNodeFacade<FStringSwitchOperator>;

	// The node extension must be registered along with the node. 
	METASOUND_REGISTER_NODE_AND_CONFIGURATION(FStringSwitchNode, FMetaSoundStringSwitchNodeConfiguration);
}

FMetaSoundStringSwitchNodeConfiguration::FMetaSoundStringSwitchNodeConfiguration(): DefaultOut(0)
{
}

TInstancedStruct<FMetasoundFrontendClassInterface> FMetaSoundStringSwitchNodeConfiguration::OverrideDefaultInterface(const FMetasoundFrontendClass& InNodeClass) const
{
	using namespace Metasound::Experimental::MetasoundStringSwitchNodePrivate;
	// Override the interface based upon the number of inputs and outputs desired. 
	return TInstancedStruct<FMetasoundFrontendClassInterface>::Make(FMetasoundFrontendClassInterface::GenerateClassInterface(GetVertexInterface(SwitchValues.Num())));
}

TSharedPtr<const Metasound::IOperatorData> FMetaSoundStringSwitchNodeConfiguration::GetOperatorData() const 
{
	// Any data the node configuration wishes to share with the operators can be produced here. 
	return MakeShared<Metasound::Experimental::FStringSwitchOperatorData>(SwitchValues, DefaultOut);
}

#undef LOCTEXT_NAMESPACE // MetasoundExperimentalRuntime
