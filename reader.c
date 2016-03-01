#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MSG_SIZE 1024

int main() {
	int ret, fd;
	char msg[MSG_SIZE];
	char *device_path = "/dev/scull_char_dev";
	fd = open(device_path, O_RDONLY);

	if(fd > 0)
	{
		printf("/dev/scull_char_dev opened successfully; fd= %d\n",fd);
	} else {
		printf("Problem while opening : /dev/scull_char_dev\n");
		goto OUT;
	}


	ret = read(fd, msg, MSG_SIZE );
	if(ret) {
		printf("Read %d bytes to %s\n", ret, device_path);
	
	} else {
		printf("Read failed with return code:%i, for path:%s\n", ret, device_path);
		goto OUT;
	}
	close(fd);
	return 0;
OUT:
	return -1;
}
