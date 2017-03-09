#include <iostream>
#include<fstream>
#include <cmath>
using namespace std;
class matrix
{
    private:
    int i,j,k;
    public:
    int m,n,p;
    double **a,**b,**c;
    matrix()
{
c=new double* [m];
     for(i=0;i<m;i++){c[i]=new double [n];}
}
    double input()
    {
    ifstream fin("ppc.dat");
    fin>>m>>p>>n;
    //cout<<m<<n<<"\n";
    a=new double* [m];
     for(i=0;i<m;i++){a[i]=new double [n];}
    /*b=new double* [p];
     for(i=0;i<m;i++){b[i]=new double [n];}*/

    for(i=0;i<m;i++)
                    for(j=0;j<p;j++)
            fin>>a[i][j];
            /*for(i=0;i<p;i++)
                    for(j=0;j<n;j++)
            fin>>b[i][j];*/
            fin.close();
            return **a;
            cout<<"First matrix is:\n";
            output(a,m,n);
           /* cout<<"Second matrix is:\n";
            output(b,m,n);
            **c=add(a,b,m,n);
            cout<<"Sum matrix is:\n";
            output(c,m,n);
            **c=prod(a,b,m,p,n);
            cout<<"Product matrix is:\n";
            output(c,m,n);*/


      }
double add(double **a,double **b,int m,int n)
{
    c=new double* [m];
     for(i=0;i<m;i++){c[i]=new double [n];}

  for(i=0;i<m;i++)
{for(j=0;j<n;j++)
c[i][j]=a[i][j]+b[i][j];
}
return **c;
}
double prod(double **a,double **b,int m,int p,int n)
{
    c=new double* [m];
     for(i=0;i<m;i++){c[i]=new double [n];}

  for(i=0;i<m;i++)
{for(j=0;j<n;j++)
{
    c[i][j]=0.0;
    for(k=0;k<p;k++)
c[i][j]+=a[i][k]*b[k][j];
}
}
return **c;
}

void output(double **a,int m,int n)
{
    for(i=0;i<m;i++)
{for(j=0;j<n;j++)
          { cout<<a[i][j]<<"\t";}
          cout<<"\n";}
}
~matrix()
{
    for(i=0;i<m;i++){delete[] a[i];}
    delete []a;
    for(i=0;i<p;i++){delete[] b[i];}
    delete []b;
    for(i=0;i<m;i++){delete[] c[i];}
    delete []c;
}
};
