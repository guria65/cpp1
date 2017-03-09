//#include<iostream>
//#include "pp4.cpp"
#include "comutil.h"
//#include <qlabel.h>
using namespace std;
/*
int main(){
dynamicVector<double> v(3,1.);
dynamicVector<double> u(3,1.5);
//u=2. * v;
printf("v:\n");
print(v);
printf("u:\n");
print(u);
printf("u+v:\n");
print(u+v);
printf("-v:\n");
print(-v);
//printf("u*v=%f\n",u*v);
return 0;
}
*/

int main(){
linkedList<double> c(3.);
c.append(5.);
c.append(6.);
c.dropFirstItem();
print(c);
return 0;
}
