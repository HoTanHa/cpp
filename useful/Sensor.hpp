#ifndef __SENSOR_HPP__
#define __SENSOR_HPP__

#include "peripherals_linux.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <linux/types.h>
#include <fcntl.h>
#include <cstring>

#include <string>

namespace Sensor
{
    class FileDecription
    {
    private:
        int mFd;

    public:
        FileDecription(std::string &dev);
        ~FileDecription();
        int get_fd();
    };

    FileDecription::FileDecription(std::string &dev)
    {
        mFd = open(dev.c_str(), O_RDWR);
    }

    FileDecription::~FileDecription()
    {
        if (mFd > 0)
        {
            close(mFd);
        }
    }

    int FileDecription::get_fd()
    {
        return mFd;
    }

    ////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////

    class HDC3020
    {
    public:
        enum AutoMode
        {
            AUTO_1_MEASUREMENT_PER_2_SECONDS = 0x3220,
            AUTO_1_MEASUREMENT_PER_1_SECOND = 0x3021,
            AUTO_2_MEASUREMENTS_PER_1_SECOND = 0x3622,
            AUTO_4_MEASUREMENTS_PER_1_SECOND = 0x3423,
            AUTO_10_MEASUREMENTS_PER_1_SECOND = 0x3727,
        };

    private:
        std::string mI2cDev;
        uint8_t mI2cAddress;
        uint8_t buffer_hdc3020_send[2];
        uint8_t buffer_hdc3020_rev[10];

        uint8_t gen_crc(uint8_t *data, int len)
        {
            uint8_t crc = 0xff;
            int i, j;
            for (i = 0; i < len; i++)
            {
                crc ^= data[i];
                for (j = 0; j < 8; j++)
                {
                    if ((crc & 0x80) != 0)
                        crc = (uint8_t)((crc << 1) ^ 0x31);
                    else
                        crc <<= 1;
                }
            }
            return crc;
        }

    public:
        HDC3020(std::string &devi2c, uint8_t i2cAddress);
        HDC3020(std::string &devi2c, int PinADDR, int PinADDR1);
        ~HDC3020();

        int read_manufacturer_id(uint16_t *manuId);
        int read_NIST_ID(uint8_t *buffer_get_id);
        int soft_reset();
        int set_trigger_mode();
        int set_automode(HDC3020::AutoMode mode);
        int exit_automode();
        int read_hummidity_temperature_automode(uint32_t *humi, int *temp);
    };

    HDC3020::HDC3020(std::string &devi2c, uint8_t i2cAddress) : mI2cDev(devi2c), mI2cAddress(i2cAddress)
    {
    }

    HDC3020::HDC3020(std::string &devi2c, int PinADDR, int PinADDR1) : mI2cDev(devi2c)
    {
        mI2cAddress = ((0x44 | (PinADDR ? 0x00 : 0x01) | ((PinADDR1 ? 0x00 : 0x01) << 1)) << 1);
    }

    HDC3020::~HDC3020() {}

    int HDC3020::read_manufacturer_id(uint16_t *manuId)
    {
        FileDecription fdi2c(mI2cDev);
        int i2cfd = fdi2c.get_fd();
        if (i2cfd <= 0)
        {
            return -1;
        }
        buffer_hdc3020_send[0] = 0x37;
        buffer_hdc3020_send[1] = 0x81;
        if (i2c_read_nbyte(i2cfd, mI2cAddress, buffer_hdc3020_send, 2,
                           buffer_hdc3020_rev, 2))
        {
            return 1;
        }

        *manuId = ((static_cast<uint16_t>(buffer_hdc3020_rev[0]) << 8) + static_cast<uint16_t>(buffer_hdc3020_rev[1] & 0xFF));
        return 0;
    }

