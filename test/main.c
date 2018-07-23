/********************************头文件****************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
/********************************宏定义****************************************/
#define REG_R      0
#define REG_W      1
/*********************************函数*****************************************/
int main(int argc, char *argv[])
{
    int ret = 0;
    int fd;
	unsigned int buf[2];

    if((argc != 3) && ( argc != 4)) {
        printf("usage: %s <reg addr> <r|w> [value] \n", argv[0]);
    	return -1;
    }

	fd = open("/dev/reg_tool", O_RDWR);
	if (fd < 0) {
		printf("can't open /dev/reg_tool\n");
		return -1;
	}

	switch (argc) {
	case 3:
        ret = strcmp ("r", argv[2]);
        if (ret)
            goto errout;
        buf[0] = strtoul(argv[1], NULL, 0);
        ioctl(fd, REG_R, buf);  /* val = buf[1] */
        printf("reg[0x%08x]=0x%08x\n", buf[0], buf[1]);
		break;
	case 4:
        ret = strcmp ("w", argv[2]);
        if (ret)
            goto errout;
        buf[0] = strtoul(argv[1], NULL, 0);
		buf[1] = strtoul(argv[3], NULL, 0);
        ioctl(fd, REG_W, buf);  /* val = buf[1] */
		break;
    default:
        goto errout;
    }

    return 0;

errout:
    printf("usage: %s <reg addr> <r|w> [value] \n", argv[0]);
	return ret;
}

