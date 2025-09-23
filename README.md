# OpenGL Example

Scaffolded using Google Gemini, but extensively hacked afterwards.

Loads an STL file and renders it using a simple Phong shader.

Needs cleanup and simplification.

## Get libraries


#!/bin/bash

```bash
cd $HOME
brew install vcpkg
git clone https://github.com/microsoft/vcpkg "$HOME/vcpkg"
export VCPKG_ROOT="$HOME/vcpkg"
```

## Build

```bash
cmake -DCMAKE_TOOLCHAIN_FILE=/Users/bfowler/vcpkg/scripts/buildsystems/vcpkg.cmake --fresh .
```
