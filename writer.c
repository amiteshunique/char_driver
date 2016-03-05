#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
int main() {
	int ret, fd;
	int noc;	// No. of characters to be written.
	int len;
	char *msg = "123456789A123456789B123456789C123"; 
	char *device_path = "/dev/scull_char_dev";
	fd = open(device_path, O_WRONLY);
	
	printf("Please enter the number of characters to be written: ");
	scanf("%d",&noc);

	if(noc > len || noc <0)
		len = noc;
	else
		len = strlen(msg);
	if(fd > 0)
	{
		printf("/dev/scull_char_dev opened successfully; fd= %d\n",fd);
	} else {
		printf("Problem while opening : /dev/scull_char_dev\n");
		goto OUT;
	}


	ret = write(fd, msg, len );
	if(ret) {
		printf("Wrote %d bytes to %s\n", ret, device_path);
	
	} else {
		printf("Wrote failed to write with return code:%i, for path:%s\n", ret, device_path);
		goto OUT;
	}
	close(fd);
	return 0;
OUT:
	return -1;
}
