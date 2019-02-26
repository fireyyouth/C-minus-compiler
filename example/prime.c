def i = 2;
while (i < 100) {
    def flag = 0;
    def j = 2;
    while (flag == 0 && j < i) {
        if (i % j == 0) {
            flag = 1;
        } else {
            j = j + 1;
        }
    }
    if (flag != 1) {
        write i;
    }
    i = i + 1;
}

