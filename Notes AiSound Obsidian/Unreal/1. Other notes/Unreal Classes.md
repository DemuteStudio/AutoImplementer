# Core Classes
### [[UOBJECT()]]
![[UOBJECT()]]

### AActor
Represent things that can be placed in a game world/has a location.
All classes that derive from AActor or any of it's descendants must start with an uppercase A.

# Common Classes
### AGameModeBase
Represents rules for a game mode, such as number of players, spawn locations, and many default classes such as hud, pawn and playercontroller classes.
DON'T DO IT IN C++, DO IT IN BLUEPRINTS.
### APawn
Represents something in the world that can be controlled, either by the player or by Ai.
### ACharacter
Derived from APawn and represents a bipedal humanoid character.
### AHUD
isn't necessary to display the hud, but is a very good way of grouping several widgets together and managing their display / updating their logic.
### APlayerController
Controllers control APAwns, a Playercontroller controls the player's pawn. It usually listens for inputs and transforms those inputs into game actions.
### UGameInstanceSubsystem
Allows the developer to create [[Singleton]]s that manage a game wide subsystem (instead of having all of these systems in a single class: [[Unreal Classes#UGameInstance (Obsolete)|UGameInstance]]) such as saving the game, or achievements. These are automatically detected and ran by Unreal on game launch. 
# Uncommon Classes
### AGameStateBase
Used to update multiple clients to the game's state, only useful in multiplayer scenarios.
### APlayerState
Used to update multiple clients to a player's state, only useful in multiplayer scenarios.
### ASpectatorClass
Used to represent a spectator, a useually dead player that can fly around the map waiting to respawn.
### UGameInstance (Obsolete)
A [[Singleton]] that represents the whole game, from launching it to closing it, menus included.