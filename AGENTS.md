# C++ OpenMP CMake Project

## Build Commands
```bash
cmake -B build
cmake --build build
./build/src/main
```

## Key Details
- Each `.cpp` file in `src/` automatically becomes an executable
- Uses GCC-15 from Homebrew (required for OpenMP on macOS)
- C++17 standard

## Compiler Setup
- macOS: GCC-15 (`/opt/homebrew/opt/gcc/bin/g++-15`) required - Apple's clang lacks OpenMP
- Linux: Standard `find_package(OpenMP)` works

## VS Code
- Install CMake Tools extension
- Uses GCC-15 paths from `.vscode/settings.json`

## CI
- GitHub Action at `.github/workflows/compile.yml`
- Runs on Ubuntu with `libomp-dev`