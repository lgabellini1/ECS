# ECS - Entity Component System for Unreal Engine
This is a custom library built as part of a larger personal project. 
It is **not** meant to be a complete, ready-to-work ECS library; as such, there are much more indicated libraries to use (like EnTT, https://github.com/skypjack/entt). This was built both as a learning exercise on ECS software design in general (and its integration with a classic inheritance-driven architecture like Unreal's), an as a efficient, minimal alternative to the much more powerful but cumbersome <a href="https://dev.epicgames.com/documentation/unreal-engine/mass-entity-in-unreal-engine?lang=en" target="_blank">Unreal Engine MassEntity Plugin</a>.

## Installation
### Requirements
- Unreal Engine 5.7 at least
- A C++ project
### Manual Installation
1. Download or clone this repository.
2. Copy the plugin folder into your project's Plugins directory.
3. Open your project in Unreal Engine.
4. Navigate to Edit -> Plugins, then find ECS and enable it.
5. Restart the editor.

## Usage
Here's an example on how to write a game system which moves focus on the nearest interactable object:
```cpp
class FFocusSystem : public ECS::ISystem
{
public:
  virtual void Update(ECS::FRegistry& Registry, float DeltaTime) override
  {
    // matches every entity with InteractionFocus + Transform3D, running the supplied function on it
    ECS::ForEach<FInteractionFocus, FTransform3D>(Registry,
      [&](ECS::FEntity Player, FInteractionFocus& Focus, FTransform3D& PlayerTransform)
      {
        ECS::FEntity Closer;

        // use tags to match or exclude specific entities
        ECS::TExcludeSet<FInvalidInteractable> Exclude();

        ECS::ForEach<FInteractable, FTransform3D>(Registry, Exclude,
          [&](ECS::FEntity Entity, FInteractable&, FTransform3D& Transform)
        { // find nearest... });

        Focus.Target = Closer;
      });
  }
};
```
Then, user-defined systems can be registered for usage like this:
```cpp
void RegisterSystems(ECS::FSystemRegistrar& Registrar)
{
  Registrar.Add<FFocusSystem>();
}

// on initialization
FDelegateHandle ProviderHandle = ECS::RegisterSystemsProvider(
  ECS::FSystemsProvider::CreateStatic(&RegisterSystems)
);

// on shutdown
ECS::UnregisterSystemsProvider(ProviderHandle);
```
Actors can be recongnized as entities by adding a `UECSEntityComponent` component to the Actor blueprint.

## License
Mit License @ 2026 Luca Gabellini
