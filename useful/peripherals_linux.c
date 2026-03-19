#include "peripherals_linux.h"

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int i2c_write_byte_register(int fd, unsigned char slave_addr, unsigned char reg_addr, unsigned char value)
{
    unsigned char outbuf[2];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    messages[0].addr = slave_addr >> 1;
    messages[0].flags = 0;
    messages[0].len = sizeof(outbuf);
    messages[0].buf = outbuf;

    outbuf[0] = reg_addr & 0xff;
    outbuf[1] = value & 0xff;

    packets.msgs = messages;
    packets.nmsgs = 1;
    if (ioctl(fd, I2C_RDWR, &packets) < 0)
    {
        perror("Unable to send data");
        return 1;
    }
    return 0;
}

int i2c_read_byte_register(int fd, unsigned char slave_addr, unsigned char reg_addr, unsigned char *value)
{
    unsigned char inbuf[2];
    unsigned char outbuf[2];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    outbuf[0] = reg_addr & 0xff;

    messages[0].addr = slave_addr >> 1;
    messages[0].flags = 0;
    messages[0].len = 1;
    messages[0].buf = outbuf;

    messages[1].addr = slave_addr >> 1;
    messages[1].flags = I2C_M_RD /* | I2C_M_NOSTART*/;
    messages[1].len = 1;
    messages[1].buf = inbuf;

    packets.msgs = messages;
    packets.nmsgs = 2;
    if (ioctl(fd, I2C_RDWR, &packets) < 0)
    {
        perror("Unable to send data");
        return 1;
    }
    *value = inbuf[0];
    return 0;
}

int i2c_write_nbyte(int fd, unsigned char slave_addr, unsigned char *write_buff, int nWrite)
{
    unsigned char *outbuf = malloc(nWrite + 1);
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];
    int res = 0;

    memcpy(outbuf, write_buff, nWrite);
    messages[0].addr = slave_addr >> 1;
    messages[0].flags = 0;
    messages[0].len = nWrite;
    messages[0].buf = outbuf;

    packets.msgs = messages;
    packets.nmsgs = 1;
    if (ioctl(fd, I2C_RDWR, &packets) < 0)
    {
        perror("Unable to send data");
        res = 1;
    }
    free(outbuf);
    return res;
}

int i2c_read_nbyte(int fd, unsigned char slave_addr, unsigned char *write_buff, int nWrite, unsigned char *read_buff, int nRead)
{
    unsigned char *inbuf = malloc(nRead + 1);
    unsigned char *outbuf = malloc(nWrite + 1);
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];
    int res = 0;

    memcpy(outbuf, write_buff, nWrite);

    messages[0].addr = slave_addr >> 1;
    messages[0].flags = 0;
    messages[0].len = sizeof(outbuf);
    messages[0].buf = outbuf;

    messages[1].addr = slave_addr >> 1;
    messages[1].flags = I2C_M_RD /* | I2C_M_NOSTART*/;
    messages[1].len = nRead;
    messages[1].buf = inbuf;

    packets.msgs = messages;
    packets.nmsgs = 2;
    if (ioctl(fd, I2C_RDWR, &packets) < 0)
    {
        perror("Unable to send data");
        res = 1;
    }
    memcpy(read_buff, inbuf, nRead);
    free(inbuf);
    free(outbuf);
    return res;
}
