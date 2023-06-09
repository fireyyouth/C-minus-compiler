# overview
A simple handcrafted compiler, compiling a C-like language to x86_64 asm
- demo.c (print 10 fibonacci numbers)
```
def x = 1;
def y = 0;
def i = 0;
while (i < 10) {
    def z = x + y;
    write z;
    x = y;
    y = z;
    i = i + 1;
}
```

# build requirements
- x86_64 machine
- cmake >= 3.1.0
- C++14 compiler

# build steps
```
mkdir build
cd build
cmake ..
make
```

# run
- for Mac user, need nasm and clang
```
./main demo.c > demo.asm
nasm -f macho64 demo.asm -o demo.o
clang demo.o -o demo
./demo
```
- for Linux user, need nasm and gcc
```
./main demo.c > demo.asm
nasm -f elf64 demo.asm -o demo.o
gcc demo.o -o demo
./demo
```

