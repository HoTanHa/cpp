#include <iostream>
#include <cassert>
#include <ctype.h>

#include <cstring>
// enum
// {
//     /*********RN6864**********/
//     /*CSI0 config choose*/
//     CSI0_CH0CH1CH2CH3_720P = 0,
//     CSI0_CH0CH1CH2CH3_1080P,
//     /*CSI1 config choose*/
//     CSI1_CH0CH1CH2CH3_720P,
//     /*CSI2 config choose*/
//     CSI2_CH0CH1CH2CH3_720P,

//     /**********N4***************/
//     /*CSI1 config choose*/
//     CSI1_CH0CH1CH2CH3_1080P_N4,
//     CSI1_CH0CH1CH2CH3_720P_N4,

//     /**********ADSUN**************/
//     CSI2_ADSUN,
//     CSI2_CH0CH1CH2CH3_1080P,
//     CSI2_CH0CH1_1080P,
//     CSI2_CH2CH3_1080P,
//     CSI0_ADSUN,
//     CSI0_ADSUN_720P720P720P720P,
//     CSI0_ADSUN_720P720P720P1080P,
//     CSI0_ADSUN_720P720P1080P720P,
//     CSI0_ADSUN_720P720P1080P1080P,

//     CSI0_ADSUN_720P1080P720P720P,
//     CSI0_ADSUN_720P1080P720P1080P,
//     CSI0_ADSUN_720P1080P1080P720P,
//     CSI0_ADSUN_720P1080P1080P1080P,

//     CSI0_ADSUN_1080P720P720P720P,
//     CSI0_ADSUN_1080P720P720P1080P,
//     CSI0_ADSUN_1080P720P1080P720P,
//     CSI0_ADSUN_1080P720P1080P1080P,

//     CSI0_ADSUN_1080P1080P720P720P,
//     CSI0_ADSUN_1080P1080P720P1080P,
//     CSI0_ADSUN_1080P1080P1080P720P,
//     CSI0_ADSUN_1080P1080P1080P1080P,

//     INPUT_TYPE_MUX,
// };

struct msm_camera_i2c_reg_array
{
    unsigned short reg_addr;
    unsigned short reg_data;
    unsigned int delay;
};

struct msm_camera_i2c_reg_setting_array
{
    struct msm_camera_i2c_reg_array reg_setting_a[100];
    unsigned short size;
    unsigned short delay;
};
struct msm_camera_i2c_reg_setting
{
    struct msm_camera_i2c_reg_array *reg_setting;
    unsigned short size;
    unsigned short delay;
};

struct sensor_param
{
        struct msm_camera_i2c_reg_setting *sensor_start_setting;
        int a;
        int b;
};

static struct msm_camera_i2c_reg_array csi0_ch0ch1_1080p_start_regs[] = {
    {0x81, 0x0F, 0x00},
    {0xDF, 0xF0, 0x00},
    {0xF0, 0xC0, 0x00},
    {0x88, 0x40, 0x00},
    {0xF6, 0x40, 0x00},
    {0xFF, 0x00, 0x00},};

static struct msm_camera_i2c_reg_setting csi0_ch0ch1_1080p_start_setting = {
    .reg_setting = csi0_ch0ch1_1080p_start_regs,
    .size = sizeof(csi0_ch0ch1_1080p_start_regs) / sizeof(csi0_ch0ch1_1080p_start_regs[0]),
    .delay = 0,
};
struct sensor_param csi0_ch0ch1_1080p_info_ais = {
    .sensor_start_setting = &csi0_ch0ch1_1080p_start_setting,
    .a = 10,
    .b=20,
};

    void print_number(int *myInt){
        assert(myInt != nullptr);
std::cout << *myInt << std::endl;
}

void run_cassert()
{
    int a = 5;
    int *b = nullptr;
    int *c = nullptr;
    b = &a;
    print_number(b);
    // print_number(c);
    int *d = &(*b);
    print_number(d);
}

void run_ctype()
{
    int i = 0;
    char str[] = "C++ cplusplus 123";
    while (str[i])
    {
        // if (isalpha(str[i]))
        //     printf("character %c is alphabetic\n", str[i]);
        // else
        //     printf("character %c is not alphabetic\n", str[i]);
        if (isalnum(str[i]))
            printf("character %c is alpha numberic\n", str[i]);
        else
            printf("character %c is not alpha numberic\n", str[i]);
        i++;
    }
}

void run_isxdigit()
{
    char str[] = "f0A1cA";
    long int number;
    if (isxdigit(str[0]))
    {
        number = strtol(str, NULL, 16);
        printf("The hexadecimal number %lX is %ld.\n", number, number);
    }
}

#include "abc.h"
#include <ostream>
#include <fstream>
int main()
{
    // run_cassert()
    // run_ctype();
    // run_isxdigit();
    // int sd = 121210;
    // int a = sd - 9;
    // if (a & 0x01)
    // {
    //     printf("aaaaaaaaa");
    // }
    // else
    // {
    //     printf("bbbbbbbbbb");
    // }

    // if (a & 0x02)
    // {
    //     printf("aaaaaaaaa");
    // }
    // else
    // {
    //     printf("bbbbbbbbbb");
    // }
    // std::cout << std::endl;

    // memcpy(&(arrayabc[0]), &a1, sizeof(a1));
    // memcpy(&(arrayabc[1]), &(b1[0]), sizeof(b1));

    // csi0_ch0ch1_1080p_info_ais.sensor_start_setting->reg_setting
    //         std::cout
    //     << arrayabc[0].a << ".." << arrayabc[0].b << ".." << arrayabc[0].c << ".." << std::endl;
    // std::cout << arrayabc[2].a << ".." << arrayabc[2].b << ".." << arrayabc[2].c << ".." << std::endl;

    int length;
    char *array;

    std::ifstream myStream;
    myStream.open("/home/htha/Camera_1_08_23_22 13_12_2021.jpg", std::ios_base::binary);
    // myStream.open("/home/htha/Camera_1_08_23_22 13_12_2021_1.jpg", std::ios_base::binary);

    if (!myStream.bad())
    {
        std::cout << std::hex;
        std::cout.width(2);

        while (!myStream.eof())
        {
            unsigned char c;
            myStream >> c;
            std::cout << static_cast<unsigned>(c);
        }
    }

    return 0;
}