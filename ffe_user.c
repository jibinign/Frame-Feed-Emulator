// SPDX-License-Identifier: GPL-2.0

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define V240P _IOW('a', 'a', int*)
#define V360P _IOW('a', 'b', int*)
#define V480P _IOW('a', 'c', int*)
#define V720P _IOW('a', 'd', int*)
#define V1080P _IOW('a', 'e', int*)

static int ff_initialize(int fd, int cmd, int count);

int main(void)
{
	int fd;
	int count = 1;		//By default frame count is 1
	int cmd;		//Resolution

	fd = open("/dev/ffemulator", O_RDWR);
	if (fd < 0) {
		printf("device file cannot be opened..\n");
		return 0;
	}
	printf("Enter required resolution for test frame..\n");
	printf("\t240 for 240p\n\t360 for 360p\n\t480 for 480p\n\t720 for 720p\n\t1080 for 1080p\n");
	scanf("%d", &cmd);
	if (ff_initialize(fd, cmd, count))
		return 0;
	close(fd);
	return 0;
}

static int ff_initialize(int fd, int cmd, int count)
{
	switch (cmd) {
	case 240:
		ioctl(fd, V240P, (int *) &count);
		break;
	case 360:
		ioctl(fd, V360P, (int *) &count);
		break;
	case 480:
		ioctl(fd, V480P, (int *) &count);
		break;
	case 720:
		ioctl(fd, V720P, (int *) &count);
		break;
	case 1080:
		ioctl(fd, V1080P, (int *) &count);
		break;
	default:
		printf("sorry..! The option entered is not valid..\n");
		return -1;
	}
}
