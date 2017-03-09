#include <iostream>
#include<fstream>
#include <cmath>
//#include<Rcpp.h>
//#include "C:\Users\Guria\Documents\ppc1\pp.cpp"
using namespace std;
//typedef matrix<double> mat;
class matrix
{
    friend const matrix& operator+(const matrix&,const matrix&);
    friend const matrix& operator-(const matrix&,const matrix&);
    friend const matrix& operator*(const matrix&,const matrix&);
  public:
    int m;
    int n;
    double **a;
    matrix(int m,int n){
   (*this).m=m;(*this).n=n;
  a=new double* [m];
for(i=0;i<m;i++){a[i]=new double [n];}
}
    matrix(const matrix&d){
    m=d.m;
    n=d.n;
    a=new double* [m];
for(i=0;i<m;i++){a[i]=new double [n];}
  for(i=0;i<d.m;i++)
for(j=0;j<d.n;j++)
    a[i][j]=d.a[i][j];
}
    ~matrix(){}
matrix input();
void output();
matrix& addKTime(double,const matrix&);
matrix sca_mult(double g);
matrix prod(const matrix& d);
matrix pdsub(int r);
matrix iden(int n);
matrix inv();
matrix tran();
matrix getCol(int);
matrix& setCol(int,double);
matrix cAug(matrix&);
double& ele();
const matrix& operator=(const matrix&);
const matrix& operator+=(const matrix&);
const matrix& operator-=(const matrix&);
const matrix& operator*=(const double&);

private:
    int i,j,k;
};

//typedef matrix mat;
/*int main()
{
    mat xx(23,3);
    cout<<"Given matrix is:\n";
     xx.input();
xx.setCol(0,1.);
mat y=xx.getCol(2);
mat x=xx.getCol(0);
mat z=xx.getCol(1);
x=x.cAug(z);
//x.output();
mat x1=x;
   mat xt=x1.tran();
mat s=xt.prod(x);
mat si=s.inv();
mat b0=si.prod(xt.prod(y));
  b0.output();
long double p,l=0.0;
s.output();

mat vb=s;
vb.setCol(0,0.0);
vb.setCol(1,0.0);
cout<<"\n";
vb.output();
cout<<"\n";
for(int i=0;i<y.m;i++){
l-=log(1+exp((1-2*y.tran().getCol(i).ele())*(xt.getCol(i).tran().prod(b0).ele())));
  // cout<< l<<"\n";
p=exp((xt.getCol(i).tran().prod(b0).ele()))/(1+exp((xt.getCol(i).tran().prod(b0).ele())));
cout<<p<<"\n";
}
return 0;
}*/
/*matrix::matrix(int m,int n){
   (*this).m=m;(*this).n=n;
  a=new double* [m];
for(i=0;i<m;i++){a[i]=new double [n];}
}
//template<class T>
matrix::matrix(const matrix& d ){
    m=d.m;
    n=d.n;
    a=new double* [m];
for(i=0;i<m;i++){a[i]=new double [n];}
  for(i=0;i<d.m;i++)
for(j=0;j<d.n;j++)
    a[i][j]=d.a[i][j];
}
//template<class T>
matrix::~matrix()
{
   // for(i=0;i<nr;i++){delete [] a[i];}
  //  delete [] a;
 // cout<<"destruc\n";
}*/
//template<class T>
matrix matrix::input()
    {

      ifstream fin("ppc.dat");
    fin>>m>>n;
//    a=new double* [m];
//for(i=0;i<m;i++){a[i]=new double [n];}
    for(i=0;i<m;i++)
                    for(j=0;j<n;j++)
            fin>>a[i][j];
            //for(i=0;i<p;i++)
             //       for(j=0;j<n;j++)
           // fin>>b[i][j];
            fin.close();
        return *this;
    }
   // template<class T>
