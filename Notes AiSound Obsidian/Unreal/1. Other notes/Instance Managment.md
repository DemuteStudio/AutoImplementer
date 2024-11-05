# Creating New Instances
### CreateNewObject()

### SpawnActor()

## CDO
Class Default Objects are templates from which every instance will be created. Every newly created instance is a clone of the CDO.

Unreal C++ constructors are not called when creating a new instance but are called to initialize this CDO.
The constructor is thus used to 
- Set initial property values
- Add contained items (sub objects)
These will apply to all the instances you make of that class
### BeginPlay()
This method is called after the creation of each instance and can thus be used to setup stuff unique to each instance, unlike unreal constructors.
# Deleting Instances
Unreal automatically recognizes when objects are no longer referenced and automatically mark them for [[Garbage Collection]].

You can use [[UPROPERTY()]] to avoid automatic garbage collection.
### EndPlay()
This method is called when the object is added to [[Garbage Collection]]
### BeginDestroy()
This is called before the object is destroyed, allowing you to run specific cleanup code.
However, cleanup is called whenever unreal feels like doing [[Garbage Collection]], not right after the object is no longer referenced.