// SPDX-License-Identifier: GPL-2.0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define V240P _IOW('a', 'a', int*)
#define V360P _IOW('a', 'b', int*)
#define V480P _IOW('a', 'c', int*)
#define V720P _IOW('a', 'd', int*)

static int ff_initialize(int fd, int cmd, int count);

int main(void)
{
	int fd, fd1;
	int count = 1;		//By default frame count is 1
	int cmd;		//Resolution
	int rd = 0;
	char head[20];

	fd = open("/dev/ffemulator", O_RDWR);
	if (fd < 0) {
		printf("device file cannot be opened..\n");
		return 0;
	}
	printf("Enter required resolution for test frame..\n");
	printf("\t240 for 240p\n\t360 for 360p\n\t480 for 480p\n\t720 for 720p\n");
	scanf("%d", &cmd);
	if (ff_initialize(fd, cmd, count))
		return 0;
	printf("Enter 1 to read frame..\n");
	scanf("%d", &rd);
	if (rd == 1) {
		unsigned char *data;
		int size;

		switch (cmd) {
		case 240:
			strcpy(head, "P6\n426 240\n255\n");
			size = 306720;
			break;
		case 360:
			strcpy(head, "P6\n640 360\n255\n");
			size = 691200;
			break;
		case 480:
			strcpy(head, "P6\n854 480\n255\n");
			size = 1229760;
			break;
		default:
			strcpy(head, "P6\n1280 720\n255\n");
			size = 2764800;
			break;
		}
		remove("sample.ppm");
		fd1 = open("sample.ppm", O_WRONLY | O_CREAT, 0777);
		write(fd1, head, sizeof(head));
		close(fd1);
		data = malloc(size);
		read(fd, data, size);
		fd1 = open("sample.ppm", O_WRONLY | O_APPEND);
		write(fd1, data, size);
		close(fd1);
	}
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
	default:
		printf("sorry..! The option entered is not valid..\n");
		return -1;
	}
}
