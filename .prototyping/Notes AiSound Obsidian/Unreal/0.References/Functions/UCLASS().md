This macro tells Unreal Engine that the associated class needs to be managed by the [[Reflection System]], allowing it to interact with the engine.

This is not needed for utility or data-only classes that don't require the benefits of the reflection system ([[Serialization]] or [[Blueprints]]), or performance-critical code where avoiding the overhead is important (such as physic calculations).