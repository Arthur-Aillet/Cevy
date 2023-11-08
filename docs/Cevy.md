<h1 style="font-size:60px;">C++evy</h1>

# **`<<Component>>`**

Basic structure holding data, Ecs main building bloc

---
**Exemple Rust:**
```rust

#[derive(Component)]
struct Position {
	f64 x;
	f64 y;
}

```
**Exemple C++:**
```cpp

class Position : public Component {
	float x;
	float y;
};

```

# **`<<Entity>>`**

Group of Components

---
**Exemple Rust:**
```rust

commands.spawn((Camera, Position(3., 3.)));

```
**Exemple C++:**
```cpp

Enity camera = new Entity(new Camera, new Position(3, 3));

//Implementation:

class Entity {
    public:
    Entity(Component* ...) { // Vargs
    }
}

```
# **`<<System>>`**

Function accessing one or
multiple component group

----
**Exemple Rust:**
```rust

fn hello_world() {
    println!("hello world!");
}

.add_systems(Update, hello_world)

//Advanced querys
fn debug_player_hp(
    query: Query<
      (&Health, Option<&PlayerName>),
      (With<Player>, Without<Enemy>)
    >,
)

```
**Exemple C++:**
```cpp


```
*Features*

- Scheduler:
    - Custom set of steps
    - Custom ordering
- Conditional running
- Optional Querys
- Multithreaded
- Querys filters
    - With
    - Without
    - Or
- Extract Schedule as Depedencie graph

# **`<<App>>`**

Setup and manage Ecs,
Connect systems

---
**Exemple Rust:**
```rust

fn main() {
  app::new()
    .add_plugin(CameraPlugin)
    .add_system(Start, spawn_player)
    .init_ressource(clock::new())
    .run();
}

```
**Exemple C++:**
```cpp

int main() {
    // ...
}

```
# **`<<Resource>>`**
Global and Unique
data of some kind

---
**Exemple Rust:**
```rust

.init_ressource(clock::new())

fn greet_people(time: Res<Time>, query: Query<&Name, With<Person>>) {
    for name in &query {
        println!("hello {}!", name.0);
    }
}

```
**Exemple C++:**
```cpp


```
# **`<<Command>>`**

Interract with entity pool

---
**Exemple Rust:**
```rust

fn spawn_things( // System
    mut commands: Commands,
) {
    // manage resources
    commands.insert_resource(MyResource);
    commands.remove_resource::<MyResource>()
    commands.spawn(MyEntity);
    let id = commands.spawn(MyEntity);

    commands.entity(id)
      .insert(ComponentB::default())
      .remove::<ComponentA>();
}

```
**Exemple C++:**
```cpp


```
# **`<<Plugins>>`**

Connect systems without Ecs management

 Create an abstraction layer

---
**Exemple Rust:**
```rust

impl Plugin for HelloPlugin {
  fn build(&self, app: &mut App) {
    app.add_system(Start, spawn_camera)
      .add_system(Update, move_camera)
      .add_system(PostUpdate, destroy_camera)
      .add_event<CameraDestroyed>::new()
  }
}

```
**Exemple C++:**
```cpp


```
# **`<<EventWriter>>`**

Send a message holding components

---
**Exemple Rust:**
```rust

fn event_trigger(
    mut cam_event: EventWriter<MyEvent>)
{
    cam_event.send(
      MyEvent{
        message: "Behold! a message...";
    })
}

```
**Exemple C++:**
```cpp


```
# **`<<EventReader>>`**

System executed on ressources and given component groups when signal is sent

---
**Exemple Rust:**
```rust

fn event_listener(mut events: EventReader<MyEvent>) {
    for my_event in events.read() {
        info!("{}", my_event.message);
    }
}

```
**Exemple C++:**
```cpp


```
# **`<<State>>`**

Define state of app

---
**Exemple Rust:**
```rust

#[derive(..., States)]
enum AppState {
    #[default]
    Menu,
    InGame,
}

App::new()
	.add_state::<AppState>()
	.add_systems(OnExit(AppState::Menu), ...)
	.add_systems(OnEnter(AppState::InGame), ...)
	.add_systems(
		Update, (...).run_if(in_state(AppState::InGame)),
	)

fn menu(mut next_state: ResMut<NextState<AppState>>) {
	if ... {
		next_state.set(AppState::InGame);
	}
}

```
**Exemple C++:**
```cpp


```

# **`<<GameEngine>>`**

App superset creating the default ECS App and adding
plugins to control bases, holding utilities for all games

---
**Ressources:**

 - Asset Manager
 - Global Timer
 - Font Manager

**Default Stages:**

*At Start:*

 - PreStartup
 - Startup
 - PostStartup

*Each time possible:*

 - First
 - PreUpdate
 - StateTransition `// Run between each states transition`
 - RunFixedUpdateLoop `//This will run FixedUpdate zero to many times,
		based on how much time has elapsed.`
 - Update `// Default Stage when adding system`
 - PostUpdate
 - Last
