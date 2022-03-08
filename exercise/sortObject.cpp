#include <iostream>
#include <algorithm>
#include <vector>

class DataImage
{
private:
    int imageSize;
    char *name;

public:
    DataImage(int size)
    {
        this->imageSize = size;
        name = new char [10];
    }
    ~DataImage()
    {
        delete [] name;
    }

    void printSize()
    {
        std::cout << "Size: " << imageSize << std::endl;
    }
    int getSize()
    {
        return imageSize;
    }
    friend bool operator== (const DataImage& c1, const DataImage& c2);
    friend bool operator!= (const DataImage& c1, const DataImage& c2);

    friend bool operator< (const DataImage& c1, const DataImage& c2);
    friend bool operator> (const DataImage& c1, const DataImage& c2);

    friend bool operator<= (const DataImage& c1, const DataImage& c2);
    friend bool operator>= (const DataImage& c1, const DataImage& c2);
};

bool operator== (const DataImage& c1, const DataImage& c2)
{
    return c1.imageSize == c2.imageSize;
}

bool operator!= (const DataImage& c1, const DataImage& c2)
{
    return c1.imageSize != c2.imageSize;
}

bool operator< (const DataImage& c1, const DataImage& c2)
{
    return c1.imageSize < c2.imageSize;
}

bool operator> (const DataImage& c1, const DataImage& c2)
{
    return c1.imageSize > c2.imageSize;
}

bool operator<= (const DataImage& c1, const DataImage& c2)
{
    return c1.imageSize <= c2.imageSize;
}

bool operator>= (const DataImage& c1, const DataImage& c2)
{
    return c1.imageSize >= c2.imageSize;
}

struct sortDataImage
{
    bool operator()(DataImage *image1, DataImage *image2) const
    {
        return (image1->getSize() < image2->getSize());
    }
};

bool sizeLess(DataImage *img1, DataImage *img2)
{
    return (img1->getSize() < img2->getSize());
};

int main()
{
    std::vector<DataImage *> dataImagVector;
    DataImage *img1 = new DataImage(20);
    DataImage *img2 = new DataImage(10);
    DataImage *img3 = new DataImage(50);
    DataImage *img4 = new DataImage(40);

    dataImagVector.push_back(img1);
    dataImagVector.push_back(img2);
    dataImagVector.push_back(img3);
    dataImagVector.push_back(img4);
    // okok
    // std::sort(dataImagVector.begin(), dataImagVector.end(), &sizeLess);
    // not Ok
    std::sort(dataImagVector.begin(), dataImagVector.end());
    // ok ok
    // std::sort(dataImagVector.begin(), dataImagVector.end(), sortDataImage());

    for (std::vector<DataImage *>::iterator constIterator = dataImagVector.begin();
         constIterator != dataImagVector.end();
         constIterator++)
    {
        DataImage *a = *constIterator;
        a->printSize();
    }

    for (std::vector<DataImage *>::iterator constIterator = dataImagVector.begin();
         constIterator != dataImagVector.end();
         constIterator++)
    {
        DataImage *a = *constIterator;
        delete a;
    }
    return 0;
}

// valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./a.out
// g++ -g --std=c++11 sortObject.cpp 