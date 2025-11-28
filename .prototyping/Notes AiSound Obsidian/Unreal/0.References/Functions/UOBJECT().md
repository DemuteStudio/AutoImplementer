`UObject` is the base class for all classes that participate in Unreal Engine's [[Reflection System|reflection]] and object management systems.
All classes that derive from UObject or any of it's descendants (AActor excluded) must start with an uppercase U.

Note : *[[UCLASS()]]*, *[[USTRUCT()]]* and *[[UENUM()]]* do not derive from this class as they are Runtime Macros and not classes, `AActor` is a class and does derive from `UOBJECT`.