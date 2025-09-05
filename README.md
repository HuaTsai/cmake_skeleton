# CMake Skeleton

Commands

```bash
cmake -B build -G Ninja -DENABLE_CLANG_TIDY=ON
cmake --build build -j$(nproc)
cmake --build build --target format
build/main
ctest --test-dir build --output-on-failure
```
