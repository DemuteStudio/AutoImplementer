# Interactive Sound Exchange (ISX) Format Specification

> **Note:** ISX was previously known internally as GISB. Type identifiers retain the `GISB` prefix for compatibility.

## Table of Contents

- [1. Introduction](#1-introduction)
- [2. File Organization](#2-file-organization)
- [3. Type System](#3-type-system)
- [4. Container Types](#4-container-types)
- [5. Randomizable Parameters](#5-randomizable-parameters)
- [6. 3D Spatialization (Attenuation)](#6-3d-spatialization-attenuation)
- [7. Runtime Parameters & Modulation](#7-runtime-parameters--modulation)
- [8. Playback Semantics](#8-playback-semantics)
- [9. Field Reference](#9-field-reference)
- [Appendix A: Examples](#appendix-a-examples)

---

## 1. Introduction

### What is ISX?

ISX (Interactive Sound Exchange) is a JSON-based format for describing interactive audio logic. It defines hierarchical sound containers that can be parsed and played back by any compatible interpreter.

### Design Goals

- **Portability:** Engine-agnostic representation of audio behavior
- **Hierarchy:** Composite pattern allowing containers to nest within containers
- **Interactivity:** Runtime parameters control playback decisions
- **Variation:** Built-in randomization to reduce repetitive audio

### Format Overview

ISX uses JSON with polymorphic type information. Audio logic is organized into **Banks** (collections of events) and **Events** (playable sound definitions). Each event contains a tree of **Containers** that define what audio plays and how.

---

## 2. File Organization

### Directory Structure

```
BankName/
├── Bank_BankName.json      # Bank definition
├── Events/                 # Event definitions
│   ├── EventName1.json
│   └── EventName2.json
└── Audio Files/            # Audio assets
    ├── sound1.ogg
    └── sound2.wav
```

### Bank File

Banks group related events. The interpreter loads a bank, which references its events by name.

| Field | Type | Description |
|-------|------|-------------|
| `name` | string | Bank identifier |
| `hideFlags` | int | Legacy (always `0`) |
| `events` | string[] | Event names (without `.json` extension) |

```json
{
  "name": "Bank_Footsteps",
  "hideFlags": 0,
  "events": ["Footstep_Walk", "Footstep_Run"]
}
```

### Event File

Events are the playable units. Each event has a root container that defines its audio hierarchy.

| Field | Type | Description |
|-------|------|-------------|
| `name` | string | Event identifier |
| `hideFlags` | int | Legacy (always `0`) |
| `rootAudioObject` | object | Root container |

```json
{
  "name": "Footstep_Walk",
  "hideFlags": 0,
  "rootAudioObject": { ... }
}
```

### Audio File References

The `soundClip` field in Simple containers references audio files by filename only (no path). Files are resolved from the `Audio Files/` directory.

---

## 3. Type System

### Polymorphic Deserialization

Containers use the `$type` field for polymorphic deserialization. This field contains a fully-qualified type identifier that the interpreter uses to determine which container class to instantiate.

```json
{
  "$type": "GISB.Runtime.GISB_SingleSound, Demute.GISB",
  "soundClip": "example.ogg",
  "loop": false
}
```

### Type Identifiers

| Container | Type Identifier |
|-----------|-----------------|
| Simple | `GISB.Runtime.GISB_SingleSound, Demute.GISB` |
| Random | `GISB.Runtime.GISB_RandomSound, Demute.GISB` |
| Switch | `GISB.Runtime.GISB_SwitchSound, Demute.GISB` |
| Blend | `GISB.Runtime.GISB_BlendSound, Demute.GISB` |

### Container Hierarchy (Composite Pattern)

Containers follow the composite pattern:
- **Leaf containers** (Simple) play audio directly
- **Composite containers** (Random, Switch, Blend) contain child containers

Children can be any container type, allowing arbitrary nesting depth.

---

## 4. Container Types

### Base Container Fields

All containers inherit these optional fields:

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `mute` | boolean | false | Silences this container and all descendants (may still be traversed for timing) |
| `volumeDB` | object | null (0) | Volume parameter. See [Randomizable Parameters](#5-randomizable-parameters) |
| `pitchCents` | object | null (0) | Pitch parameter. See [Randomizable Parameters](#5-randomizable-parameters) |
| `lowpass` | object | null (0) | Lowpass filter parameter. See [Randomizable Parameters](#5-randomizable-parameters) |
| `attenuation` | object | null (2D) | 3D spatialization settings. See [Attenuation](#6-3d-spatialization-attenuation) |
| `playbackProbabilityPercent` | float | 100 | Chance to play (0-100). Random 0-100 generated per-play; if > probability, container is skipped |
| `modulators` | array | [] | Runtime modulation bindings. See [Modulator Structure](#modulator-structure) |

---

### Simple (SingleSound)

Plays a single audio file. This is the only leaf container type.

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `soundClip` | string | Yes | Audio filename |
| `loop` | boolean | No | Loop playback continuously |

**Playback behavior:**
- If `loop` is true, audio repeats until explicitly stopped
- If `loop` is false, audio plays once then the voice is released

---

### Random (RandomSound)

Selects one child container to play based on random selection.

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `RandomPlaylist` | array | Yes | Child containers to select from |
| `avoidLastPlayed` | int | No | Number of recent selections to exclude |
| `loop` | boolean | No | Continuously select and play children |
| `transitionMode` | string | No | `"TriggerRate"` or `"Crossfade"` |
| `triggerRate` | float | No | Selections per second (`"TriggerRate"` mode) |
| `crossfadeDuration` | float | No | Crossfade time in seconds (`"Crossfade"` mode) |

**Selection algorithm:**
1. Build candidate list from `RandomPlaylist`
2. Remove the last `avoidLastPlayed` selections from candidates
3. Select randomly from remaining candidates
4. Record selection in history

**Transition modes (when looping):**
- `TriggerRate`: Trigger new selection at specified rate
- `Crossfade`: Crossfade between selections over specified duration

---

### Switch (SwitchSound)

Selects a child container based on a runtime parameter value.

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `ParameterName` | string | Yes | Runtime parameter to evaluate. See [Switch Parameters](#switch-parameters) |
| `DefaultValue` | string | Yes | Fallback value if parameter not set |
| `SwitchCases` | array | Yes | Array of case definitions |

**SwitchCase structure:**

| Field | Type | Description |
|-------|------|-------------|
| `parameterValue` | string | Value to match |
| `audioObject` | object | Container to play when matched |

**Selection behavior:**
1. Query runtime parameter by `ParameterName`
2. If parameter not set, use `DefaultValue`
3. Find case where `parameterValue` matches
4. Play the matched case's `audioObject`
5. If no match found, play nothing

---

### Blend (BlendSound)

Plays all child containers simultaneously (layered audio).

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `BlendPlaylist` | array | Yes | Child containers to play together |

**Playback behavior:**
- All children in `BlendPlaylist` are instantiated and played concurrently
- The blend completes when all children complete (or continues if any child loops)

---

## 5. Randomizable Parameters

Volume, Pitch, and Lowpass use a randomizable structure:

| Field | Type | Description |
|-------|------|-------------|
| `randomize` | boolean | Enable random variation |
| `baseValue` | float | Base value |
| `minModifier` | float | Minimum random offset |
| `maxModifier` | float | Maximum random offset |

**Evaluation:**
```
if randomize:
    value = baseValue + random(minModifier, maxModifier)
else:
    value = baseValue
```

#### Volume (`volumeDB`)

- **Unit:** Decibels (dB)
- **Application:** Multiply audio amplitude by `10^(dB/20)`
- **Typical range:** -60 (near silent) to 0 (full volume)

#### Pitch (`pitchCents`)

- **Unit:** Cents (100 cents = 1 semitone)
- **Application:** Multiply playback rate by `2^(cents/1200)`
- **Example:** +1200 cents = double speed (one octave up)

#### Lowpass (`lowpass`)

- **Unit:** Percentage (0-100)
- **Application:** 0 = no filtering, 100 = maximum filtering
- **Implementation:** Map to lowpass filter cutoff frequency

---

## 6. 3D Spatialization (Attenuation)

### Attenuation Structure

| Field | Type | Description |
|-------|------|-------------|
| `active` | boolean | Enable 3D spatialization |
| `minDistance` | float | Distance where attenuation begins |
| `maxDistance` | float | Distance where sound is inaudible |
| `preset` | int | Falloff curve preset (`"Linear"`, `"Logarithmic"`, `"Inverse"` or `"Custom"`) |
| `curve` | object | Custom curve (when preset=3 `"Custom"`) |
| `volumeAtMaxDistance` | float | Volume reduction at max distance (dB) |
| `spreadAtMinDistance` | float | Stereo spread at min distance (0 = mono/point source, 1 = full stereo) |
| `spreadAtMaxDistance` | float | Stereo spread at max distance (0 = mono/point source, 1 = full stereo) |
| `lowPassAtMinDistance` | float | Lowpass amount at min distance (0-1) to stimulate air absorption |
| `lowPassAtMaxDistance` | float | Lowpass amount at max distance (0-1) to stimulate air absorption |

### Inheritance

Attenuation uses an inheritable wrapper structure:

| Field | Type | Description |
|-------|------|-------------|
| `overrideParent` | boolean | If true, use this value; if false, inherit from parent |
| `value` | object | Attenuation settings (structure above) |

**Inheritance behavior:**
- Walk up the container hierarchy
- Use the first attenuation where `overrideParent` is true
- If none found, the audio plays in 2D (no spatialization)

### Distance Model

- Below `minDistance`: No attenuation applied
- Between `minDistance` and `maxDistance`: Attenuation interpolated by curve
- Above `maxDistance`: Full attenuation (`volumeAtMaxDistance`)

- Both `spread` and `lowpass` interpolate linearly between their min and max values

### Falloff Presets

| Preset | Value | Formula |
|--------|-------|---------|
| Linear | 0 | `1 - (distance - min) / (max - min)` |
| Logarithmic | 1 | `1 / (1 + log(distance / min))` |
| Inverse | 2 | `min / distance` |
| Custom | 3 | Evaluate `curve` at normalized distance |

### Custom Curves

Custom curves use Unity's AnimationCurve serialization format. For full details, see the [Unity AnimationCurve documentation](https://docs.unity3d.com/ScriptReference/AnimationCurve.html).

```json
{
  "keys": [
    {"time": 0.0, "value": 1.0, "inTangent": 0.0, "outTangent": -1.0, "inWeight": 0.33, "outWeight": 0.33, "weightedMode": 0},
    {"time": 1.0, "value": 0.0, "inTangent": -1.0, "outTangent": 0.0, "inWeight": 0.33, "outWeight": 0.33, "weightedMode": 0}
  ],
  "length": 2,
  "preWrapMode": 8,
  "postWrapMode": 8
}
```

**Top-level fields:**
- `keys`: Array of keyframes
- `length`: Number of keyframes (informational)
- `preWrapMode` / `postWrapMode`: Behavior outside curve bounds (8 = ClampForever)

**Keyframe fields:**
- `time`: Normalized distance (0 = minDistance, 1 = maxDistance)
- `value`: Attenuation multiplier (1 = full volume, 0 = silent)
- `inTangent` / `outTangent`: Slope entering/exiting the keyframe (used for cubic Hermite interpolation)
- `inWeight` / `outWeight`: Tangent weighting (0-1), typically 0.33 for standard curves
- `weightedMode`: 0 = no weighting, 1 = in weighted, 2 = out weighted, 3 = both

> **Note:** This is Unity-specific serialization. Non-Unity interpreters should implement cubic Hermite spline interpolation or use a simplified linear interpolation between keyframes.

---

## 7. Runtime Parameters & Modulation

### Switch Parameters

Switch containers query string-valued parameters at play time.

**Runtime API requirement:** Interpreters must provide a mechanism to set string parameters by name:
```
SetParameter("SurfaceType", "Concrete")
```

### Modulator Parameters

Modulators bind to float parameters in the 0-1 range for control over audio at runtime.

### Modulator Structure

| Field | Type | Description |
|-------|------|-------------|
| `Target` | string | Parameter to modulate (`"Volume"`) |
| `ParameterName` | string | Runtime parameter name |
| `Curve` | object | Animation curve mapping input to output |

**Currently supported targets:**
- `"Volume"` - Multiplies output volume

### Curve Evaluation

1. Query runtime parameter value (0-1 range)
2. Evaluate curve at that input value
3. Apply result as multiplier to target

**Example:** A curve that maps 0→0.5 and 1→1.0 would output half volume when the parameter is 0, full volume when 1.

---

## 8. Playback Semantics

### Event Instantiation

When an event is triggered:
1. Create an event instance
2. Instantiate the root container
3. Begin traversal

### Container Traversal

Traversal is recursive and depends on container type:

**Simple:**
- Acquire a voice
- Apply accumulated parameters
- Begin playback

**Random:**
- Run selection algorithm
- Instantiate and traverse selected child

**Switch:**
- Evaluate parameter
- Find matching case
- Instantiate and traverse matched child's container

**Blend:**
- Instantiate all children
- Traverse all children concurrently

### Voice Management

A "voice" represents a single audio playback instance. Interpreters should:
- Pool voices for efficiency
- Each Simple container at play time acquires one voice
- Apply all accumulated parameters (volume, pitch, lowpass, attenuation) to the voice
- Release voice when playback completes (or on stop)

### Parameter Inheritance

Parameters accumulate through the hierarchy:

- **Volume:** Sum all dB values down the tree
- **Pitch:** Sum all cent values down the tree
- **Lowpass:** Take maximum value in the tree
- **Attenuation:** Use nearest ancestor with `overrideParent: true`

---

## 9. Field Reference

### Bank Fields

| Field | Type | Required | Default |
|-------|------|----------|---------|
| `name` | string | Yes | - |
| `hideFlags` | int | Yes | 0 |
| `events` | string[] | Yes | - |

### Event Fields

| Field | Type | Required | Default |
|-------|------|----------|---------|
| `name` | string | Yes | - |
| `hideFlags` | int | Yes | 0 |
| `rootAudioObject` | object | Yes | - |

### Container Fields (All Types)

| Field | Type | Default | Containers |
|-------|------|---------|------------|
| `$type` | string | - | All (required) |
| `mute` | boolean | false | All |
| `volumeDB` | object | null (0) | All |
| `pitchCents` | object | null (0) | All |
| `lowpass` | object | null (0) | All |
| `attenuation` | object | null (false) | All |
| `playbackProbabilityPercent` | float | 100 | All |
| `modulators` | array | [] | All |

### Simple-Specific Fields

| Field | Type | Default |
|-------|------|---------|
| `soundClip` | string | - (required) |
| `loop` | boolean | false |

### Random-Specific Fields

| Field | Type | Default |
|-------|------|---------|
| `RandomPlaylist` | array | - (required) |
| `avoidLastPlayed` | int | 0 |
| `loop` | boolean | false |
| `transitionMode` | string | null |
| `triggerRate` | float | 1.0 |
| `crossfadeDuration` | float | 0.0 |

### Switch-Specific Fields

| Field | Type | Default |
|-------|------|---------|
| `ParameterName` | string | - (required) |
| `DefaultValue` | string | - (required) |
| `SwitchCases` | array | - (required) |

### Blend-Specific Fields

| Field | Type | Default |
|-------|------|---------|
| `BlendPlaylist` | array | - (required) |

### Randomizable Parameter Fields

| Field | Type | Default |
|-------|------|---------|
| `randomize` | boolean | false |
| `baseValue` | float | 0.0 |
| `minModifier` | float | 0.0 |
| `maxModifier` | float | 0.0 |

### Attenuation Fields

| Field | Type | Default |
|-------|------|---------|
| `overrideParent` | boolean | false |
| `value.active` | boolean | false |
| `value.minDistance` | float | 1.0 |
| `value.maxDistance` | float | 10.0 |
| `value.preset` | int | 0 |
| `value.curve` | object | null |
| `value.volumeAtMaxDistance` | float | 0.0 |
| `value.spreadAtMinDistance` | float | 0.0 |
| `value.spreadAtMaxDistance` | float | 0.0 |
| `value.lowPassAtMinDistance` | float | 0.0 |
| `value.lowPassAtMaxDistance` | float | 0.0 |

### Modulator Fields

| Field | Type | Default |
|-------|------|---------|
| `Target` | string | - (required) |
| `ParameterName` | string | - (required) |
| `Curve` | object | - (required) |

---

## Appendix A: Examples

### Minimal Event

```json
{
  "name": "UI_Click",
  "hideFlags": 0,
  "rootAudioObject": {
    "$type": "GISB.Runtime.GISB_SingleSound, Demute.GISB",
    "soundClip": "click.ogg",
    "loop": false
  }
}
```

### Random with Variation

```json
{
  "name": "Footstep",
  "hideFlags": 0,
  "rootAudioObject": {
    "$type": "GISB.Runtime.GISB_RandomSound, Demute.GISB",
    "avoidLastPlayed": 2,
    "RandomPlaylist": [
      {"$type": "GISB.Runtime.GISB_SingleSound, Demute.GISB", "soundClip": "step_01.ogg", "loop": false},
      {"$type": "GISB.Runtime.GISB_SingleSound, Demute.GISB", "soundClip": "step_02.ogg", "loop": false},
      {"$type": "GISB.Runtime.GISB_SingleSound, Demute.GISB", "soundClip": "step_03.ogg", "loop": false}
    ],
    "volumeDB": {
      "randomize": true,
      "baseValue": 0.0,
      "minModifier": -3.0,
      "maxModifier": 0.0
    },
    "pitchCents": {
      "randomize": true,
      "baseValue": 0.0,
      "minModifier": -100.0,
      "maxModifier": 100.0
    }
  }
}
```

### Switch with Nested Random

```json
{
  "name": "Footstep_Surface",
  "hideFlags": 0,
  "rootAudioObject": {
    "$type": "GISB.Runtime.GISB_SwitchSound, Demute.GISB",
    "ParameterName": "Surface",
    "DefaultValue": "Stone",
    "SwitchCases": [
      {
        "parameterValue": "Stone",
        "audioObject": {
          "$type": "GISB.Runtime.GISB_RandomSound, Demute.GISB",
          "avoidLastPlayed": 2,
          "RandomPlaylist": [
            {"$type": "GISB.Runtime.GISB_SingleSound, Demute.GISB", "soundClip": "stone_01.ogg", "loop": false},
            {"$type": "GISB.Runtime.GISB_SingleSound, Demute.GISB", "soundClip": "stone_02.ogg", "loop": false}
          ]
        }
      },
      {
        "parameterValue": "Wood",
        "audioObject": {
          "$type": "GISB.Runtime.GISB_RandomSound, Demute.GISB",
          "avoidLastPlayed": 2,
          "RandomPlaylist": [
            {"$type": "GISB.Runtime.GISB_SingleSound, Demute.GISB", "soundClip": "wood_01.ogg", "loop": false},
            {"$type": "GISB.Runtime.GISB_SingleSound, Demute.GISB", "soundClip": "wood_02.ogg", "loop": false}
          ]
        }
      }
    ]
  }
}
```

### Blend with Attenuation

```json
{
  "name": "Explosion",
  "hideFlags": 0,
  "rootAudioObject": {
    "$type": "GISB.Runtime.GISB_BlendSound, Demute.GISB",
    "BlendPlaylist": [
      {
        "$type": "GISB.Runtime.GISB_SingleSound, Demute.GISB",
        "soundClip": "explosion_core.ogg",
        "loop": false
      },
      {
        "$type": "GISB.Runtime.GISB_SingleSound, Demute.GISB",
        "soundClip": "explosion_debris.ogg",
        "loop": false,
        "volumeDB": {"randomize": false, "baseValue": -6.0, "minModifier": 0.0, "maxModifier": 0.0}
      }
    ],
    "attenuation": {
      "overrideParent": true,
      "value": {
        "active": true,
        "minDistance": 5.0,
        "maxDistance": 100.0,
        "preset": 1,
        "volumeAtMaxDistance": -40.0,
        "spreadAtMinDistance": 0.0,
        "spreadAtMaxDistance": 1.0,
        "lowPassAtMinDistance": 0.0,
        "lowPassAtMaxDistance": 0.5
      }
    }
  }
}
```
