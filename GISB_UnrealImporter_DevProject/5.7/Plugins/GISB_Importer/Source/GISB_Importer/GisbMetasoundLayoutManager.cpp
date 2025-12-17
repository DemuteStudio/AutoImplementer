// Copyright Demute SRL. All Rights Reserved.

#if WITH_EDITOR

#include "GisbMetasoundLayoutManager.h"
#include "MetasoundEditorModule.h"

// ========================== Configuration Presets ==========================

FGisbLayoutConfig FGisbLayoutConfig::Compact()
{
	FGisbLayoutConfig Config;
	Config.HorizontalSpacing = 200.0f;
	Config.VerticalSpacing = 100.0f;
	Config.ParameterLayerOffset = 180.0f;
	Config.HighConnectionSpacingMultiplier = 1.3f;
	Config.HighConnectionThreshold = 3;
	Config.OriginPosition = FVector2D(0, 0);
	return Config;
}

FGisbLayoutConfig FGisbLayoutConfig::Spacious()
{
	FGisbLayoutConfig Config;
	Config.HorizontalSpacing = 400.0f;
	Config.VerticalSpacing = 200.0f;
	Config.ParameterLayerOffset = 300.0f;
	Config.HighConnectionSpacingMultiplier = 1.5f;
	Config.HighConnectionThreshold = 3;
	Config.OriginPosition = FVector2D(0, 0);
	return Config;
}

FGisbLayoutConfig FGisbLayoutConfig::Debug()
{
	FGisbLayoutConfig Config;
	Config.HorizontalSpacing = 600.0f;
	Config.VerticalSpacing = 300.0f;
	Config.ParameterLayerOffset = 400.0f;
	Config.HighConnectionSpacingMultiplier = 2.0f;
	Config.HighConnectionThreshold = 2;
	Config.OriginPosition = FVector2D(0, 0);
	return Config;
}

// ========================== Constructor ==========================

GisbMetasoundLayoutManager::GisbMetasoundLayoutManager(UMetaSoundBuilderBase* InBuilder, const FGisbLayoutConfig& InConfig)
	: Builder(InBuilder)
	, Config(InConfig)
{
	if (!Builder)
	{
		UE_LOG(LogTemp, Error, TEXT("GisbMetasoundLayoutManager: Builder is null!"));
	}
}

// ========================== Configuration ==========================

void GisbMetasoundLayoutManager::SetConfig(const FGisbLayoutConfig& NewConfig)
{
	Config = NewConfig;
}

void GisbMetasoundLayoutManager::EnableDebugLogging(bool bEnable)
{
	bDebugLogging = bEnable;
}

// ========================== Registration API ==========================

void GisbMetasoundLayoutManager::RegisterNode(FMetaSoundNodeHandle NodeHandle, EGisbNodeCategory Category, const FString& DebugName)
{
	if (!NodeHandle.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("RegisterNode: Invalid node handle for %s"), *DebugName);
		return;
	}

	FGisbNodeInfo NodeInfo;
	NodeInfo.NodeHandle = NodeHandle;
	NodeInfo.Category = Category;
	NodeInfo.DebugName = DebugName;

	NodeRegistry.Add(NodeHandle, NodeInfo);

	if (bDebugLogging)
	{
		UE_LOG(LogTemp, Log, TEXT("Registered node: %s (Category: %d)"), *DebugName, static_cast<int32>(Category));
	}
}

void GisbMetasoundLayoutManager::RegisterConnection(FMetaSoundNodeHandle FromNode, FMetaSoundNodeHandle ToNode)
{
	if (!FromNode.IsSet() || !ToNode.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("RegisterConnection: Invalid node handle(s)"));
		return;
	}

	// Update connection tracking
	if (FGisbNodeInfo* FromInfo = NodeRegistry.Find(FromNode))
	{
		FromInfo->ConnectedTo.AddUnique(ToNode);
		FromInfo->OutgoingConnectionCount++;
	}

	if (FGisbNodeInfo* ToInfo = NodeRegistry.Find(ToNode))
	{
		ToInfo->ConnectedFrom.AddUnique(FromNode);
		ToInfo->IncomingConnectionCount++;
	}

	if (bDebugLogging)
	{
		FString FromName = NodeRegistry.Contains(FromNode) ? NodeRegistry[FromNode].DebugName : TEXT("Unknown");
		FString ToName = NodeRegistry.Contains(ToNode) ? NodeRegistry[ToNode].DebugName : TEXT("Unknown");
		UE_LOG(LogTemp, Log, TEXT("Registered connection: %s -> %s"), *FromName, *ToName);
	}
}

