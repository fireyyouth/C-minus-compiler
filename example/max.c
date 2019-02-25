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
