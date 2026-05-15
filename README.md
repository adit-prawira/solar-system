# Solar System Simulation

This is a solar system simulation written in C++ OpenGL.

## Prerequisites

- Download GLFW here -> https://www.glfw.org/download.html
- Extract zip file for GLFW
- Copy all content from the lib folder into the current project's lib folder (select lib based on your machine's OS architecture)
- Copy all content from the include folder into the current project's lib folder
- Download GLAD here -> https://glad.dav1d.de/
- Extract zip GLAD zip file
- Copy content in include folder into the current project's include folder
- Copy content in src folder into the current project's src folder

## Running

```bash
make clean && make && ./a.out
```

Or with shader setup handled automatically:

```bash
make test
```

## C++ Setup for VS Code

- Create `c_cpp_properties.json` file and copy content from `c_cpp_properties.example.json` into it, and adjust based on the OS you are using

## Neovim / LSP Setup (clangd)

Generate a `compile_commands.json` so clangd can resolve all include paths:

```bash
brew install compiledb              # or: pip install compiledb
compiledb make                      # parses Makefile, writes compile_commands.json
```

Re-run `compiledb make` whenever the Makefile or source files change significantly. The generated `compile_commands.json` should not be tracked in git — it is listed in `.gitignore`.