void GisbMetasoundLayoutManager::RegisterGraphInputNode(FMetaSoundNodeHandle NodeHandle, const FName& PinName)
{
	// Determine category based on pin name
	EGisbNodeCategory Category = EGisbNodeCategory::ParameterInput;
	if (PinName.ToString().Contains(TEXT("Trigger")) || PinName.ToString().Contains(TEXT("Play")))
	{
		Category = EGisbNodeCategory::TriggerFlow;
	}

	RegisterNode(NodeHandle, Category, FString::Printf(TEXT("Input: %s"), *PinName.ToString()));
}

void GisbMetasoundLayoutManager::RegisterGraphOutputNode(FMetaSoundNodeHandle NodeHandle, const FName& PinName)
{
	RegisterNode(NodeHandle, EGisbNodeCategory::SignalOutput, FString::Printf(TEXT("Output: %s"), *PinName.ToString()));
}

// ========================== Layout Execution ==========================

void GisbMetasoundLayoutManager::ComputeLayout()
{
	if (!Builder)
	{
		UE_LOG(LogTemp, Error, TEXT("ComputeLayout: Builder is null!"));
		return;
	}

	if (NodeRegistry.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ComputeLayout: No nodes registered"));
		return;
	}

	if (bDebugLogging)
	{
		UE_LOG(LogTemp, Log, TEXT("ComputeLayout: Starting layout for %d nodes"), NodeRegistry.Num());
	}

	// Execute layout algorithm phases
	AssignColumns();
	ComputeParameterLayers();
	AssignRows();
	ApplySmartSpacing();
	ComputeFinalPositions();

	if (bDebugLogging)
	{
		LogLayoutInfo();
	}
}

