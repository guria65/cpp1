//#include "pp2.cpp"
//#include "C:\Users\Guria\Documents\ppc1\pp.cpp"
//using namespace std;
#include <iostream>
#include<fstream>
#include <cmath>
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
//#include<Rcpp.h>
//#include "C:\Users\Guria\Documents\ppc1\pp.cpp"
using namespace std;
class matrix
  {
    friend double det(matrix&);
    friend matrix invert(const matrix&);
    friend matrix qr(matrix&);
    friend matrix t(const matrix&);
    friend matrix operator+(const matrix&,const matrix&);
    friend matrix operator-(const matrix&,const matrix&);
    friend matrix operator*(const matrix&,const matrix&);
    friend double num(const matrix&);
    friend void print  (const matrix&);
    friend matrix accept(int,int);

  public:
    int m;
    int n;
    double **a;
    matrix(int m,int n)
    {
      (*this).m=m;
      (*this).n=n;
      a=new double* [m];
      for (i=0;i<m;i++)
        {
          a[i]=new double [n];
        }
    }
    matrix(const matrix&d)
    {
      m=d.m;
      n=d.n;
      a=new double* [m];
      for (i=0;i<m;i++)
        {
          a[i]=new double [n];
        }
      for (i=0;i<d.m;i++)
        for (j=0;j<d.n;j++)
          a[i][j]=d.a[i][j];
    }
    ~matrix(){}
    matrix input();
//void output();
//void output(const matrix&);
//matrix& addKTime(double,const matrix&);
//matrix sca_mult(double g);
//matrix prod(const matrix& d);
//matrix pdsub(int r);
    matrix iden(int n);
//matrix inv();
//matrix tran();
    matrix getCol(int);
    matrix delCol(int);
    matrix setCol(int,double);
    matrix cAug(const matrix&);
    matrix minor(int, int);
//double& ele(matrix&,int,int);
    matrix& operator=(const matrix&);
    matrix& operator+=(const matrix&);
    matrix& operator-=(const matrix&);
    matrix& operator*=(const double&);

  private:
    int i,j,k;
  };

matrix matrix::input()
{
  ifstream fin("koo.dat");
  fin>>m>>n;
//    a=new double* [m];
//for(i=0;i<m;i++){a[i]=new double [n];}
  (*this).m=m;
  (*this).n=n;
  for (j=0;j<(*this).n;j++)
    for (i=0;i<(*this).m;i++)
      fin>>(*this).a[i][j];
  //for(i=0;i<p;i++)
  //       for(j=0;j<n;j++)
  // fin>>b[i][j];
  fin.close();
  return *this;
}
void print(const matrix&d)
{
  //cout.precision(4);
//cout.setf(ios::fixed);
//#include<iomanip>//cout.setf(ios::showpoint | ios::showpos, ios::showpoint);
  for (int i=0;i<d.m;i++)
    {
      for (int j=0;j<d.n;j++)
        {
          cout <<d.a[i][j]<<"\t";
        }
      cout<<"\n";
    }


  ofstream TextFile1;
  TextFile1.open ("CancerImageFeature.txt");

  for (int i=0;i<d.m;i++)
    {
      for (int j=0;j<d.n;j++)
        TextFile1 << d.a[i][j]<<'\t';
      TextFile1<<'\n';
    }

  TextFile1.close( );
}
matrix matrix::iden(int h)
{
  m=h;
  n=h;
  for (i=0;i<n;i++)
    for (j=0;j<n;j++)
      i==j ? a[i][j]=1.0: a[i][j]=0.0;
  return *this;
}
matrix matrix::getCol(int k)
{
  matrix d((*this).m,1);
  for (i=0;i<d.m;i++)
    d.a[i][0]=(*this).a[i][k];
  return d;
}
matrix matrix::delCol(int k)
{
  matrix d((*this).m,(*this).n-1);
  for (i=0;i<d.m;i++)
    {
      for (j=0;j<d.n;j++)
        {
          if (j<k)
            d.a[i][j]=(*this).a[i][j];
          else
            d.a[i][j]=(*this).a[i][j+1];
        }
    }
  return d;
}
//template<class T>
matrix matrix::setCol(int k,double g)
{
  for (i=0;i<(*this).m;i++)
    a[i][k]=g;
  return *this;
}
//template<class T>
matrix matrix::cAug(const matrix& d)
{
  matrix h((*this).m,(*this).n+d.n);
  for (i=0;i<(*this).m;i++)
    {
      for (j=0;j<(*this).n;j++)
        h.a[i][j]=(*this).a[i][j];
      for (int k=0;k<d.n;k++)
        h.a[i][n+k]=d.a[i][k];
    }
  return h;
}
//template<class T>
//double& matrix::ele(matrix&d,int i=0,int j=0){
//    return d.a[i][j];}
double num(const matrix& d)
{
  if (d.m==1&&d.n==1)
    return d.a[0][0];
  else
    return 0;
}

