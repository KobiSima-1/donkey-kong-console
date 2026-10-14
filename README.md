# Donkey Kong – Console Game (C++)

A text-mode remake of the classic Donkey Kong arcade game, written in C++ for the Windows console.
Mario has to climb ladders and dodge barrels and ghosts to reach Pauline at the top of each level.
Built as a team project of two for an object-oriented programming course at The Academic College of Tel Aviv-Yafo.

## Gameplay

- **Three levels** loaded from text files (`dkong_01.screen` – `dkong_03.screen`). Any `.screen` file placed in the game folder is picked up automatically, and you can play all levels in a row or choose one.
- **Barrels** are thrown by Donkey Kong in a random direction, roll down sloped floors, fall between platforms and explode if they fall too far.
- **Ghosts** patrol the floors; **special ghosts** can also climb ladders.
- **Hammer** power-up: pick it up to smash barrels and ghosts for extra points.
- **Lives, score and a legend** are shown on screen, with animations when Mario loses a life, swings the hammer or destroys an enemy.

## Controls

| Key | Action |
| --- | --- |
| `A` / `D` | Move left / right |
| `W` | Jump, or climb up a ladder |
| `X` | Climb down a ladder |
| `S` | Stay in place |
| `P` | Use the hammer |
| `Esc` | Pause / resume |

## Record, replay and automated testing

The game can record a session and play it back deterministically:

| Command | What it does |
| --- | --- |
| `ex3.exe` | Normal game |
| `ex3.exe -save` | Plays normally and records every key press (`.steps`) and every game event (`.result`), together with the random seed |
| `ex3.exe -load` | Replays a recorded session from the files |
| `ex3.exe -load -silent` | Replays without drawing anything and checks that the events match the `.result` file – a quick regression test for the game logic |

Because the random seed is saved with the steps, barrels and ghosts behave exactly the same in the replay as in the original game.

## Design

- `Entity` is an abstract base class for everything that moves; `Mario`, `Barrel` and `Ghost` derive from it, and `SpecialGhost` extends `Ghost` with ladder climbing.
- `Game` is an abstract base for the game loop, with two implementations: `HumanInputGame` (keyboard) and `FileInputGame` (replay from file).
- `Board` loads and validates level files. A level with small mistakes is fixed rather than rejected: missing characters at the end of a row are filled in, and objects that are not standing on a floor are moved to a valid random position.
- `Steps` and `Results` handle saving and loading recorded sessions.
- Enemies are stored as `std::unique_ptr` in `std::vector`s and created and destroyed during play.

## Level file format

Each level is an 80 × 25 text grid:

| Char | Meaning | Char | Meaning |
| --- | --- | --- | --- |
| `@` | Mario (start) | `=` | Floor |
| `$` | Pauline (goal) | `<` / `>` | Floor sloping left / right |
| `&` | Donkey Kong | `H` | Ladder |
| `x` | Ghost | `p` | Hammer |
| `L` | Legend (score and lives) | `Q` | Wall |

## Running

Windows only (uses the Windows console API).

1. Open `ex3.sln` in Visual Studio 2022.
2. Build and run (`Ctrl+F5`). The `.screen` files must be in the working directory.
