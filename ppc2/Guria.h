#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include<fstream>
#include <cmath>
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <graphics.h>

using namespace std;
class vector
  {
 public:
    int n;
    double *a;
    vector(int n)
    {
      (*this).n=n;
      a=new double [n];
       }
    vector(const vector& d)
    {
      n=d.n;
      a=new double [n];
         or (i=0;i<d.n;i++)
          a[i]=d.a[i];
    }
    ~vector(){}
    vector& ins(double ,int);
    vector& del(int );
    matrix& operator=(const matrix&);
    matrix& operator+=(const matrix&);
    matrix& operator-=(const matrix&);
    matrix& operator*=(const double&);
   // matrix& operator*=(const matrix&);
  private:
    int i,j,k;
  };

#endif //MATRIX_H
