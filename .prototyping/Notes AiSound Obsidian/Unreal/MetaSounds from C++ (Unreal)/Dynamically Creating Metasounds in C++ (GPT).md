#### 1. **Include Necessary Headers**

Include the necessary MetaSound-related headers in your C++ class. These headers provide the functionality to create and manipulate MetaSound graphs.

```cpp
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "MetasoundAssetSubsystem.h"
#include "MetasoundFrontendDocument.h"
#include "MetasoundSource.h"
#include "MyDynamicMetaSoundActor.generated.h"
```
#### 2. **Create a C++ Class**

Create a new C++ class that derives from `AActor` (or any other appropriate class). This class will contain the logic to create and manage a MetaSound component dynamically.
#### 3. **Define Your Class Members**

Define the necessary members in your class header file `.h` This will include an audio component to play the MetaSound and a handle to the MetaSound.

```cpp
UCLASS()
class MYPROJECT_API AMyDynamicMetaSoundActor : public AActor
{
    GENERATED_BODY()

public:
    AMyDynamicMetaSoundActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
    UAudioComponent* AudioComponent;

    UPROPERTY()
    UMetaSoundSource* MetaSoundSource;
};

```
###### Code Dissection
![[UCLASS()]]

![[GENERATED_BODY()]]

![[UPROPERTY()]]
#### 4. **Implement the Constructor and BeginPlay**

Initialize the audio component and create the MetaSound source dynamically in your class constructor and `BeginPlay` method.
```cpp
#include "MyDynamicMetaSoundActor.h"
#include "Components/AudioComponent.h"
#include "MetasoundSource.h"
#include "MetasoundFrontend.h"
#include "MetasoundEditor.h" // Required for working with MetaSound graph and editor

AMyDynamicMetaSoundActor::AMyDynamicMetaSoundActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create an audio component and attach it to the root
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    RootComponent = AudioComponent;

    // Create a new MetaSound source
    MetaSoundSource = NewObject<UMetaSoundSource>(this, UMetaSoundSource::StaticClass());
}

void AMyDynamicMetaSoundActor::BeginPlay()
{
    Super::BeginPlay();

    if (MetaSoundSource)
    {
        // Create a new MetaSound document
        FMetasoundFrontendDocument MetaSoundDocument;

        // Define the graph of the MetaSound
        FMetasoundFrontendGraph Graph;

        // Example: Add an oscillator node to the MetaSound graph
        FMetasoundFrontendNode OscillatorNode;
        OscillatorNode.Name = "Oscillator";
        OscillatorNode.ClassName = FMetasoundFrontendClassName("SineWaveOscillator", "Audio");
        Graph.Nodes.Add(OscillatorNode);

        // Example: Add an output node to the MetaSound graph
        FMetasoundFrontendNode OutputNode;
        OutputNode.Name = "Output";
        OutputNode.ClassName = FMetasoundFrontendClassName("Output", "Audio");
        Graph.Nodes.Add(OutputNode);

        // Example: Connect oscillator node to output node
        FMetasoundFrontendEdge Edge;
        Edge.From.NodeID = OscillatorNode.ID;
        Edge.From.OutputName = "Audio";
        Edge.To.NodeID = OutputNode.ID;
        Edge.To.InputName = "Audio";
        Graph.Edges.Add(Edge);

        // Set the graph to the MetaSound document
        MetaSoundDocument.RootGraph = Graph;

        // Set the document to the MetaSound source
        MetaSoundSource->SetDocument(MetaSoundDocument);

        // Assign the MetaSound source to the audio component
        AudioComponent->SetSound(MetaSoundSource);

        // Start playing the MetaSound
        AudioComponent->Play();
    }
}

void AMyDynamicMetaSoundActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Optional: Logic to manipulate MetaSound in real-time
}

```

## ORGANIZE THIS
### Explanation of the Code

1. **Header Inclusions**: The necessary headers are included for creating and manipulating audio components and MetaSounds.
    
2. **Class Members**: `AudioComponent` is used to play audio, and `MetaSoundSource` holds the MetaSound data.
    
3. **Constructor Initialization**: The audio component and MetaSound source are initialized in the constructor.
    
4. **MetaSound Creation**:
    
    - A new `FMetasoundFrontendDocument` is created to define the MetaSound structure.
    - Nodes are added to the MetaSound graph, such as an oscillator and an output node. This example shows a basic sine wave oscillator connected to an audio output.
    - Connections (edges) are established between nodes to define the flow of audio data.
5. **Assigning and Playing MetaSound**: The MetaSound source is set to the audio component, and playback is initiated with `AudioComponent->Play()`.
    

### Summary

- **Dynamic MetaSound Creation**: This approach demonstrates creating MetaSound assets dynamically at runtime using C++.
- **Real-time Audio Manipulation**: The ability to create and manipulate MetaSound graphs in C++ provides flexibility for dynamic and procedural audio design.
- **Integration with Unreal Components**: Audio playback is integrated using `UAudioComponent` to manage sound playback.

**a.** Do you want to see how to handle more complex MetaSound graphs with multiple nodes and connections?

**b.** Are you interested in dynamically changing parameters of the MetaSound at runtime to create interactive audio effects?