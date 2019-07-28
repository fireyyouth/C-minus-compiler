# C-minus-compiler, compiling a C-like language to X64 asm

# requirements
- X64 machine
- cmake 3.1.0
- C++14 compiler

# build
```
mkdir build
cd build
cmake ..
make
```

# run
for Mac user
```
./main example.c > example.asm
nasm -f macho64 example.asm -o example.o
clang example.o -o example
./example
```
for Linux user
```
./main example.c > example.asm
nasm -f elf64 example.asm -o example.o
gcc example.o -o example
./example
```
# example code
echo.c
```
def x;
read x;
write x;
```
max.c
```
def max = 0;
def n;
read n;
while (n > 0) {
    n = n - 1;
    def t;
    read t;
    if (t > max) {
        max = t;
    }
}
write max;
```