void GisbMetasoundLayoutManager::ApplyLayout()
{
	if (!Builder)
	{
		UE_LOG(LogTemp, Error, TEXT("ApplyLayout: Builder is null!"));
		return;
	}

	int32 SuccessCount = 0;
	int32 FailCount = 0;

	for (auto& Pair : NodeRegistry)
	{
		FGisbNodeInfo& NodeInfo = Pair.Value;
		EMetaSoundBuilderResult Result;

		Builder->SetNodeLocation(NodeInfo.NodeHandle, NodeInfo.Position, Result);

		if (Result == EMetaSoundBuilderResult::Succeeded)
		{
			SuccessCount++;
			if (bDebugLogging)
			{
				UE_LOG(LogTemp, Log, TEXT("Applied layout: %s at (%f, %f)"),
					*NodeInfo.DebugName, NodeInfo.Position.X, NodeInfo.Position.Y);
			}
		}
		else
		{
			FailCount++;
			UE_LOG(LogTemp, Warning, TEXT("Failed to set position for: %s"), *NodeInfo.DebugName);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("ApplyLayout: %d succeeded, %d failed"), SuccessCount, FailCount);
}

// ========================== Layout Algorithm: Phase 1 - Column Assignment ==========================

void GisbMetasoundLayoutManager::AssignColumns()
{
	// BFS traversal for left-to-right column assignment

	TQueue<FMetaSoundNodeHandle> Queue;
	TSet<FMetaSoundNodeHandle> Visited;

	// Find all source nodes (no incoming connections) → Column 0
	for (auto& Pair : NodeRegistry)
	{
		FGisbNodeInfo& NodeInfo = Pair.Value;
		if (NodeInfo.IncomingConnectionCount == 0)
		{
			NodeInfo.LayoutColumn = 0;
			Queue.Enqueue(NodeInfo.NodeHandle);
			Visited.Add(NodeInfo.NodeHandle);

			if (bDebugLogging)
			{
				UE_LOG(LogTemp, Log, TEXT("Source node: %s (Column 0)"), *NodeInfo.DebugName);
			}
		}
	}

	// BFS: Assign columns based on parent column + 1
	while (!Queue.IsEmpty())
	{
		FMetaSoundNodeHandle CurrentHandle;
		Queue.Dequeue(CurrentHandle);

		FGisbNodeInfo* CurrentInfo = NodeRegistry.Find(CurrentHandle);
		if (!CurrentInfo)
		{
			continue;
		}

		int32 NextColumn = CurrentInfo->LayoutColumn + 1;

		// Process all downstream nodes
		for (const FMetaSoundNodeHandle& ChildHandle : CurrentInfo->ConnectedTo)
		{
			FGisbNodeInfo* ChildInfo = NodeRegistry.Find(ChildHandle);
			if (!ChildInfo)
			{
				continue;
			}

			// Update column to max of existing or new column (handles multiple parents)
			if (ChildInfo->LayoutColumn < NextColumn)
			{
				ChildInfo->LayoutColumn = NextColumn;
			}

			// Add to queue if not visited
			if (!Visited.Contains(ChildHandle))
			{
				Queue.Enqueue(ChildHandle);
				Visited.Add(ChildHandle);
			}
		}
	}

	// Handle orphan nodes (no connections) - place in column 0
	for (auto& Pair : NodeRegistry)
	{
		FGisbNodeInfo& NodeInfo = Pair.Value;
		if (NodeInfo.LayoutColumn == -1)
		{
			NodeInfo.LayoutColumn = 0;
			UE_LOG(LogTemp, Warning, TEXT("Orphan node assigned to column 0: %s"), *NodeInfo.DebugName);
		}
	}
}

// ========================== Layout Algorithm: Phase 2 - Layer Assignment ==========================

void GisbMetasoundLayoutManager::ComputeParameterLayers()
{
	// Assign layer based on node category
	for (auto& Pair : NodeRegistry)
	{
		FGisbNodeInfo& NodeInfo = Pair.Value;

		switch (NodeInfo.Category)
		{
		case EGisbNodeCategory::ParameterInput:
		case EGisbNodeCategory::ParameterProcessor:
			// Place parameter nodes above main flow
			NodeInfo.ParameterLayer = 1;
			break;

		case EGisbNodeCategory::SignalSource:
		case EGisbNodeCategory::SignalProcessor:
		case EGisbNodeCategory::SignalOutput:
		case EGisbNodeCategory::TriggerFlow:
		case EGisbNodeCategory::Unknown:
		default:
			// Main signal flow
			NodeInfo.ParameterLayer = 0;
			break;
		}

		if (bDebugLogging)
		{
			UE_LOG(LogTemp, Log, TEXT("Layer assignment: %s -> Layer %d"), *NodeInfo.DebugName, NodeInfo.ParameterLayer);
		}
	}
}

// ========================== Layout Algorithm: Phase 3 - Row Assignment ==========================

void GisbMetasoundLayoutManager::AssignRows()
{
	// Group nodes by column and layer
	TMap<int32, TMap<int32, TArray<FMetaSoundNodeHandle>>> ColumnLayerGroups;

	for (auto& Pair : NodeRegistry)
	{
		FGisbNodeInfo& NodeInfo = Pair.Value;
		ColumnLayerGroups.FindOrAdd(NodeInfo.LayoutColumn).FindOrAdd(NodeInfo.ParameterLayer).Add(NodeInfo.NodeHandle);
	}

	// Assign rows within each column/layer group
	for (auto& ColumnPair : ColumnLayerGroups)
	{
		for (auto& LayerPair : ColumnPair.Value)
		{
			TArray<FMetaSoundNodeHandle>& NodesInGroup = LayerPair.Value;

			// Sort by total connection count (high connections toward center)
			NodesInGroup.Sort([this](const FMetaSoundNodeHandle& A, const FMetaSoundNodeHandle& B)
			{
				FGisbNodeInfo* InfoA = NodeRegistry.Find(A);
				FGisbNodeInfo* InfoB = NodeRegistry.Find(B);

				if (!InfoA || !InfoB)
				{
					return false;
				}

				int32 TotalA = InfoA->IncomingConnectionCount + InfoA->OutgoingConnectionCount;
				int32 TotalB = InfoB->IncomingConnectionCount + InfoB->OutgoingConnectionCount;

				return TotalA > TotalB;  // Higher connections first (center)
			});

			// Assign sequential row numbers
			int32 RowIndex = 0;
			for (const FMetaSoundNodeHandle& NodeHandle : NodesInGroup)
			{
				if (FGisbNodeInfo* NodeInfo = NodeRegistry.Find(NodeHandle))
				{
					NodeInfo->LayoutRow = RowIndex++;
				}
			}
		}
	}
}

// ========================== Layout Algorithm: Phase 4 - Smart Spacing ==========================

void GisbMetasoundLayoutManager::ApplySmartSpacing()
{
	// Note: Spacing multiplier will be applied in ComputeFinalPositions
	// This phase can be expanded in future for more sophisticated spacing logic

	if (bDebugLogging)
	{
		UE_LOG(LogTemp, Log, TEXT("Smart spacing: Threshold=%d, Multiplier=%.2f"),
			Config.HighConnectionThreshold, Config.HighConnectionSpacingMultiplier);
	}
}

// ========================== Layout Algorithm: Phase 5 - Position Computation ==========================

void GisbMetasoundLayoutManager::ComputeFinalPositions()
{
	for (auto& Pair : NodeRegistry)
	{
		FGisbNodeInfo& NodeInfo = Pair.Value;

		// X = OriginX + (Column * HorizontalSpacing)
		float X = GetColumnXPosition(NodeInfo.LayoutColumn);

		// Y = OriginY + (Layer * ParameterLayerOffset) + (Row * VerticalSpacing)
		float Y = GetRowYPosition(NodeInfo.LayoutColumn, NodeInfo.LayoutRow, NodeInfo.ParameterLayer);

		NodeInfo.Position = FVector2D(X, Y);

		if (bDebugLogging)
		{
			UE_LOG(LogTemp, Log, TEXT("Position computed: %s -> (%.1f, %.1f) [Col=%d, Row=%d, Layer=%d]"),
				*NodeInfo.DebugName, X, Y, NodeInfo.LayoutColumn, NodeInfo.LayoutRow, NodeInfo.ParameterLayer);
		}
	}
}

// ========================== Helper Methods ==========================

float GisbMetasoundLayoutManager::GetColumnXPosition(int32 Column)
{
	return Config.OriginPosition.X + (Column * Config.HorizontalSpacing);
}

float GisbMetasoundLayoutManager::GetRowYPosition(int32 Column, int32 Row, int32 Layer)
{
	float BaseY = Config.OriginPosition.Y;

	// Apply layer offset
	BaseY += Layer * Config.ParameterLayerOffset;

	// Apply row spacing (with smart spacing for high-connection nodes)
	float EffectiveSpacing = Config.VerticalSpacing;

	// Check if this column has high-connection nodes
	bool bHasHighConnections = false;
	for (auto& Pair : NodeRegistry)
	{
		FGisbNodeInfo& NodeInfo = Pair.Value;
		if (NodeInfo.LayoutColumn == Column && NodeInfo.ParameterLayer == Layer)
		{
			int32 TotalConnections = NodeInfo.IncomingConnectionCount + NodeInfo.OutgoingConnectionCount;
			if (TotalConnections >= Config.HighConnectionThreshold)
			{
				bHasHighConnections = true;
				break;
			}
		}
	}

	if (bHasHighConnections)
	{
		EffectiveSpacing *= Config.HighConnectionSpacingMultiplier;
	}

	BaseY += Row * EffectiveSpacing;

	return BaseY;
}

void GisbMetasoundLayoutManager::LogLayoutInfo()
{
	UE_LOG(LogTemp, Log, TEXT("=============== Layout Info ==============="));
	UE_LOG(LogTemp, Log, TEXT("Total Nodes: %d"), NodeRegistry.Num());

	// Count nodes per category
	TMap<EGisbNodeCategory, int32> CategoryCounts;
	for (auto& Pair : NodeRegistry)
	{
		EGisbNodeCategory Category = Pair.Value.Category;
		CategoryCounts.FindOrAdd(Category, 0)++;
	}

	UE_LOG(LogTemp, Log, TEXT("Category Distribution:"));
	for (auto& CatPair : CategoryCounts)
	{
		UE_LOG(LogTemp, Log, TEXT("  Category %d: %d nodes"), static_cast<int32>(CatPair.Key), CatPair.Value);
	}

	// Find layout bounds
	int32 MinColumn = INT32_MAX, MaxColumn = INT32_MIN;
	int32 MinRow = INT32_MAX, MaxRow = INT32_MIN;
	for (auto& Pair : NodeRegistry)
	{
		MinColumn = FMath::Min(MinColumn, Pair.Value.LayoutColumn);
		MaxColumn = FMath::Max(MaxColumn, Pair.Value.LayoutColumn);
		MinRow = FMath::Min(MinRow, Pair.Value.LayoutRow);
		MaxRow = FMath::Max(MaxRow, Pair.Value.LayoutRow);
	}

	UE_LOG(LogTemp, Log, TEXT("Layout Bounds: Columns [%d, %d], Rows [%d, %d]"), MinColumn, MaxColumn, MinRow, MaxRow);

	// Detailed node info
	UE_LOG(LogTemp, Log, TEXT("Node Details:"));
	for (auto& Pair : NodeRegistry)
	{
		FGisbNodeInfo& NodeInfo = Pair.Value;
		UE_LOG(LogTemp, Log, TEXT("  %s: Col=%d, Row=%d, Layer=%d, Pos=(%.1f, %.1f), Connections=%d/%d"),
			*NodeInfo.DebugName,
			NodeInfo.LayoutColumn,
			NodeInfo.LayoutRow,
			NodeInfo.ParameterLayer,
			NodeInfo.Position.X,
			NodeInfo.Position.Y,
			NodeInfo.IncomingConnectionCount,
			NodeInfo.OutgoingConnectionCount);
	}

	UE_LOG(LogTemp, Log, TEXT("==========================================="));
}

#endif // WITH_EDITOR
