Properties marked with `UPROPERTY()` can be exposed to the Unreal Editor thanks to the [[Reflection System]], allowing game designers and developers to modify object properties through the editor UI.

Also stops the [[Garbage Collection]] from eating up whatever a property is pointing to (the system does not recognize non UPROPERTY pointers and believes it can clean up whatever is at it's end).

Requires [[GENERATED_BODY()]] and [[UCLASS()]].