def x;
read x;
def i = 2;
while (x > 1) {
    while (x % i == 0) {
        x = x / i;
        write i;
    }
    i = i + 1;
}
