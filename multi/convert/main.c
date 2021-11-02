#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char **argv)
{
	unsigned char count;
	char c;
	struct stat f_info;
	int fd = open(argv[1], O_RDONLY);
	fstat(fd, &f_info);
	long long size = f_info.st_size;
	char *file = mmap(NULL, 4096, PROT_READ, MAP_PRIVATE, fd, 0);
	long long  i = 0;
	while (i < size)
	{
		count = *(file + 1);
		c = *file;
		for (unsigned char j = 0; j < count; ++j)
			write(1, &c, 1);
		i += 2;
		file += 2;
	}
}
