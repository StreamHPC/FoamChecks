# Clang-tidy checks for OpenFOAM

This repository stores clang-tidy plugin(s) for OpenFOAM.
Currently one such check is present for migrating the uses of `min`/`max` to `Foam::min`/`Foam::max`
to be compatible with `__clang_hip_math.h` built-in OpenMP header.

## Building

Requires llvm and clang 15 development files (available from apt as `llvm-15-dev` and `libclang-15-dev`).
Use the cmake build system.

# Using

To load the plugin and enable only these checks add the `-load <path-to>/libfoamchecks.so -checks=-*,foam-*`
arguements to `clang-tidy` or `run-clang-tidy`.
