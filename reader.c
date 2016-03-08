#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define MSG_SIZE 1024

int main() {
	int ret, fd, offset, msg_size;
	int c=0;
	char msg[MSG_SIZE];
	char *device_path = "/dev/scull_char_dev";
	fd = open(device_path, O_RDONLY);

RETRY:
	while(c != 2) {
		printf("Please enter the no of characters to be read:");
		scanf("%d", &msg_size);
		printf("Please enter the offset for reading:");
		scanf("%d", &offset);
		
		if(msg_size > 1024 || msg_size<0)
			goto RETRY;
	
		if(fd > 0)
		{
			printf("/dev/scull_char_dev opened successfully; fd= %d\n",fd);
		} else {
			printf("Problem while opening : /dev/scull_char_dev\n");
			goto OUT;
		}
	
		lseek(fd, offset, SEEK_SET);
		ret = read(fd, msg, msg_size );
	
		if(ret) {
			printf("Read %d bytes to %s\n", ret, device_path);
		
		} else {
			printf("Read failed with return code:%i, for path:%s\n", ret, device_path);
			goto OUT;
		}	
		close(fd);
		c++;
	}
	return 0;
OUT:
	return -1;
}
