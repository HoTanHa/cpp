#include <bits/stdc++.h>
#include <regex>

std::string *readDataDeviceKeyOnFile()
{
    std::fstream f;
    std::string serial;
    std::string *dataFile = new std::string("");
    f.open("abc.txt");
    if (f)
    {
        while (!f.eof())
        {
            std::getline(f, serial);
            dataFile->append(serial);
            dataFile->append("  ");
        }
    }
    f.close();
    return dataFile;
}
int main()
{
    std::vector<long> serialCheck;
    std::fstream f;
    std::string serial;
    std::string *dataFile = new std::string("");
    f.open("abc.txt");
    if (f)
    {
        while (!f.eof())
        {
            std::getline(f, serial);
            dataFile->append(serial);
            dataFile->append("  ");
        }
    }
    f.close();

    std::regex word_regex("([689][0-9]{8})");
    auto words_begin =
        std::sregex_iterator(dataFile->begin(), dataFile->end(), word_regex);
    auto words_end = std::sregex_iterator();
    for (std::sregex_iterator i = words_begin; i != words_end; ++i)
    {
        std::smatch match = *i;
        serialCheck.push_back(std::atol(match.str().c_str()));
    }

    for (std::vector<long>::iterator it = serialCheck.begin(); it != serialCheck.end(); ++it)
    {
        std::cout << *it << "  ";
    }
    std::cout << "\r\n";

    delete dataFile;
    serialCheck.clear();
}