//template<class T>
matrix& matrix::operator=(const matrix&d)
{
  m=d.m;
  n=d.n;
  for ( i = 0; i < d.m; i++)
    for ( j = 0; j < d.n; j++)
      a[i][j] = d.a[i][j];
  return *this;
}
//template<class T>
matrix& matrix::operator+=(const matrix&d)
{
  m=d.m;
  n=d.n;
  for ( i = 0; i < d.m; i++)
    for ( j = 0; j < d.n; j++)
      a[i][j] += d.a[i][j];
  return *this;
}
//template<class T>
matrix& matrix::operator-=(const matrix&d)
{
  m=d.m;
  n=d.n;
  for ( i = 0; i < d.m; i++)
    for ( j = 0; j < d.n; j++)
      a[i][j] -= d.a[i][j];
  return *this;
}
//template<class T>
matrix& matrix:: operator*=(const double& g)
{
  for ( i = 0; i < m; i++)
    for ( j = 0; j < n; j++)
      a[i][j] *=g;
  return *this;
}
//template<class T>
matrix operator+(const matrix&d,const matrix&dd)
{
  matrix h=d;
  h+=dd;
  return h;
}
//template<class T>
matrix operator-(const matrix& d,const matrix& dd)
{
  matrix h=d;
  h-=dd;
  return h;
}

//template<class T>
matrix operator*(const matrix&d,const matrix&dd)
{
  matrix h(d.m,dd.n);
  for (int i=0;i<d.m;i++)
    {
      for (int j=0;j<dd.n;j++)
        {
          h.a[i][j]=0.0;
          for (int k=0;k<d.n;k++)
            h.a[i][j]+=d.a[i][k]*dd.a[k][j];

        }
    }
  return h;
}
matrix matrix::minor(int i,int j)
{
  matrix d=*this;
  return t(t(d.delCol(j)).delCol(i));
}
double det(matrix& d)
{matrix c(d.m-1,d.n-1);
  double val=0.;
  if (d.n==1)val=d.a[0][0];
  else
    {
     for (int i=0;i<d.n;i++)
{c=d.minor(0,i);
     val+=(pow((double)-1,i)*(d.a[0][i])*det(c));}
   }

  return val;
}
matrix inverse(matrix& d)
{matrix c(d.m,d.n);
    if(d.n==1) c.a[0][0]=1/d.a[0][0];
    else
    for(int i=0;i<d.n;i++)
    {for(int j=0;j<d.n;j++)
    {matrix dd=d.minor(j,i);
    c.a[i][j]=pow(-1.,(i+j))*(det(dd)/det(d));
    }
    }
    return c;
    }
matrix accept(int n=23,int p=3)
{
  ifstream TextFile;
  int count=0;
  matrix d(n,p);
//cout.precision(4);
//cout.setf(ios::fixed);
  d.m=n;
  d.n=p;
//x=new double* [n];
//for(int i=0;i<n;i++) x[i]=new double [p];
  TextFile.open ("C:/Users/Guria/Documents/data/health.dat");
  while (!TextFile.eof()&&((count++)<n*p))
    {
        for (int j=0;j<d.n;j++)
      for (int i=0;i<d.m;i++)
          TextFile >> d.a[i][j];
    }
  TextFile.close( );
  return d;
}


