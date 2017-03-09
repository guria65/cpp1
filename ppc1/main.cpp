//#include<iostream>
#include "pp.cpp"
//#include<Rcpp.h>
//#include "matrix.cpp"
//#include "matrixObj.cpp"
using namespace std;
int main(int argc, char *argv[])
{

    des<double> d;
   d.rr();
     d.ww();
     cout<<"Mean is="<<d.mean(d.x)<<"\n";
    cout<<"Variance is = "<<d.var(d.x)<<"\n";
    d.sot();
    d.med();
    cout<<"ordered values are";
    d.ww();
/*
BiVar<double*, int> a;
  a.rr();
  a.ww2();
  cout<<"Mean of Y is=  "<<a.mean(a.y)<<"\n";
  cout<<"Mean of X is=  "<<a.mean(a.x)<<"\n";
  cout<<"Variance of Y is=  "<<a.var(a.y)<<"\n";
  cout<<"Variance of X is=  "<<a.var(a.x)<<"\n";
  cout<<"Covariance of X & Y is=  "<<a.cov()<<"\n";
  cout<<"Correlation of X & Y is=  "<<a.corr()<<"\n";
    a.reg();
*/
    return 0;
}

