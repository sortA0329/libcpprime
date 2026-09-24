# Contributing to libcpprime

Thanks for your interest in improving `libcpprime`.

## Development principles

- Keep the public API minimal and stable.
- Keep `include/libcpprime/` compatible with C++11 and older compiler versions in the supported matrix.
- Avoid compiler-specific behavior unless guarded and tested across compilers.
- If a change is breaking, call it out clearly in the PR description.

## Required tools

- A system C++ compiler toolchain
- mise
- Git
- Zed (recommended editor)

### Why Windows development is recommended

Windows development is recommended when contributing significant changes, because this repository validates both `msvc` and `clang-cl` in CI and those toolchains are easiest to reproduce locally on Windows.

However, since it is difficult to generate a proper Compilation database with MSVC and clang-cl, if you are developing on Windows, configuring with MinGW GCC or Clang is also required.

Linux/macOS development is still fine for day-to-day work, especially for GCC/Clang checks.

## Quick start

### 1) Clone

```bash
git clone https://github.com/<your-account>/libcpprime.git
cd libcpprime
```

### 2) Install dependencies

- Install a system C++ compiler toolchain, mise, and Git.

### 3) Open in Zed

```bash
zed .
```

### 4) Run tests

```bash
mise run test:gcc
mise run test:clang
mise run test:msvc
mise run test:clang-cl
```

Typical runtime for tests is around 10 seconds per run.

## Command reference

All project commands are defined in `mise.toml`.
List them with `mise tasks ls`.

### Configure

- `mise run configure:gcc|clang|clang-cl|msvc`: Configure a build directory
- `mise run configure`: Run all configure tasks above

### Tests

- `mise run test:gcc|clang|clang-cl|msvc`: Run tests
- `mise run test`: Run all test tasks above

### Benchmarks

- `mise run bench:gcc|clang|clang-cl|msvc`: Run standard benchmark and generate plots
- `mise run bench-heavy:gcc|clang|clang-cl|msvc`: Run heavier benchmark variant (`--heavy`)

### Docs

- `mise run docs`: Copy `README.md` to `docs/index.md`, then build MkDocs site
- `mise run serve`: Serve MkDocs locally

### Clean

- `mise run clean:gcc|clang|clang-cl|msvc`: Remove one build directory
- `mise run clean`: Remove all build directories

### Lint and format

- `mise run check`: Check for lint and formatting issues
- `mise run fix`: Auto-fix lint and formatting issues
