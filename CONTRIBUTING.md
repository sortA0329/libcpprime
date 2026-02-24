# Contributing to libcpprime

Thanks for your interest in improving `libcpprime`.

## Development principles

- Keep the public API minimal and stable.
- Keep `include/libcpprime/` compatible with C++11 and older compiler versions in the supported matrix.
- Avoid compiler-specific behavior unless guarded and tested across compilers.
- If a change is breaking, call it out clearly in the PR description.

## Required tools

### Build and test

- CMake >= 3.20
- A C++ compiler toolchain (GCC/Clang/MSVC/clang-cl)
- Ninja
- Task (`go-task`)
- Git

### Python tooling (benchmarks/docs)

- Python >= 3.12
- `uv`

Python dependencies are managed by `uv`.

### Why Windows development is recommended

Windows development is recommended when contributing significant changes, because this repository validates both `msvc` and `clang-cl` in CI and those toolchains are easiest to reproduce locally on Windows.

However, since it is difficult to generate a proper Compilation database with MSVC and clang-cl, if you are developing on Windows, configuring with MinGW GCC or Clang is also required.

Linux/macOS development is still fine for day-to-day work, especially for GCC/Clang checks.

### Dev Container support

This repository includes a Dev Container at `.devcontainer/`.
Use it if you want a reproducible Linux environment without installing tools manually.

## Quick start

### 1) Clone

```bash
git clone https://github.com/<your-account>/libcpprime.git
cd libcpprime
```

### 2) Install dependencies

- Install CMake, compiler(s), and Task.
- For docs/benchmark plots, install Python 3.12+ and `uv`.

### 3) Run tests

```bash
task test:gcc
task test:clang
task test:msvc
task test:clang-cl
```

Typical runtime for tests is around 20-60 seconds per run.

## Command reference

All project commands are defined in `Taskfile.yml`.

### Configure

- `task configure:gcc|clang|clang-cl|msvc`: Configure a build directory
- `task configure`: Run all configure tasks above

### Tests

- `task test:gcc|clang|clang-cl|msvc`: Run tests
- `task test`: Run all test tasks above

### Benchmarks

- `task bench:gcc|clang|clang-cl|msvc`: Run standard benchmark and generate plots
- `task bench-heavy:gcc|clang|clang-cl|msvc`: Run heavier benchmark variant (`--heavy`)

### Docs

- `task docs`: Copy `README.md` to `docs/index.md`, then build MkDocs site
- `task serve`: Serve MkDocs locally

### Clean

- `task clean:gcc|clang|clang-cl|msvc`: Remove one build directory
- `task clean`: Remove all build directories
