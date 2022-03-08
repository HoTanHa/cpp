#include <iostream>

class buffer
{
private:
    /* data */
    int len;
    uint8_t *buff;

public:
    buffer(uint8_t *buffer_t, int len_t);
    ~buffer();
};

buffer::buffer(uint8_t *buffer_T, int len_t)
{
    std::cout << "Constructor...." << std::endl;
    this->len = len_t;
    this->buff = new uint8_t[len];
}

buffer::~buffer()
{
    std::cout << "Desconstructor..." << std::endl;
    if (this->buff != nullptr)
    {
        /* code */
        delete[] buff;
        buff = nullptr;
    }
}

class b1 : public buffer
{
public:
    b1(uint8_t *buff, int len) : buffer(buff, len){};
};

class b2 : public buffer
{
public:
    b2(uint8_t *buff, int len) : buffer(buff, len){};
    ~b2()
    {
    }
};
int main()
{
    char* str = "sdhjsajkds";
    int len = 10;
    b1 *a = new b1((uint8_t*)str, len);
    delete a;

    std::cout<<"======================="<<std::endl;
    b2 *a2 = new b2((uint8_t*)str, len);
    delete a2;
    
    return 0;
}