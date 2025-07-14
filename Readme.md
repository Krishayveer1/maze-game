# Maze Game (C++)

This is a C++-based terminal game project that simulates a simple maze-based game with AI logic. The original codebase provided a game engine, basic player and enemy classes, and game loop logic. I extended it by enhancing the AI components, introducing additional logic layers, and modularizing the code for clarity and reusability.

---

## 🧩 Project Structure

maze-game/
├── main.cpp # Entry point
├── Makefile # Compilation rules
├── manualinterface.h # Terminal input handling
├── game/
│ ├── game.cpp/.h # Core game logic and board management
│ ├── player.cpp/.h # Player movement and status
│ ├── enemy.cpp/.h # Enemy behavior
├── game-ai/
│ ├── brain.cpp/.h # AI decision-making logic
│ ├── brain1.cpp/.h # Additional AI logic layers


---

## 🧠 Original Project Provided

The starter code included:
- A functional game loop
- Core classes for `Player`, `Enemy`, and `Game`
- Basic AI logic via `brain.cpp`
- A `Makefile` for compilation

---

## 🛠️ My Contributions

I contributed the following:
- **Organized AI logic** into a dedicated `game-ai/` folder
- **Refactored and expanded AI logic** in `brain1.cpp` to handle more complex game states
- Modularized headers for maintainability (`brain1.h`)
- Cleaned and structured the overall codebase into clearly separated subfolders

---

## 🚀 Getting Started

### Prerequisites
- A C++ compiler (e.g., `g++`)
- Unix terminal or WSL (for Windows users)

### To Compile:
From the root folder:
```bash
make
To Run:
bash
Copy
Edit
./game.out
🧪 Features
Terminal-based user interface

Modular game engine architecture

Player and enemy logic

Expandable AI for smarter behaviors

📁 Folder Breakdown
Folder	Description
game/	Handles main game mechanics and actors
game-ai/	Handles AI behavior and logic modules

🙋 About Me
I’m a Computational Data Science major with an interest in AI logic, simulations, and systems programming. This project helped me explore how low-level decision systems can be embedded in game environments.

📜 License
This project is for academic and learning purposes. Attribution required for any reuse.
