#include "excute.h"

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        FILE *fp;
        char start_char;
        char counter;
        for (int i = 1; i < argc; ++i)
        {
            fp = fopen(argv[i], "r");
            if (!fp) 
            {
                printf("invalid file\n");
                return 1;
            }
            if (i == 1) {
                start_char = fgetc(fp);
                counter = 1;
            }
            counter = excute(&start_char, counter, fp);
            if (i + 1 == argc) {
                write(1, &start_char, 1);
                write(1, &counter, 1);
            }
            fclose(fp);
        }
    }
}
