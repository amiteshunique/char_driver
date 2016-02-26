#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int main() {
	int ret, fd;

	fd = open("/dev/scull_char_dev", O_WRONLY);

	if(fd > 0)
	{
		printf("/dev/scull_char_dev opened successfully; fd= %d\n",fd);
	} else {
		printf("Problem while opening : /dev/scull_char_dev\n");
		goto OUT;
	}

	close(fd);
	return 0;
OUT:
	return -1;
}
