# 🏰 Tower of Hanoi Solver
**Recursive + Iterative Algorithm (C Language)**

This project is a complete Tower of Hanoi solver featuring a **recursive version**, an **iterative stack-based version**, a **batch performance tester**, and a **colored interactive console interface**.

---

## ✨ Features

### 🔹 Recursive Algorithm
- Classic divide-and-conquer method  
- Shows theoretical steps and complexity  
- Supports custom number of disks  
- Includes built-in performance calibration  
- Standard tests (5, 10, 15, 20 disks)

### 🔹 Iterative Algorithm
- Stack-based version (no recursion)  
- Faster and avoids recursion depth limits  
- Fully simulates legal moves  
- Supports large disk counts  
- Visual progress bar for long runs  

### 🔹 Batch Performance Testing
Runs tests from **1 → N** disks and displays a table:

- Total moves  
- Execution time  
- Moves per second  

### 🔹 Visualizer (Windows Only)
Launch a graphical Tower of Hanoi visualizer:

dist/run.bat



---

## 🛠 Tech Stack

| Component | Technology |
|----------|------------|
| Language | C |
| UI       | ANSI-colored terminal |
| Algorithms | Recursive + Iterative (Stacks) |
| Visualizer | react |

---

## 📁 Project Structure

/Tower-of-Hanoi/
│
├── projet_algo.c # Main C source code
├── dist/
│ └── run.bat # Visualizer launcher (Windows)
│
└── README.md


---

## 🚀 Compile & Run

### Compile
```bash
gcc projet_algo.c -o hanoi
```
### Run
```bash

./hanoi
