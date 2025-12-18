# MetasoundLayoutManager Guide

**Automatic Node Positioning for MetaSound Patches**

---

## Overview

When generating MetaSound patches programmatically, all nodes spawn at position (0,0) by default, creating visual spaghetti. **GisbMetasoundLayoutManager** automatically arranges nodes in a readable left-to-right layout.

**Key Features:**
- Arranges nodes left-to-right following signal flow
- Places parameter nodes above main signal flow
- Adjusts spacing based on connection density
- Zero runtime cost (editor-only code)

---

## Quick Start

### Basic Usage Pattern

```cpp
#include "GisbMetasoundLayoutManager.h"

void BuildMetasoundPatch()
{
#if WITH_EDITOR
    // 1. Create layout manager
    GisbMetasoundLayoutManager Layout(builder);
#endif

    // 2. Create nodes and register them
    FMetaSoundNodeHandle waveNode = builder->AddNodeByClassName(...);
#if WITH_EDITOR
    Layout.RegisterNode(waveNode, EGisbNodeCategory::SignalSource, TEXT("WavePlayer"));
#endif

    // 3. Connect nodes and register connections
    builder->ConnectNodes(triggerNode, waveNode, result);
#if WITH_EDITOR
    Layout.RegisterConnection(triggerNode, waveNode);
#endif

    // 4. Compute and apply layout
#if WITH_EDITOR
    Layout.ComputeLayout();
    Layout.ApplyLayout();
#endif

    // 5. Build asset
    return EditorSubsystem->BuildToAsset(builder, ...);
}
```

**Important:** Always wrap layout code in `#if WITH_EDITOR` to keep it out of shipping builds.

---

## Node Categories

Choose the right category when registering nodes:

| Category | Use For | Examples |
|----------|---------|----------|
| **SignalSource** | Audio generators | Wave Player, Trigger inputs |
| **SignalProcessor** | Audio processing | Mixer, effects, routing |
| **SignalOutput** | Graph outputs | Audio Out, On Finished |
| **ParameterInput** | Parameter inputs | Volume param, Pitch param |
| **ParameterProcessor** | Parameter processing | Volume node, Pitch node |
| **TriggerFlow** | Trigger routing | Trigger Any, Trigger Accumulate |

**Main flow** (SignalSource → SignalProcessor → SignalOutput) arranges left-to-right.
**Parameter nodes** (ParameterInput, ParameterProcessor) position above main flow.

---

## Configuration

### Presets

```cpp
// Compact (simple patches < 10 nodes)
GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Compact());

// Spacious (default, most patches 10-30 nodes)
GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Spacious());

// Debug (complex patches > 30 nodes)
GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Debug());
```

### Custom Configuration

```cpp
FGisbLayoutConfig Config;
Config.HorizontalSpacing = 500.0f;      // Distance between columns
Config.VerticalSpacing = 250.0f;        // Distance between rows
Config.ParameterLayerOffset = 350.0f;   // Offset for parameter layer

GisbMetasoundLayoutManager Layout(builder, Config);
```

---

## Complete Example

**Simple wave player with volume control:**

