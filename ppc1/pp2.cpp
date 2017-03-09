#include <iostream>
using namespace std;
void duplicate (double& a, double& b, double& c)
{
a*=2;
b*=2;
c*=2;
}
int main ()
{
//int x=1, y=3, z=7;
double x=1.09, y=3.54, z=7.006;

duplicate (x, y, z);
cout << "x=" << x << ", y=" << y << ", z=" << z;
return 0;
}
