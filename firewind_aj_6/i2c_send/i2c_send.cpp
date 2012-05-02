#include <linux/i2c-dev.h>
#include <fcntl.h>   /* File control definitions */
#include <errno.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if(argc != 7)
	{
		printf("Usage: ./i2c_send device_name command v1 v2 v3 v4\r\n");
		return -1;
	}

       char buf[5] = {atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6])};
 
	printf("SENDING TO %s command %d value %d %d %d %d ", argv[1], buf[0], buf[1], buf[2], buf[3], buf[4]);
	
	int fd = open(argv[1], O_RDWR);
        if(fd < 0)
        {
            printf("I2C: error opening device: %s\r\n", strerror(errno));
            return -1;
        }

        if(ioctl(fd, I2C_SLAVE, 0x1b) < 0)
        {
            printf("I2C: error selecting I2C_SLAVE: %s\r\n", strerror(errno));
            close(fd);
            return -1;
        }

        // Do it
        if(write(fd, buf, 5) != 5)
        {
            printf("I2C: error writing data: %s\r\n", strerror(errno));
            close(fd);
            return -1;
        }

	close(fd);
	
	printf("SUCCESS\r\n");
	return 0;
}
 
