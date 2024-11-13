// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MetasoundDataReference.h"
#include "MetasoundDataTypeRegistrationMacro.h"
#include "IAudioProxyInitializer.h"
#include "MetasoundSource.h"
#include "MetasoundFrontendDataTypeRegistry.h"
#include "DSP/InterpolatedLinearPitchShifter.h"


class FMetaSoundSourceProxy;

using FMetaSoundSourceProxyPtr = TSharedPtr<FMetaSoundSourceProxy, ESPMode::ThreadSafe>;
class FMetaSoundSourceProxy final : public Audio::TProxyData<FMetaSoundSourceProxy>
{
public:
	IMPL_AUDIOPROXY_CLASS(FMetaSoundSourceProxy);

	ENGINE_API explicit FMetaSoundSourceProxy(UMetaSoundSource* InMetaSoundSource);

	FMetaSoundSourceProxy(const FMetaSoundSourceProxy& Other) = default;

	virtual ~FMetaSoundSourceProxy() override {}

	uint32 MetaSoundSourceId = INDEX_NONE;
	int32 NumChannels = INDEX_NONE;
};

namespace Metasound
{
	// Forward declare ReadRef
	class FMetaSoundSourceAsset;
	typedef TDataReadReference<FMetaSoundSourceAsset> FMetaSoundSourceAssetReadRef;

	// Helper utility to test if exact types are required for a datatype.
	template <>
	struct TIsExplicit<FMetaSoundSourceAsset>
	{
		static constexpr bool Value = true;
	};

	// Metasound data type that holds onto a weak ptr. Mostly used as a placeholder until we have a proper proxy type.
	class AUTOIMPLEMENTINGAUDIO_API FMetaSoundSourceAsset
	{
		FMetaSoundSourceProxyPtr SoundMetaSoundSourceProxy;
	public:

		FMetaSoundSourceAsset() = default;
		FMetaSoundSourceAsset(const FMetaSoundSourceAsset&) = default;
		FMetaSoundSourceAsset& operator=(const FMetaSoundSourceAsset& Other) = default;

		FMetaSoundSourceAsset(const TSharedPtr<Audio::IProxyData>& InInitData);

		bool IsSoundMetaSoundSourceValid() const;

		const FMetaSoundSourceProxyPtr& GetSoundMetaSoundSourceProxy() const
		{
			return SoundMetaSoundSourceProxy;
		}

		const FMetaSoundSourceProxy* operator->() const
		{
			return SoundMetaSoundSourceProxy.Get();
		}

		FMetaSoundSourceProxy* operator->()
		{
			return SoundMetaSoundSourceProxy.Get();
		}
	};

	DECLARE_METASOUND_DATA_REFERENCE_TYPES(FMetaSoundSourceAsset, AUTOIMPLEMENTINGAUDIO_API, FMetaSoundSourceAssetTypeInfo, FMetaSoundSourceAssetReadRef, FMetaSoundSourceAssetWriteRef)
	REGISTER_METASOUND_DATATYPE(Metasound::FMetaSoundSourceAsset, "MetasoundSource", Metasound::ELiteralType::UObjectProxy, UMetaSoundSource);
}
 

