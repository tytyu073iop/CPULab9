# CPU Lab 9 - OpenMP

C++17 project with OpenMP parallel programming examples.

## Prerequisites

- CMake 3.14+
- GCC-15 (Homebrew on macOS) - Apple's clang lacks OpenMP support

## Build

```bash
cmake -B build
cmake --build build
```

## Run

```bash
./build/src/main
```

## Add New Program

Add a `.cpp` file to `src/` — it automatically becomes an executable.

Example: `src/program2.cpp` → `./build/src/program2`

## CI

GitHub Actions workflow at `.github/workflows/compile.yml` verifies compilation on Ubuntu.

## License

MIT