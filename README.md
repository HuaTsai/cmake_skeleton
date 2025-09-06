# CMake Skeleton

Commands

```bash
cmake -B build -G Ninja -DENABLE_CLANG_TIDY=ON
cmake --build build -j$(nproc)
cmake --build build --target format
build/main
ctest --test-dir build --output-on-failure
sudo cmake --build build --target install  # install in /usr/local
sudo cmake --build build --target uninstall
cd build && cpack
sudo dpkg -i myproject-1.0.0-x86_64.deb  # install in /usr
```
