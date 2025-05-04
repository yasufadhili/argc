# Setting up LLVM

## Environment
- Ubuntu 22.04 on x86-64
- g++ (GCC) ≥ 13.1.0  (tested up through 13.1)
- CMake ≥ 4.0.1      (compatible with 3.30.x series)
- Python ≥ 3.12      (latest stable: 3.13.3)
- LLVM 17.0.0        (minimum tested version)

## Steps

```bash
sudo apt update
sudo apt install -y git cmake ninja-build g++ python3 libxml2-dev \
    libedit-dev libncurses5-dev libz-dev
```

```bash
sudo apt install -y clang lld
```

```bash
git clone --depth 1 https://github.com/llvm/llvm-project.git
```

```bash
cd llvm-project
```

```bash
cmake -S llvm -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

```bash
ninja -C build check-llvm
```

