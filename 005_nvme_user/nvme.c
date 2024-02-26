#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/nvme_ioctl.h>


#define NVME_DEVICE_PATH "/dev/nvme0n1"  // NVMe设备路径

int main() {
    int fd = open(NVME_DEVICE_PATH, O_RDWR);  // 打开NVMe设备文件
    if (fd == -1) {
        perror("Failed to open NVMe device");
        return -1;
    }

	char *buffer = malloc(4096);
	if(!buffer) {
		perror("malloc");
		return -1;
	}

	memset(buffer,0,4096);

    // 设置读写命令参数
    struct nvme_user_io io;
    io.opcode = 1;  // 写操作
    io.addr = (__u64)buffer;  // 分配缓冲区，假设大小为4KB
    io.slba = 0;  // 起始逻辑块地址（LBA）
    io.nblocks = 1;  // 操作的块数，这里只写入一个块

    strcpy(buffer,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	
    // 发送命令到NVMe设备
    if (ioctl(fd, NVME_IOCTL_SUBMIT_IO, &io) == -1) {
        perror("Failed to submit IO request");
        close(fd);
        free(buffer);
        return -1;
    }
    printf("Write operation completed successfully\n");
    // 修改命令为读操作
    io.opcode = 2;
    // 发送命令到NVMe设备
     if (ioctl(fd, NVME_IOCTL_SUBMIT_IO, &io) == -1) {
        perror("Failed to submit IO request");
        close(fd);
        free(buffer);
        return -1;
    }
    printf("Read operation completed successfully\n");
    // 关闭设备文件和释放缓冲区
    close(fd);
    free(buffer);
    return 0;
}

