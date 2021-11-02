#include "main.h"

char    excute(char *start_char, char counter, FILE *file_str)
{
    char cur_char;
  
    while (1)
    {
        cur_char = fgetc(file_str);
        if (cur_char == EOF)
        {
            return counter;
        }
        if (*start_char != cur_char){
            write(1, start_char, 1);
            write(1, &counter, 1);
            *start_char = cur_char;
            counter = 0;
        }
        ++counter;
    }
}
