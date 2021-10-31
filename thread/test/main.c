#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



#define PAGESIZE 4096

int main()
{
    int fd = open ("./txt", O_RDONLY);
    u_int8_t *first = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE,  -1, 0);

    struct stat sb;
    fstat(fd, &sb);

    char *file_in_memory = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    printf("%c\n", file_in_memory[1]);
}
