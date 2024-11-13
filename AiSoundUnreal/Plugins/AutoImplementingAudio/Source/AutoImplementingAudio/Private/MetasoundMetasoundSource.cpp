// Fill out your copyright notice in the Description page of Project Settings.

#include "MetasoundMetasoundSource.h"

namespace Metasound
{
	FMetaSoundSourceAsset::FMetaSoundSourceAsset(const TSharedPtr<Audio::IProxyData>& InInitData)
	{
		if (InInitData.IsValid())
		{
			if (InInitData->CheckTypeCast<FMetaSoundSourceProxy>())
			{
				// should we be getting handed a SharedPtr here?
				SoundMetaSoundSourceProxy = MakeShared<FMetaSoundSourceProxy, ESPMode::ThreadSafe>(InInitData->GetAs<FMetaSoundSourceProxy>());
			}
		}
	}

	bool FMetaSoundSourceAsset::IsSoundMetaSoundSourceValid() const
	{
		return SoundMetaSoundSourceProxy.IsValid();
	}
}