void matrix::output()
{
    for(i=0;i<m;i++)
{for(j=0;j<n;j++)
          { cout<<a[i][j]<<"\t";}
          cout<<"\n";}
}
//template<class T>
matrix& matrix::addKTime(double k,const matrix& d)
{if((*this).m==d.m && (*this).n==d.n)
   {  for(i=0;i<m;i++)
for(j=0;j<n;j++)
{a[i][j]+=k*d.a[i][j];
}}
return *this;
}
//template<class T>
matrix matrix::sca_mult(double g)
{
  for(i=0;i<m;i++)
{for(j=0;j<n;j++)
a[i][j]=a[i][j]*g;
}
return *this;
}
//template<class T>
matrix matrix::prod(const matrix& d)
{
    matrix h(m,n);
    h.m=(*this).m;
    h.n=d.n;
  for(i=0;i<m;i++)
{for(j=0;j<n;j++)
{
    h.a[i][j]=0.0;
    for(k=0;k<n;k++)
{h.a[i][j]+=a[i][k]*d.a[k][j];}
}
}
return h;
}
//template<class T>
matrix matrix::pdsub(int r)
{
    matrix c(m,n);
    c.m=r;
    c.n=r;
for(i=0;i<r;i++)
for(j=0;j<r;j++)
c.a[i][j]=a[i][j];
return c;
}
//template<class T>
matrix matrix::iden(int h)
{m=h;
n=h;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
    i==j ? a[i][j]=1.0: a[i][j]=0.0;
    return *this;
}
//template<class T>
matrix matrix::inv()
{double g;
    matrix c(m,n);
   // c.m=m;
   // c.n=n;
   matrix d=*this;
c.iden(n);
for(i=0;i<m;i++)
{g=d.a[i][i];
        for(j=0;j<n;j++)
        {
            d.a[i][j]=d.a[i][j]/g;
            c.a[i][j]=c.a[i][j]/g;
        }

                    for(k=0;k<m;k++)
                    {
                        if(k!=i){
                            double y=d.a[k][i];
                            for(j=0;j<n;j++)
                {
                        d.a[k][j]-=d.a[i][j]*y;
                        c.a[k][j]-=c.a[i][j]*y;
                        }
                    }
                            }
                            //c.output();
}
return c;
}
//template<class T>
matrix matrix::tran()
{
        double* temp;
        temp=new double [m*n];
        for(i=0;i<m;i++)
    for(j=0;j<n;j++)
    temp[i*n+j]=a[i][j];
 matrix d((*this).n,(*this).m);
 for(i=0;i<d.m;i++)
   for(j=0;j<d.n;j++)
    d.a[i][j]=temp[j*d.m+i];
    return d;
    }
//template<class T>
matrix matrix::getCol(int k)
{matrix d((*this).m,1);
    for(i=0;i<(*this).m;i++)
d.a[i][0]=a[i][k];
      return d;
}
//template<class T>
matrix& matrix::setCol(int k,double g)
{
    for(i=0;i<(*this).m;i++)
a[i][k]=g;
      return *this;
}
//template<class T>
matrix matrix::cAug(matrix& d)
{matrix h(m,n+d.n);
    for(i=0;i<m;i++)
    {for(j=0;j<n;j++)
h.a[i][j]=a[i][j];
for(int k=0;k<d.n;k++)
h.a[i][n+k]=d.a[i][k];
}
h.m=m;
h.n=n+d.n;
      return h;
}
//template<class T>
double& matrix::ele()
{
    return (*this).a[0][0];
}


//template<class T>
const matrix& matrix::operator=(const matrix&d){
    m=d.m;
    n=d.n;
for( i = 0; i < d.m; i++)
for( j = 0; j < d.n; j++)
a[i][j] = d.a[i][j];
return *this;
}
//template<class T>
const matrix& matrix::operator+=(const matrix&d){
    m=d.m;
    n=d.n;
for( i = 0; i < d.m; i++)
for( j = 0; j < d.n; j++)
a[i][j] += d.a[i][j];
return *this;
}
//template<class T>
const matrix& matrix::operator-=(const matrix&d){
    m=d.m;
    n=d.n;
for( i = 0; i < d.m; i++)
for( j = 0; j < d.n; j++)
a[i][j] -= d.a[i][j];
return *this;
}
//template<class T>
const matrix& matrix:: operator*=(const double& g){
for( i = 0; i < m; i++)
for( j = 0; j < n; j++)
a[i][j] *=g;
return *this;
}
//template<class T>
const matrix& operator+(const matrix&d,const matrix&dd){
     matrix h=d;
     h+=dd;
return h;
}
//template<class T>
const matrix& operator-(const matrix& d,const matrix& dd){
matrix h=d;
     h-=dd;
return h;
}

//template<class T>
const matrix& operator*(const matrix&d,const matrix&dd){
matrix h(d.m,dd.n);
   for(int i=0;i<h.m;i++)
{for(int j=0;j<d.n;j++)
{
    h.a[i][j]=0.0;
    for(int k=0;k<d.m;k++)
h.a[i][j]+=h.a[i][k]*d.a[k][j];

}
}
return h;
}


