# MetaSound: Configurable Nodes & Custom Type Registration

## Part 1: Configurable MetaSound Nodes

### What They Are
Configurable MetaSound nodes are nodes whose interface (inputs/outputs) can change dynamically based on configuration properties set in the Details panel. They use a configuration struct that inherits from `FMetaSoundFrontendNodeConfiguration` to allow users to customize node behavior at edit-time.

### What They're For
- Creating nodes with variable numbers of inputs/outputs
- Dynamically changing pin types based on configuration
- Building flexible, reusable node types (e.g., a mixer with 1-100 inputs)
- Customizing node behavior without creating separate node classes

### Where to Explore

**UE 5.7 Engine Location:**
`C:\Program Files\Epic Games\UE_5.7\Engine\Plugins\Experimental\MetasoundExperimental\Source\MetasoundExperimentalRuntime\`

**Key Example Files:**
- **MetasoundExampleNodeConfiguration.cpp/h** - Simple example showing dynamic input/output counts
- **MetasoundCatMakeNode.cpp/h** - Advanced: Channel-based dynamic inputs
- **MetasoundCatBreakNode.cpp/h** - Advanced: Channel-based dynamic outputs
- **MetasoundCatMixerNode.cpp/h** - Advanced: Combined count + format configuration

**Editor Customization:**
- **MetasoundExampleNodeDetailsCustomization.cpp/h** - Custom UI widgets for configuration properties

---

## Part 2: Custom Data Type Registration (For Plugins)

### What's Possible
You can register custom data types in your plugin to use as inputs/outputs in MetaSound nodes:
- Primitive types (custom structs)
- UObject-based types (assets like USoundWave, UAudioBus)
- Array variants (auto-generated)

### Registration Macro

```cpp
REGISTER_METASOUND_DATATYPE(
    DataType,           // C++ type (e.g., FSoundAttenuationAsset)
    "DisplayName",      // Editor display name
    ELiteralType,       // Optional: Use UObjectProxy for UObjects
    UClassName);        // Optional: UClass for UObject types
```

### For UObject Types (e.g., Attenuation Assets)

#### Wrapper Class Pattern

```cpp
// In your header file
class FSoundAttenuationAsset
{
    TSharedPtr<Audio::IProxyData> AttenuationProxy;

public:
    FSoundAttenuationAsset() = default;
    FSoundAttenuationAsset(const FSoundAttenuationAsset&) = default;
    FSoundAttenuationAsset& operator=(const FSoundAttenuationAsset&) = default;

    // REQUIRED: Constructor from proxy data
    FSoundAttenuationAsset(const TSharedPtr<Audio::IProxyData>& InInitData)
        : AttenuationProxy(InInitData)
    {
    }

    const TSharedPtr<Audio::IProxyData>& GetProxy() const
    {
        return AttenuationProxy;
    }
};

// REQUIRED: Declare reference types
DECLARE_METASOUND_DATA_REFERENCE_TYPES(
    FSoundAttenuationAsset,
    YOURPLUGIN_API,
    FSoundAttenuationAssetTypeInfo,
    FSoundAttenuationAssetReadRef,
    FSoundAttenuationAssetWriteRef)
```

#### Registration in Plugin Module

```cpp
// In your plugin module's StartupModule()
void FYourPluginModule::StartupModule()
{
    // Register your custom type
    REGISTER_METASOUND_DATATYPE(
        FSoundAttenuationAsset,
        "SoundAttenuationAsset",
        Metasound::ELiteralType::UObjectProxy,
        USoundAttenuation);
}
```

### Critical Requirement for UObject Types

⚠️ **The UObject class MUST implement `IAudioProxyDataFactory`**

For UObject types, the target UClass (e.g., `USoundAttenuation`) must:
1. Inherit from `IAudioProxyDataFactory`
2. Implement `CreateProxyData()` method

**Problem:** Most engine UClasses (like `USoundAttenuation`) do NOT implement this interface.

**Solutions:**
- Subclass the UObject and add proxy support
- Use a different approach (direct references without proxy system)
- Use existing proxy-compatible types (USoundWave, UAudioBus work out-of-box)

### Plugin Implementation

**Where to Register:**
- In your plugin's Runtime module
- Call `REGISTER_METASOUND_DATATYPE` in `FYourPluginModule::StartupModule()`

**Plugin Compatibility:**
✅ Fully supported in plugins - no engine modifications needed
✅ MetasoundExperimental itself is a plugin
✅ All registration happens at module startup

**Limitations:**
- Target UObject must support `IAudioProxyDataFactory` (not all do)
- Type names must be unique to avoid collisions

### Reference Examples

Study these engine implementations:

**Simplest UObject Type:**
- `MetasoundAudioBus.h` (~50 lines) - Best starting point

**Registration Examples:**
- `MetasoundEngineModule.cpp:50-53` - See how Wave, AudioBus, and WaveTable are registered

**Type System Documentation:**
- `MetasoundDataTypeRegistrationMacro.h` (967 lines) - Full registration system
- `MetasoundFrontendDataTypeRegistry.h` - Registry interface

### Existing Engine Types

These UObject types already work in MetaSound:
- **FWaveAsset** → `USoundWave`
- **FAudioBusAsset** → `UAudioBus`
- **FWaveTableBankAsset** → `UWaveTableBank`

All follow the same wrapper + proxy pattern shown above.
