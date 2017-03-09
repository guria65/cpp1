#include<iostream.h>
#include<fstream.h>
//#include<stdlib.h>
class foo
{
    private:
    int m,n,i;
    double **a;
    public:
    void input()
    {
    ifstream fin("ppc.dat");
    fin>>m>>n;
    cout<<m<<n<<"\n";
    a=new double* [m];
     for(i=0;i<m;i++){a[i]=new double [n];}

    for(i=0;i<m;i++)
                    for(int j=0;j<n;j++)
            fin>>a[i][j];
            fin.close();
for(i=0;i<m;i++)
{for(int j=0;j<n;j++)
          { cout<<a[i][j]<<"\t";}
          cout<<"\n";}
     }

};
//void file::input()

