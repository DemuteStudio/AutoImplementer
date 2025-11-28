---
aliases:
  - reflection system
---
The Unreal Engine reflection system is a core feature that enables the engine to inspect, modify, and interact with objects and their properties at runtime. This system provides powerful capabilities for various aspects of game development, including [[Serialization|serialization]], [[UPROPERTY()|property editing]], networking, and scripting with [[Blueprints]].

>**[[Serialization]]**
>The reflection system enables automatic serialization and deserialization of objects, making it easy to save and load game data, replicate data across the network, and handle undo/redo operations in the editor.

>**[[UPROPERTY()|Property Editing]]**
>The reflection system provides tools for creating custom property editors and exposing properties to the editor’s details panel.

>**[[Blueprints|Blueprint Integration]]**
>The reflection system allows C++ functions and properties to be exposed to Blueprints, Unreal Engine’s visual scripting language.
>By marking functions with *[[UFUNCTION()]]* and properties with *[[UPROPERTY()]]*, developers can make C++ functionality available to Blueprint scripts, enabling a seamless integration between code and visual scripting.

> **[[Garbage Collection]]**
> Unreal Engine uses garbage collection to manage memory and prevent memory leaks. The reflection system helps track object references and ensures that objects are deleted when they are no longer needed.

>**Networking**
>The reflection system plays a crucial role in Unreal's networking framework, enabling property replication and remote procedure calls (RPCs) between client and server.

>**Runtime Type Information (RTTI)**
> The reflection system provides runtime type information, allowing developers to query an object's class, its properties, and methods at runtime. This is useful for implementing dynamic behavior, debugging, and editor extensions.