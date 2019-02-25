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