```cpp
#if WITH_EDITOR
#include "GisbMetasoundLayoutManager.h"
#endif

TScriptInterface<IMetaSoundDocumentInterface> BuildSimpleSound(
    USoundWave* WaveAsset,
    const FString& PatchName)
{
    // Get subsystems
    UMetaSoundBuilderSubsystem* BuilderSys =
        GEngine->GetEngineSubsystem<UMetaSoundBuilderSubsystem>();
    UMetasoundEditorSubsystem* EditorSys =
        GEditor->GetEditorSubsystem<UMetasoundEditorSubsystem>();

    // Create builder
    EMetaSoundBuilderResult Result;
    UMetaSoundSourceBuilder* Builder =
        BuilderSys->CreateSourceBuilder(FName(*PatchName), Result);

#if WITH_EDITOR
    // Create layout manager
    GisbMetasoundLayoutManager Layout(Builder);
#endif

    // Add play trigger
    FMetaSoundBuilderNodeOutputHandle PlayTrigger =
        Builder->AddGraphInputNode(FName("Play"),
            GET_METASOUND_DATA_TYPE_NAME(Trigger),
            FMetasoundFrontendLiteral(), Result);

#if WITH_EDITOR
    Layout.RegisterGraphInputNode(PlayTrigger.NodeHandle, FName("Play"));
#endif

    // Add wave player
    static FMetasoundFrontendClassName WavePlayerClass =
        {TEXT("UE"), TEXT("WavePlayer"), TEXT("")};
    FMetaSoundNodeHandle WaveNode =
        Builder->AddNodeByClassName(WavePlayerClass, Result);

#if WITH_EDITOR
    Layout.RegisterNode(WaveNode,
        EGisbNodeCategory::SignalSource, TEXT("WavePlayer"));
#endif

    // Connect trigger to wave player
    FMetaSoundBuilderNodeInputHandle PlayInput =
        Builder->FindNodeInputByName(WaveNode, FName("Play"), Result);
    Builder->ConnectNodes(PlayTrigger, PlayInput, Result);

#if WITH_EDITOR
    Layout.RegisterConnection(PlayTrigger.NodeHandle, WaveNode);
#endif

    // Add volume parameter
    FMetaSoundBuilderNodeOutputHandle VolumeParam =
        Builder->AddGraphInputNode(FName("Volume"),
            GET_METASOUND_DATA_TYPE_NAME(Float),
            FMetasoundFrontendLiteral(1.0f), Result);

#if WITH_EDITOR
    Layout.RegisterGraphInputNode(VolumeParam.NodeHandle, FName("Volume"));
#endif

    // Add volume processor
    static FMetasoundFrontendClassName VolumeClass =
        {TEXT("GISB"), TEXT("Volume"), TEXT("")};
    FMetaSoundNodeHandle VolumeNode =
        Builder->AddNodeByClassName(VolumeClass, Result);

#if WITH_EDITOR
    Layout.RegisterNode(VolumeNode,
        EGisbNodeCategory::ParameterProcessor, TEXT("Volume"));
#endif

    // Connect volume param and audio
    FMetaSoundBuilderNodeInputHandle VolumeParamInput =
        Builder->FindNodeInputByName(VolumeNode, FName("Volume"), Result);
    Builder->ConnectNodes(VolumeParam, VolumeParamInput, Result);

#if WITH_EDITOR
    Layout.RegisterConnection(VolumeParam.NodeHandle, VolumeNode);
#endif

    FMetaSoundBuilderNodeOutputHandle AudioOut =
        Builder->FindNodeOutputByName(WaveNode, FName("Audio"), Result);
    FMetaSoundBuilderNodeInputHandle VolumeAudioIn =
        Builder->FindNodeInputByName(VolumeNode, FName("Audio In"), Result);
    Builder->ConnectNodes(AudioOut, VolumeAudioIn, Result);

#if WITH_EDITOR
    Layout.RegisterConnection(WaveNode, VolumeNode);
#endif

    // Add graph output
    FMetaSoundBuilderNodeOutputHandle VolumeAudioOut =
        Builder->FindNodeOutputByName(VolumeNode, FName("Audio Out"), Result);
    FMetaSoundBuilderNodeInputHandle GraphAudioOut =
        Builder->AddGraphOutputNode(FName("Audio"),
            GET_METASOUND_DATA_TYPE_NAME(Audio),
            FMetasoundFrontendLiteral(), Result);

#if WITH_EDITOR
    Layout.RegisterGraphOutputNode(GraphAudioOut.NodeHandle, FName("Audio"));
#endif

    Builder->ConnectNodes(VolumeAudioOut, GraphAudioOut, Result);

#if WITH_EDITOR
    Layout.RegisterConnection(VolumeNode, GraphAudioOut.NodeHandle);
#endif

    // Compute and apply layout
#if WITH_EDITOR
    Layout.ComputeLayout();
    Layout.ApplyLayout();
#endif

    // Build asset
    return EditorSys->BuildToAsset(Builder,
        FName(*PatchName),
        FString(TEXT("/Game/Metasounds")),
        Result);
}
```

**Result:**
```
Layer +1:  [Play Trigger]  [Volume Param]
                |               |
Layer 0:   [Wave Player]-------[Volume]-----[Audio Out]
```

---

## Debug Logging

Enable detailed logging during development:

```cpp
Layout.EnableDebugLogging(true);
```

**Output includes:**
- Node registrations
- Connection tracking
- Layout bounds (columns/rows)
- Final positions for each node

---

## Common Issues

### Nodes Still at (0, 0)

**Check:**
- Called `ComputeLayout()` before `ApplyLayout()`
- Called `ApplyLayout()` before `BuildToAsset()`
- Builder is valid

**Debug:**
```cpp
Layout.EnableDebugLogging(true);
// Check log for "Applied layout" messages
```

### Overlapping Nodes

**Check:**
- All nodes registered with `RegisterNode()`
- Using unique debug names
- Try Spacious or Debug preset

### Unexpected Layout

**Check:**
- Node categories are correct
- Connections registered after `ConnectNodes()`
- Review debug log output

### Build Errors

**Check:**
- All layout code wrapped in `#if WITH_EDITOR`
- MetasoundEditor in `PrivateDependencyModuleNames` (Build.cs)
- Header include wrapped in `#if WITH_EDITOR`

---

## Best Practices

**Registration:**
- Register nodes immediately after creation
- Register connections immediately after `ConnectNodes()`
- Use descriptive debug names

**Configuration:**
- Start with Spacious preset (default)
- Use Debug preset for complex patches
- Use Compact only for very simple patches

**Debugging:**
- Enable debug logging during development
- Disable logging for production imports
- Check Output Log if layout looks wrong

---

## Files Reference

**Source Files:**
- `Plugins/GISB_Importer/Source/GISB_Importer/GisbMetasoundLayoutManager.h`
- `Plugins/GISB_Importer/Source/GISB_Importer/GisbMetasoundLayoutManager.cpp`

**Build Configuration:**
- `Plugins/GISB_Importer/Source/GISB_Importer/GISB_Importer.Build.cs` (requires MetasoundEditor dependency)

**Integration Example:**
- `Plugins/GISB_Importer/Source/GISB_Importer/GISB_MetasoundBuilder.cpp` - See `BuildSimpleSoundNode()` function

**Related Documentation:**
- `.documentation/ISX_Format_Specification.md` - ISX container types
- `.documentation/GISB_Importer_Technical_Documentation.md` - Plugin architecture
- `GISB_UnrealImporter_DevProject/5.7/CLAUDE.md` - Project setup

---

*GISB Importer Plugin | Unreal Engine 5.7*
