def year = 2000;
while (year < 2020) {
    write year;
    if(year % 4 == 0) {
        if( year % 100 == 0) {
            if ( year % 400 == 0) {
                write 1;
            } else {
                write 0;
            }
        } else {
            write 1;
        }
    } else {
        write 0;
    }
    year = year + 1;
}

