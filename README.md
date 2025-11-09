# Custom Shel lin C++ (Capstone Project)

### 👨‍💻 Author: Abhishek Mohapatra  
**Registration Number:** 2241013303  
**Batch:** 11  
**Course:** Capstone Project (Custom Shell Implementation in C++)  

---

## 📘 Project Overview

This project is a **Custom Command Shell** implemented in **C++** that mimics a simplified version of the Linux terminal.  
It supports **basic commands, background execution, redirection, piping,** and **job control** (`fg` / `bg` commands).

The goal of this project is to understand **process management**, **I/O redirection**, **piping**, and **job scheduling** in operating systems.

---

##  **Key Features**

| Day | Topic | Description |
|-----|--------|-------------|
| **Day 1** | Basic Shell Loop | Created the main shell loop with input prompt and exit functionality |
| **Day 2** | Command Execution | Implemented execution of system commands using `system()` and `_spawnvp` |
| **Day 3** | Background Execution | Added `&` for running jobs in the background |
| **Day 4** | Piping & Redirection | Added support for input/output redirection (`<`, `>`) and simple pipes (`|`) |
| **Day 5** | Foreground & Background Control | Implemented `fg` and `bg` job control commands with process management |

---

## 🛠️ **Technologies Used**

- **Language:** C++17  
- **Compiler:** g++ (MinGW / GCC)  
- **Platform:** Windows Command Prompt  
- **Headers:** `<iostream>`, `<vector>`, `<string>`, `<process.h>`, `<windows.h>`  

---

## ⚙️ **How to Compile and Run**

### ▶️ **Option 1: Using Makefile**
```bash
make
./myshell