int main()
{
  // matrix xx(400,7);
  // cout<<"Given matrix is:\n";
//xx.input();
  matrix xx=accept(30,6);
  print(xx);
  /*//print(xx.minor(0,1));
  cout<<det(xx);
  cout<<"\n";
  print(inverse(xx));
    cout<<"\n";
print(invert(xx));

  xx.setCol(0,1.);
  matrix y=xx.getCol(2);
  matrix x=xx.getCol(0).cAug(xx.getCol(1));
  //matrix x=xx.getCol(0).cAug(xx.getCol(3)).cAug(xx.getCol(4)).cAug(xx.getCol(5));
  matrix s(x.n,x.n),xt(x.n,x.m),b0(x.n,1);
  b0=invert(t(x)*x)*t(x)*y;
  print(b0);
  matrix b1(x.n,1),b(x.n,1),w(x.n,x.n),f(x.n,1),p(y.m,1);
  w.setCol(0,0.0);
  w.setCol(1,0.0);
  //print(w);
  double del;//l=0.0;
  int ic=0;
  do
  {
      for (int i=0;i<y.m;i++)
      {
  //l+=log(1+exp((1-2*num(t(y).getCol(i)))*num(t(t(x).getCol(i))*b0)));
  //  cout<< l<<"\n";
          p.a[i][0]=exp(num(t(t(x).getCol(i))*b0))/(1+exp(num(t(t(x).getCol(i))*b0)));
          w=w+(((t(x).getCol(i))*=(p.a[i][0]*(1-p.a[i][0])))*t(t(x).getCol(i)));
  //print(w);
      }
  //print(w);
      f=invert(w)*(t(x)*(y-p));
      b0+=f;
      del=num(t(f)*f);

  cout<<"Iteration No."<<ic<<"&  ";
  print(t(b0));
  }
  while ( del>.5||ic++<1000);
  print(b0);
  */
  return 0;
}
//template<typename X>
matrix t(const matrix& d)
{
  double* temp;
  temp=new double [d.m*d.n];
  for (int i=0;i<d.m;i++)
    for (int j=0;j<d.n;j++)
      temp[i*d.n+j]=d.a[i][j];
  matrix h(d.n,d.m);
  for (int i=0;i<h.m;i++)
    for (int j=0;j<h.n;j++)
      h.a[i][j]=temp[j*h.m+i];
  return h;
}

matrix invert(const matrix& d)
{
  double g;
  if (d.m==d.n)
    {
      matrix c(d.m,d.n);
      // c.m=m;
      // c.n=n;
//  matrix d=*this;
      c.iden(d.n);
      for (int i=0;i<d.n;i++)
        {
          g=d.a[i][i];
          for (int j=0;j<d.n;j++)
            {
              d.a[i][j]=d.a[i][j]/g;
              c.a[i][j]=c.a[i][j]/g;
            }

          for (int k=0;k<d.n;k++)
            {
              if (k!=i)
                {
                  double y=d.a[k][i];
                  for (int j=0;j<d.n;j++)
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
  else
//return null;
    exit(1);
}
matrix qr( matrix& d)
{
  matrix q(d.m,d.n),r(d.n,d.n);
  for (int i=0;i<d.n;i++)
    for (int j=0;j<d.n;j++)
      r.a[i][j]=0.;

  matrix q1(d.m,1),dd=d;
  for (int i=0;i<d.n;i++)
    {
      q1=dd.getCol(0);

      r.a[i][i]=sqrt(num(t(q1)*q1));
      q1*=(1/r.a[i][i]);
      //print(q1);
      for (int j=i+1;j<d.n;j++)
        r.a[i][j]=num(t(q1)*dd.getCol(j));
      if (i==0)q=q1;
      else
        q=q.cAug(q1);
      dd=dd.delCol(0)-q*(t(q)*dd.delCol(0));
    }
  return t(q).cAug(r);
}
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
/*void matrix::output()
{
    for(i=0;i<m;i++)
{for(j=0;j<n;j++)
          { cout<<a[i][j]<<"\t";}
          cout<<"\n";}
}*/
/*
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
}*/
//template<class T>
/*
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
    }*/
//template<class T>
