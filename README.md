# C-minus-compiler, tested on mac
nasm -f macho64  test.asm -o test.o && clang test.o && ./a.out
example
```
def x = 1;
def i = 0;
while (i < 10) {
    x = x + x;
    i = i+1;
}
write x;
```
