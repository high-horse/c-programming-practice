# 🟢 Level 1: Basics – Get Comfortable with C Syntax
## 1. Hello World CLI
- 🧠 Learn: main(), printf(), compilation (gcc -std=c2x)
- ✅ Use: #include <stdio.h>

## 2. Temperature Converter (Celsius ↔ Fahrenheit)
- 🧠 Learn: User input, functions, arithmetic, control flow
- ✅ Use: scanf(), printf(), function definitions

## 3. Simple Calculator CLI
- 🧠 Learn: Switch statements, loops, error handling
- ✅ Add: [[nodiscard]] attribute to calculation functions

# 🟡 Level 2: Memory & Strings – Deeper Concepts
## 4. String Reverser
- 🧠 Learn: Arrays, pointers, string manipulation
- Use: char[], char *, strlen(), pointer arithmetic

## 5. Manual Memory Allocator
- 🧠 Learn: malloc, free, memory safety
- ✅ Use: nullptr (C23), and bounds-checking functions like memcpy_s()

## 6. Word Counter (like wc)
- 🧠 Learn: File I/O, loops, character classification
- ✅ Use: fopen(), fgetc(), isalpha() from <ctype.h>

# 🟠 Level 3: Structs, Data Structures, and Files
## 7. Contact Book (Save/Load from File)
- 🧠 Learn: struct, file operations, binary vs text files
- ✅ Use: fwrite(), fread(), fscanf(), fprintf()

## 8. Linked List Implementation
- 🧠 Learn: Dynamic memory, pointers to structs, traversal
- ✅ Use: typeof (C23) to create type-safe macros

## 9. Simple JSON Parser (Subset)
- 🧠 Learn: String parsing, recursion, structs
- ✅ Try using C23 Unicode strings (char8_t) if you're parsing UTF-8

# 🔵 Level 4: System-Like Projects
## 10. Tiny Shell / Command REPL
- 🧠 Learn: fork(), exec(), system(), parsing input
- ✅ Use: [[nodiscard]] to warn if command functions return errors

## 11. HTTP Request CLI Tool
- 🧠 Learn: TCP sockets, text parsing, networking
- ✅ Use: getaddrinfo(), send(), recv()

## 12. Mini Make Tool
- 🧠 Learn: File timestamps, parsing a config, executing shell commands
- ✅ Reinforces: File I/O, string tokenization, system calls

# 🟣 Level 5: Capstone Ideas
## 13. Markdown to HTML Converter
- 🧠 Learn: Parsing, tokenization, string manipulation
- ✅ C23 Use: typeof, nullptr, safe functions

## 14. Memory Viewer / Hexdump Tool
- 🧠 Learn: Pointer arithmetic, byte-level access
- ✅ Reinforces low-level memory manipulation

## 15. Static Site Generator
- 🧠 Learn: File input/output, string templates, command-line options
- ✅ Great crossover project with your web background

# 🛠️ C Development Tools & Learning Guide

## ⚒️ Tools You'll Need Along the Way

| Tool                       | Purpose                          |
|---------------------------|----------------------------------|
| `gcc -std=c2x`            | Compile C23 code                 |
| `valgrind`               | Check for memory leaks          |
| `gdb`                    | Debugging                        |
| `make`                   | Automate compilation             |
| VS Code + C/C++ extension | Development environment         |

---

## 🌱 How to Learn While Building

| Tip                                | Why It Helps                                   |
|------------------------------------|------------------------------------------------|
| Build one small project per week   | Avoid overwhelm; reinforce each concept        |
| Read man pages (`man fopen`)       | Learn the real API, not just tutorials         |
| Practice with `gdb` early          | Debugging will teach you everything            |
| Use modern C23 features gradually  | Avoid overloading at the start                 |

---

## 📚 Optional: C23 Features to Try Out As You Go

| Feature        | Where to Use It                                       |
|----------------|-------------------------------------------------------|
| `nullptr`      | Replace `NULL` in pointer code                        |
| `[[deprecated]]` | Mark old functions                                  |
| `typeof`       | Clean up macro-heavy code                             |
| `memcpy_s`     | Replace unsafe `memcpy`                               |
| `char8_t`      | Unicode experiments (e.g., JSON, Markdown)            |
| `#elifdef`     | Smarter conditional compilation                       |
