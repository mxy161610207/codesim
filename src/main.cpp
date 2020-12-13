#include<stdio.h>
#include<stdlib.h>
#include "MyPaser.h"
using namespace std;

void ShowHelp() {
    fprintf(stderr, "usage: codesim [-v|--verbose] [-h|--help] code1 code2\n");
    return;
}


int main(int argc, char* argv[])
{
    char* filename1, *filename2;
    if (argc == 3) {
        filename1 = argv[1];
        filename2 = argv[2];
    }
    else {
        ShowHelp();
        exit(-1);
    }

    double res = MyPaser::PaserCompare(filename1, filename2);
    res = min(res, 1.0);
    printf("%.2lf\n", res);

    return 0;
}