    int HDC3020::read_NIST_ID(uint8_t *buffer_get_id)
    {
        FileDecription fdi2c(mI2cDev);
        int i2cfd = fdi2c.get_fd();
        if (i2cfd <= 0)
        {
            return -1;
        }

        buffer_hdc3020_send[0] = 0x36;
        buffer_hdc3020_send[1] = 0x83;
        if (i2c_read_nbyte(i2cfd, mI2cAddress, buffer_hdc3020_send, 2,
                           buffer_hdc3020_rev, 3))
        {
            return 1;
        }
        memcpy(buffer_get_id, buffer_hdc3020_rev, 2);

        buffer_hdc3020_send[0] = 0x36;
        buffer_hdc3020_send[1] = 0x84;
        if (i2c_read_nbyte(i2cfd, mI2cAddress, buffer_hdc3020_send, 2,
                           buffer_hdc3020_rev, 3))
        {
            return 1;
        }
        memcpy(buffer_get_id + 2, buffer_hdc3020_rev, 2);

        buffer_hdc3020_send[0] = 0x36;
        buffer_hdc3020_send[1] = 0x85;
        if (i2c_read_nbyte(i2cfd, mI2cAddress, buffer_hdc3020_send, 2,
                           buffer_hdc3020_rev, 3))
        {
            return 1;
        }
        memcpy(buffer_get_id + 4, buffer_hdc3020_rev, 2);
        return 0;
    }

    int HDC3020::soft_reset()
    {
        FileDecription fdi2c(mI2cDev);
        int i2cfd = fdi2c.get_fd();
        if (i2cfd <= 0)
        {
            return -1;
        }

        buffer_hdc3020_send[0] = 0x30;
        buffer_hdc3020_send[1] = 0xA2;
        return (i2c_write_nbyte(i2cfd, mI2cAddress, buffer_hdc3020_send, 2));
    }

    int HDC3020::set_trigger_mode()
    {
        FileDecription fdi2c(mI2cDev);
        int i2cfd = fdi2c.get_fd();
        if (i2cfd <= 0)
        {
            return -1;
        }
        buffer_hdc3020_send[0] = 0x24;
        buffer_hdc3020_send[1] = 0x00;

        return (i2c_write_nbyte(i2cfd, mI2cAddress, buffer_hdc3020_send, 2));
    }

    int HDC3020::set_automode(HDC3020::AutoMode mode)
    {
        FileDecription fdi2c(mI2cDev);
        int i2cfd = fdi2c.get_fd();
        if (i2cfd <= 0)
        {
            return -1;
        }
        buffer_hdc3020_send[0] = static_cast<uint8_t>(mode & 0xFF);
        buffer_hdc3020_send[1] = static_cast<uint8_t>((mode >> 8) & 0xFF);

        return (i2c_write_nbyte(i2cfd, mI2cAddress, buffer_hdc3020_send, 2));
    }

    int HDC3020::exit_automode()
    {
        FileDecription fdi2c(mI2cDev);
        int i2cfd = fdi2c.get_fd();
        if (i2cfd <= 0)
        {
            return -1;
        }
        buffer_hdc3020_send[0] = 0x30;
        buffer_hdc3020_send[1] = 0x93;

        return (i2c_write_nbyte(i2cfd, mI2cAddress, buffer_hdc3020_send, 2));
    }

    int HDC3020::read_hummidity_temperature_automode(uint32_t *humi, int *temp)
    {

        FileDecription fdi2c(mI2cDev);
        int i2cfd = fdi2c.get_fd();
        if (i2cfd <= 0)
        {
            return -1;
        }
        buffer_hdc3020_send[0] = 0xE0;
        buffer_hdc3020_send[1] = 0x00;
        if (i2c_read_nbyte(i2cfd, mI2cAddress, buffer_hdc3020_send, 2,
                           buffer_hdc3020_rev, 6))
        {
            return 1;
        }
        uint8_t crc;
        uint32_t i2c_value;
        // check crc temperature
        crc = gen_crc(buffer_hdc3020_rev, 2);
        if (crc == buffer_hdc3020_rev[2])
        {
            i2c_value = (static_cast<uint32_t>(buffer_hdc3020_rev[0]) << 8) +
                        static_cast<uint32_t>(buffer_hdc3020_rev[1] & 0xFF);
            *temp = ((17500 * i2c_value) / 65535) - 4500;
        }
        else
        {
            return 1;
        }
        // check crc relative humidity
        crc = gen_crc(buffer_hdc3020_rev + 3, 2);
        if (crc == buffer_hdc3020_rev[5])
        {
            i2c_value = (static_cast<uint32_t>(buffer_hdc3020_rev[3]) << 8) +
                        static_cast<uint32_t>(buffer_hdc3020_rev[4] & 0xFF);
            *humi = (10000 * i2c_value) / 65535;
        }
        else
        {
            return 1;
        }
        return 0;
    }
}

#endif // __SENSOR_HPP__