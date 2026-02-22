# 🧭 C23 Learning Roadmap for Web Developers

Welcome! This roadmap helps you transition from web development to systems-level programming using the latest C23 standard. Each week introduces a new core concept, followed by a hands-on project that reinforces what you’ve learned.

---

## 🗓️ Timeline: 12 Weeks to C Mastery (C23 Edition)

---

### ✅ Week 1: Getting Started with C
- **Goals**:
  - Set up GCC or Clang with `-std=c2x`
  - Understand basic syntax, types, `main()`, `printf()`
- **Project**: `hello_world.c`
- **C23 Feature**: Use `bool` without `<stdbool.h>`
- **Resources**: [learn-c.org](https://www.learn-c.org/)

---

### ✅ Week 2: Variables, Input/Output, and Functions
- **Goals**:
  - Use `scanf`, create and call functions
  - Understand return types and parameters
- **Project**: Temperature Converter CLI
- **C23 Feature**: Add `[[nodiscard]]` to warn if return value ignored

---

### ✅ Week 3: Control Flow and Basic Error Handling
- **Goals**:
  - Use `if`, `switch`, `while`, `for`
  - Implement basic error checking
- **Project**: Simple Calculator CLI
- **C23 Feature**: Use `static_assert` without a message

---

### ✅ Week 4: Arrays and Strings
- **Goals**:
  - Understand character arrays and null-terminated strings
  - Learn about manual string handling
- **Project**: String Reverser
- **C23 Feature**: Try using `char8_t` and UTF-8 string literals

---

### ✅ Week 5: Pointers and Memory Management
- **Goals**:
  - Understand pointers, `&`, `*`, and `malloc`/`free`
  - Prevent memory leaks and segmentation faults
- **Project**: Manual Memory Allocator (allocate, use, free)
- **C23 Feature**: Use `nullptr` instead of `NULL`

---

### ✅ Week 6: File I/O and Text Processing
- **Goals**:
  - Read/write files with `fopen`, `fgetc`, `fputs`, etc.
  - Handle file errors gracefully
- **Project**: Word Counter (like `wc`)
- **C23 Feature**: Use safer functions like `fopen_s`

---

### ✅ Week 7: Structs and Basic Data Structures
- **Goals**:
  - Learn `struct` syntax and use
  - Pass structs to functions by pointer
- **Project**: Contact Book (Save/Load to file)
- **C23 Feature**: Use `typeof` in macros

---

### ✅ Week 8: Dynamic Data Structures
- **Goals**:
  - Implement linked lists with dynamic allocation
  - Practice memory cleanup
- **Project**: Singly Linked List CLI
- **C23 Feature**: Use `nullptr` and `typeof`

---

### ✅ Week 9: Parsing and Recursion
- **Goals**:
  - Learn basic string parsing
  - Write recursive functions
- **Project**: Mini JSON-like Parser
- **C23 Feature**: Use `char8_t` for UTF-8 input

---

### ✅ Week 10: System Programming Basics
- **Goals**:
  - Execute system commands with `system()`
  - Explore environment variables
- **Project**: Mini Shell / Command REPL
- **C23 Feature**: Add `[[deprecated]]` tags to old commands

---

### ✅ Week 11: Networking and Sockets
- **Goals**:
  - Connect to a server via sockets (`getaddrinfo`, `recv`, `send`)
- **Project**: HTTP GET Request CLI
- **C23 Feature**: Use `[[nodiscard]]` to enforce response checks

---

### ✅ Week 12: Capstone Project
- **Goals**:
  - Integrate everything learned: memory, structs, files, parsing
- **Project Options**:
  - Static Site Generator (parses `.md`, generates `.html`)
  - Markdown to HTML converter
  - Hex Viewer / Memory Inspector
- **C23 Feature**: Use multiple — `typeof`, `nullptr`, attributes, etc.

---

## 🧰 Tools

| Tool        | Purpose                |
|-------------|------------------------|
| GCC / Clang | C23 compilation (`-std=c2x`) |
| `valgrind`  | Memory debugging       |
| `gdb`       | Runtime debugging      |
| VS Code     | IDE + debugger support |
| `make`      | Build automation       |

---

## 📌 Notes

- Compile with:
  ```sh
  gcc -std=c2x -Wall -Wextra -o program program.c
  ```


- Run with:
  ```sh
    ./program
  ```
