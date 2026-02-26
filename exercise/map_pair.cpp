#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>


class ParameterId
{
public:
    enum Id_Thing
    {
        ID_THING_1 = 1,
        ID_THING_2,
        ID_THING_3,
        ID_THING_4,
        ID_THING_5
    };

private:
    std::map<int, std::string> nameIdArray;

public:
    static ParameterId *getInstance()
    {
        static ParameterId instance;
        return &instance;
    }
    std::string getNameThingById(int id)
    {
        std::map<int, std::string>::iterator it = nameIdArray.find(id);
        if (it != nameIdArray.end())
        {
            return it->second;
        }
        return "Not Found";
    }

    int getIdThingByName(const std::string &name)
    {
        for (const auto &pair : nameIdArray)
        {
            if (pair.second == name)
            {
                return pair.first;
            }
        }
        return 0; // Not Found
    }

private:
    ParameterId(/* args */);
    ~ParameterId();
};

ParameterId::ParameterId(/* args */)
{
    nameIdArray.insert(std::make_pair(ParameterId::ID_THING_1, "ID_THING_1"));
    nameIdArray.insert(std::make_pair(ParameterId::ID_THING_2, "ID_THING_2"));
    nameIdArray.insert(std::make_pair(ParameterId::ID_THING_3, "ID_THING_3"));
    nameIdArray.insert(std::make_pair(ParameterId::ID_THING_4, "ID_THING_4"));
    nameIdArray.insert(std::make_pair(ParameterId::ID_THING_5, "ID_THING_5"));

    // std::pair<int, std::string> pair2(ParameterId::ID_THING_2, "ID_THING_2");
    // nameIdArray.insert(pair2);
}

ParameterId::~ParameterId()
{
}

int main()
{
    std::string input;
    while (true)
    {
        std::cout << "\nEnter a number (or non-number to exit): ";
        std::getline(std::cin, input);
        if (input.empty() || !std::all_of(input.begin(), input.end(), ::isdigit))
        {
            break;
        }
        int number = std::stoi(input);
        std::string name = ParameterId::getInstance()->getNameThingById(number);
        std::cout << "Name: " << name << std::endl;
        int id = ParameterId::getInstance()->getIdThingByName(name);
        std::cout << "ID: " << id << std::endl;
    }

    return 0;
}
/************************************************
  g++ map_pair.cpp --std=c++11
*************************************************/