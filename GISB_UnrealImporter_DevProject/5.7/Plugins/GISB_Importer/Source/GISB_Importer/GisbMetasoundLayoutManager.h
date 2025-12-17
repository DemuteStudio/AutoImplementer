// Copyright Demute SRL. All Rights Reserved.

#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "MetasoundBuilderBase.h"
#include "MetasoundFrontend.h"

// Hash function for FMetaSoundNodeHandle to allow it as a TMap key
inline uint32 GetTypeHash(const FMetaSoundNodeHandle& Handle)
{
	return GetTypeHash(Handle.NodeID);
}

// Equality operator for FMetaSoundNodeHandle to allow it as a TMap key
inline bool operator==(const FMetaSoundNodeHandle& A, const FMetaSoundNodeHandle& B)
{
	return A.NodeID == B.NodeID;
}

/**
 * Node category for layout placement
 */
enum class EGisbNodeCategory : uint8
{
	SignalSource,          // Wave players, triggers
	SignalProcessor,       // Mixers, reroute, random/switch
	SignalOutput,          // Graph outputs
	ParameterInput,        // Graph inputs (non-trigger)
	ParameterProcessor,    // Volume, pitch, lowpass
	TriggerFlow,           // Trigger Any/Accumulate
	Unknown
};

/**
 * Layout configuration parameters
 */
struct FGisbLayoutConfig
{
	float HorizontalSpacing = 300.0f;               // Between columns
	float VerticalSpacing = 150.0f;                 // Between rows
	float ParameterLayerOffset = 250.0f;            // Above/below offset
	float HighConnectionSpacingMultiplier = 1.5f;   // Multiplier for high-connection nodes
	int32 HighConnectionThreshold = 3;              // Connection count threshold
	FVector2D OriginPosition = FVector2D(0, 0);     // Starting position

	// Configuration presets
	static FGisbLayoutConfig Compact();
	static FGisbLayoutConfig Spacious();            // Default
	static FGisbLayoutConfig Debug();
};

/**
 * Node metadata tracking for layout computation
 */
struct FGisbNodeInfo
{
	FMetaSoundNodeHandle NodeHandle;
	EGisbNodeCategory Category;
	FString DebugName;

	// Layout position indices
	int32 LayoutColumn = -1;          // Horizontal position (left-to-right)
	int32 LayoutRow = -1;             // Vertical position within column
	int32 ParameterLayer = 0;         // 0=main, +1=above, -1=below

	// Connection tracking
	int32 IncomingConnectionCount = 0;
	int32 OutgoingConnectionCount = 0;
	TArray<FMetaSoundNodeHandle> ConnectedTo;
	TArray<FMetaSoundNodeHandle> ConnectedFrom;

	// Final computed position
	FVector2D Position = FVector2D::ZeroVector;
};

/**
 * Automatic node positioning system for MetaSound patches.
 * Implements hybrid hierarchical layout: left-to-right signal flow with
 * parameter nodes positioned above/below to reduce width.
 *
 * Usage:
 *   GisbMetasoundLayoutManager Layout(builder);
 *   Layout.RegisterNode(nodeHandle, EGisbNodeCategory::SignalSource, "WavePlayer");
 *   Layout.RegisterConnection(fromHandle, toHandle);
 *   Layout.ComputeLayout();
 *   Layout.ApplyLayout();
 */
class GISB_IMPORTER_API GisbMetasoundLayoutManager
{
public:
	/**
	 * Constructor
	 * @param Builder - MetaSound builder to operate on
	 * @param Config - Layout configuration (defaults to Spacious)
	 */
	GisbMetasoundLayoutManager(UMetaSoundBuilderBase* Builder, const FGisbLayoutConfig& Config = FGisbLayoutConfig::Spacious());

	// ========================== Registration API ==========================

	/**
	 * Register a node for layout positioning
	 * @param NodeHandle - Handle to the MetaSound node
	 * @param Category - Node category for layout placement
	 * @param DebugName - Human-readable name for debugging
	 */
	void RegisterNode(FMetaSoundNodeHandle NodeHandle, EGisbNodeCategory Category, const FString& DebugName);

	/**
	 * Register a connection between nodes
	 * @param FromNode - Source node handle
	 * @param ToNode - Destination node handle
	 */
	void RegisterConnection(FMetaSoundNodeHandle FromNode, FMetaSoundNodeHandle ToNode);

	/**
	 * Register a graph input node
	 * @param NodeHandle - Handle to the input node
	 * @param PinName - Name of the input pin
	 */
	void RegisterGraphInputNode(FMetaSoundNodeHandle NodeHandle, const FName& PinName);

	/**
	 * Register a graph output node
	 * @param NodeHandle - Handle to the output node
	 * @param PinName - Name of the output pin
	 */
	void RegisterGraphOutputNode(FMetaSoundNodeHandle NodeHandle, const FName& PinName);

	// ========================== Layout Execution ==========================

	/**
	 * Compute node positions based on registered nodes and connections
	 * Executes layout algorithm: column assignment, layer computation, row sorting, spacing
	 */
	void ComputeLayout();

	/**
	 * Apply computed positions to MetaSound nodes via SetNodeLocation
	 */
	void ApplyLayout();

	// ========================== Configuration ==========================

	/**
	 * Update layout configuration
	 * @param NewConfig - New configuration to use
	 */
	void SetConfig(const FGisbLayoutConfig& NewConfig);

	/**
	 * Enable or disable debug logging
	 * @param bEnable - True to enable detailed logging
	 */
	void EnableDebugLogging(bool bEnable);

private:
	// ========================== Member Variables ==========================

	UMetaSoundBuilderBase* Builder;
	FGisbLayoutConfig Config;
	TMap<FMetaSoundNodeHandle, FGisbNodeInfo> NodeRegistry;
	bool bDebugLogging = false;

	// ========================== Layout Algorithm Steps ==========================

	/**
	 * Phase 1: Assign column indices via BFS traversal
	 * Source nodes (no incoming connections) → Column 0
	 * Downstream nodes → Parent column + 1
	 */
	void AssignColumns();

	/**
	 * Phase 2: Compute parameter layer assignments
	 * ParameterInput/ParameterProcessor → Layer +1 (above)
	 * SignalSource/SignalProcessor/SignalOutput → Layer 0 (main)
	 */
	void ComputeParameterLayers();

	/**
	 * Phase 3: Assign row indices within each column
	 * Sort by connection count (high connections toward center)
	 */
	void AssignRows();

	/**
	 * Phase 4: Apply smart spacing for high-connection nodes
	 * Increases spacing multiplier when connection count exceeds threshold
	 */
	void ApplySmartSpacing();

	/**
	 * Phase 5: Compute final FVector2D positions
	 * X = OriginX + (Column * HorizontalSpacing)
	 * Y = OriginY + (Layer * ParameterLayerOffset) + (Row * VerticalSpacing)
	 */
	void ComputeFinalPositions();

	// ========================== Helper Methods ==========================

	/**
	 * Calculate X position for a given column
	 * @param Column - Column index
	 * @return X coordinate
	 */
	float GetColumnXPosition(int32 Column);

	/**
	 * Calculate Y position for a given row and layer
	 * @param Column - Column index (for smart spacing)
	 * @param Row - Row index within column
	 * @param Layer - Parameter layer (0=main, +1=above, -1=below)
	 * @return Y coordinate
	 */
	float GetRowYPosition(int32 Column, int32 Row, int32 Layer);

	/**
	 * Log layout information for debugging
	 */
	void LogLayoutInfo();
};

#endif // WITH_EDITOR
