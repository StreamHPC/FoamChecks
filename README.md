# Clang-tidy checks for OpenFOAM

This repository stores clang-tidy plugin(s) for OpenFOAM.
Currently one such check is present for migrating the uses of `min`/`max` to `Foam::min`/`Foam::max`
to be compatible with the `__clang_hip_math.h` built-in OpenMP header.

## Building

Requires llvm and clang 15 development files (available from apt as `llvm-15-dev` and `libclang-15-dev`).
Use the cmake build system.

# Using

To load the plugin and enable only these checks add the `-load <path-to>/libfoamchecks.so -checks=-*,foam-*`
arguements to `clang-tidy` or `run-clang-tidy`.

## Steps for fixing min/max issues in OpenFOAM:
1. Do clean build of OpenFOAM and capture the compilation command lines used by using
   [bear](https://github.com/rizsotto/Bear) `bear ./Allwmake -j -q`
2. Collect the clang-tidy fixes with the plugin, also include header files.
   `run-clang-tidy -export-fixes fixes.yaml -load <path-to>/libfoamchecks.so -checks='-*,foam-*' -header-filter='*'`
3. Apply the changes
   `clang-apply-replacements .`
4. Clang-tidy unfortunately gets confused with how OpenFOAM includes multiple headers with the same
   name, as such the fix might get applied multiple times to header files. To fix delete the multiply
   added namespaces such as `Foam::Foam::min` in headers with the command:
   `git diff --name-only -- '*.H' | xargs sed -Ei 's/(Foam::){2,}/Foam::/g'`
5. Some headers might have corrupted more due to the duplicated names, check the diff for headers 
   and fix by hand. Due to the same reason headers could be missed (the fixes for them get applied
   in the wrong location), so the tool doesn't guarantee that all issues are fixed, but should find
   the majority of them.
