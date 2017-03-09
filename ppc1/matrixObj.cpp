#include <iostream>
#include <fstream>
class matrix
{
    private:
    int i,j;
   public:
    int nr;
    int nc;
    double **a;
   };
class matrix1
{
    private:
    int i,j;
        matrix d;
    public:
matrix input();
void output(matrix);
};
matrix matrix1::input()
    {
        ifstream fin("ppc.dat");
    fin>>d.nr>>d.nc;
d.a=new double* [d.nr];
     for(i=0;i<d.nr;i++){d.a[i]=new double [d.nc];}

    for(i=0;i<d.nr;i++)
                    for(j=0;j<d.nc;j++)
            fin>>d.a[i][j];
            //for(i=0;i<p;i++)
             //       for(j=0;j<n;j++)
           // fin>>b[i][j];
            fin.close();
            return d;
    }
void matrix1::output(matrix d)
{
    for(i=0;i<d.nr;i++)
{for(j=0;j<d.nc;j++)
          { cout<<d.a[i][j]<<"\t";}
          cout<<"\n";}
}
