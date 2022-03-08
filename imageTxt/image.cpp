#include <iostream>
#include <fstream>
#include <unistd.h>
#include "/home/htha/Code/cpp/colorize/color.hpp"
#include <vector>
#include "font_12x24.h"
#include "font-12x16.h"

std::string getFont12_24(char kt)
{
    std::string a;

    int start = kt * 2 * 24;
    uint16_t value = 0;
    for (int i = 0; i < 24; i++)
    {
        value = console_font_12x24[start + 2 * i] * 0x100 + console_font_12x24[start + 2 * i + 1];

        a.append("\t\t\t\t\t\t");
        for (int j = 0; j < 14; j++)
        {
            if (value & (0x8000 >> j))
            {
                a.append("*");
            }
            else
            {
                a.append(" ");
            }
        }
        a.append("\r\n");
    }

    a.append("\r\n");
    a.append("\r\n");

    return a;
}

std::string getFont12_16(char kt)
{
    std::string a;

    a.append("\r\n");
    a.append("\r\n");
    a.append("\r\n");
    a.append("\r\n");
    a.append("\r\n");
    a.append("\r\n");
    a.append("\r\n");
    int start = kt * 2 * 16;
    uint16_t value = 0;
    for (int i = 0; i < 16; i++)
    {
        value = console_font_12x16[start + 2 * i] * 0x100 + console_font_12x16[start + 2 * i + 1];

        a.append("                                                       ");
        for (int j = 0; j < 12; j++)
        {
            if (value & (0x8000 >> j))
            {
                a.append("*");
            }
            else
            {
                a.append(" ");
            }
        }
        a.append("                                                        \r\n");
    }
    a.append("\r\n");
    a.append("\r\n");

    return a;
}

std::vector<std::string> foreground;
void add_foreground_color()
{
    for (auto [set, code_set] : color::color_foreground)
    {
        foreground.push_back(set);
    }
}
std::vector<std::string> background;
void add_background_color()
{
    for (auto [set, code_set] : color::color_background)
    {
        background.push_back(set);
    }
}

int main()
{
    system("clear");
    add_foreground_color();
    add_background_color();
    system("clear");
    // for (size_t i = 0; i < 10; i++)
    // {
    //     std::string a = getFont12_16(0x39 - (char)i);
    //     std::cout << color::rize(a, foreground.at(i + 2)) << std::endl;
    //     usleep(500000);

    //     system("clear");
    // }
    // // for (size_t i = 0; i < 10; i++)
    // // {
    // //     std::string a = getFont12_24((char)i + 0x30);
    // //     std::cout << color::rize(a, foreground.at(foreground.size()-i-1)) << std::endl;
    // //     usleep(300000);

    // //     system("clear");
    // // }

    // int num = 0;
    // for (size_t i = 0; i < 10; i++)
    // {

    //     std::cout << "\r\n\r\n\r\n\r\n";
    //     num = i;
    //     std::string pathFile = "/home/htha/Code/cpp/imageTxt/file";
    //     pathFile.append(std::to_string(num));
    //     pathFile.append(".txt");
    //     std::ifstream myFile(pathFile);
    //     if (myFile.fail())
    //     {
    //         continue;
    //     }

    //     for (std::string line; std::getline(myFile, line);)
    //     {
    //         std::cout << line << std::endl;
    //         usleep(5000);
    //     }
    //     myFile.close();
    //     std::cout << std::endl;
    //     sleep(2);
    //     system("clear");
    // }

    // std::string path2 = "/home/htha/Code/cpp/imageTxt/birthday.txt";
    // std::ifstream file2(path2);
    // if (file2.fail())
    // {
    //     return 1;
    // }

    // int sie = foreground.size();
    // int con = 0;
    // for (std::string line; std::getline(file2, line);)
    // {
    //     int i = (con++) % (sie - 2) + 2;
    //     std::string reset = "All";
    //     std::string force = foreground.at((i));
    //     std::string bgrd = background.at(0);
    //     std::string set = "Default";
    //     std::cout << color::rize(line, force, bgrd, set, reset) << std::endl;
    //     // std::cout << line << std::endl;
    //     usleep(50000);
    // }
    // file2.close();
    // sleep(20);

    // system("clear");
    std::string path3 = "/home/htha/Code/cpp/imageTxt/abc.txt";
    std::ifstream file3(path3);
    if (file3.fail())
    {
        return 1;
    }
    for (std::string line; std::getline(file3, line);)
    {
        // std::cout << color::rize(line, "Red", "White") << std::endl;
        std::cout << color::rize(line, "Red", "Light Gray") << std::endl;
        // std::cout << line << std::endl;
        usleep(50000);
    }
    file3.close();

    return 0;
}