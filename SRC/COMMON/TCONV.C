main()
{
    int i;
    for (i=0; i<256; i++) {
        if (kutostd(stdtoku(i)) != i) {
            printf("In std->ku  code wrong = %X\n", i);
        }
    }
    for (i=0; i<256; i++) {
        if (stdtoku(kutostd(i)) != i) {
            printf("In ku->std  code wrong = %X\n", i);
        }
    }
}
