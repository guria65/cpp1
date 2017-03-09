
#include <iostream>
#include<fstream>
#include <cmath>
//namespace guria{
using namespace std;
//Base Account class
template<class T > class des
{
protected:
T sum,ss;
int i,n;
public:
T *x;
/*
virtual void rr()
{
    cin>>n;
    x =new double[n];
    for(i=0;i<n;i++)cin>>x[i]>>double f;
    }
    */
    virtual void rr()
{      ifstream fin("roo.dat");
T f;
   fin>>n;
    x =new T [n];
    for(int i=0;i<n;i++){
        fin>>x[i]>>f;
    }

                fin.close();

    }
void ww()
{
  cout<<"Given values are :"<<endl;
  for(int i=0;i<n;i++)cout<<x[i]<<endl;
}
/*
double mean()
{sum=0.0;
      for(i=0;i<n;i++)sum+=x[i];
      //cout<<"Mean = "<<sum/(double)n<<"\n";
      return sum/(double)n;
}
*/
T mean(T *x)
{sum=0.0;
      for(int i=0;i<n;i++)sum+=x[i];
      //cout<<"Mean = "<<sum/(double)n<<"\n";
      return sum/(double)n;
}
T var(T *x)
{ss=0.0;
        for(i=0;i<n;i++)ss+=x[i]*x[i];
      return (ss/(double)n-(mean(x)*mean(x)));
}
 template<class X>
  void swap(X &a, X &b)
 { X temp;
 temp = a; /* contents of pointer */
 a = b;
b = temp;
}
void sot()
{
            for(i=0;i<n-1;i++)
                    for(int j=i+1;j<n;j++)
                    {if(x[i]>x[j]) swap(x[i],x[j]);}

}
void med()
{
    if(n%2==0)cout<<"Median= "<<(x[(n/2)-1]+x[(n/2)])/2<<endl;
    else
    cout<<"Median ="<<x[((n+1)/2)-1]<<endl;
}
/*~des()
{cout<<"Desctructor\n";
    delete[] x;
    sum=0.0;
    prod=0.0;
       }
*/};

template<class T>
class BiVar : public des<T>{
protected:
//double sum,prod;
int i,j,n;
T prod;
public:
T *x;
T *y;
virtual void rr()
{      ifstream fin("roo.dat");

    fin>>n;
    x =new T [n];
    y=new T [n];
    for(i=0;i<n;i++)
    {
        fin>>x[i]>>y[i];

 }
     fin.close();
    }
void ww2()
{
  cout<<"Given values are :"<<endl;
  for(i=0;i<n;i++)cout<<x[i]<<"\t"<<y[i]<<endl;
}
double cov()
{ prod=0.0;
    for(i=0;i<n;i++) prod+=x[i]*y[i];
return ((prod/(double)n)-mean(x)*mean(y));
}
double corr()
{
return cov()/sqrt(var(x)*var(y));
}
void reg()
{
  if(corr()>=0)  cout<< "Reg. of Y on X is: Y="<<mean(y)-(cov()/var(x))*mean(x)<<" + "<< cov()/var(x)<<"*x\n";
else
cout<< "Reg. of Y on X is: Y="<<mean(y)-(cov()/var(x))*mean(x)<<" + "<< cov()/var(x)<<"*x\n";
}
/*virtual ~BiVar()
{
    delete [] y;
       }
#include <R.h>
#include <Rdefines.h>
extern "C" SEXP convolve2 (SEXP a , SEXP b )
 {
 int i , j , na , nb , nab ;
 double *xa , *xb , *xab ;
 SEXP ab ;
 PROTECT( a = AS_NUMERIC( a ) ) ;
 PROTECT( b = AS_NUMERIC( b ) ) ;
 na = legth( a ) ; nb = LENGTH( b ) ; nab = na + nb-1;
 PROTECT( ab = NEW_NUMERIC( nab ) ) ;
xa = NUMERIC_POINTER( a ) ; xb = NUMERIC_POINTER( b ) ;
 xab = NUMERIC_POINTER( ab ) ;
 for ( i = 0; i < nab ; i ++) xab [ i ] = 0 . 0 ;
 for ( i = 0; i < na ; i ++)
 for ( j = 0; j < nb ; j ++) xab [ i + j ] += xa [ i ] * xb [ j ] ;
 UNPROTECT( 3 ) ;
return ( ab ) ;
}*/
};
