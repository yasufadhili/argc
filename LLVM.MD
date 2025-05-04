# Setting up LLVM

## Environment
- Ubuntu 22.04 on x86-64
- g++ 13.1.0 
- Cmake 4.0.0 (Think 3.x may work)
- Ninja 1.12.1
- Python 3.12
- GNU Make 4.3
- PyYAML 5.1
- zlib 

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

