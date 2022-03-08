
#define PI 3.14159265359

#include <math.h>
#include <iostream>

class point
{
private:
  float x, y;

public:
  void setPoint(float a, float b)
  {
    x = a;
    y = b;
  };

  friend point setVector(point a, point b)
  {
    point c;
    c.x = b.x - a.x;
    c.y = b.y - a.y;
    return c;
  };

  friend float calculateAngle(point vectora, point vectorb)
  {
    float t = (vectora.x * vectorb.x + vectora.y * vectorb.y);
    float m;
    m = sqrt(pow(vectora.x, 2) + std::pow(vectora.y, 2)) * sqrt(pow(vectorb.x, 2) + std::pow(vectorb.y, 2));
    return std::acos(t / m);
  };
};

class polygon
{
private:
  point *p;
  int n;

public:
  polygon(int m)
  {
    n = m;
    p = new point[m];
  };
  void setPolygon()
  {
    float a, b;
    for (int i = 0; i < n; i++)
    {
      std::cout << "x(" << i + 1 << ") = ";
      std::cin >> a;
      std::cout << "y(" << i + 1 << ") = ";
      std::cin >> b;
      p[i].setPoint(a, b);
    }
  };
  void testInPolygon(point P)
  {
    point vectora, vectorb;
    float sum = 0;
    for (int i = 0; i < n - 1; i++)
    {
      vectora = setVector(p[i], P);
      vectorb = setVector(p[i + 1], P);
      sum = sum + calculateAngle(vectora, vectorb);
    }
    vectora = setVector(p[n - 1], P);
    vectorb = setVector(p[0], P);
    sum = sum + calculateAngle(vectora, vectorb);
    if (abs(sum - 2 * PI) <= 0.01)
      std::cout << "diem nam trong hinh";
    else
      std::cout << "diem nam ngoai hinh";
  };
  ~polygon()
  {
    delete[] p;
  };
};

int main()
{
  int m;
  float a, b;
  std::cout << "nhap so dinh cua da giac: ";
  std::cin >> m;
  polygon A(m);
  A.setPolygon();
  point P;
  std::cout << "nhap diem can xet: ";
  std::cout << "x = ";
  std::cin >> a;
  std::cout << "y = ";
  std::cin >> b;
  P.setPoint(a, b);
  A.testInPolygon(P);

  return 0;
}