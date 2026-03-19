#ifndef __PERIPHERAL_LINUX_H__
#define __PERIPHERAL_LINUX_H__

#ifdef __cplusplus
extern "C"
{
#endif

    int i2c_write_byte(int fd, unsigned char slave_addr, unsigned char reg_addr, unsigned char value);
    int i2c_read_byte(int fd, unsigned char slave_addr, unsigned char reg_addr, unsigned char *value);
    int i2c_write_nbyte(int fd, unsigned char slave_addr, unsigned char *write_buff, int nWrite);
    int i2c_read_nbyte(int fd, unsigned char slave_addr, unsigned char *write_buff, int nWrite, unsigned char *read_buff, int nRead);

#ifdef __cplusplus
}
#endif
#endif //__PERIPHERAL_LINUX_H__
