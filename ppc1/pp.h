#ifndef _PP_H_
#define _PP_H_
#include <iostream>
#include<fstream>
#include <cmath>
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <graphics.h>

#ifndef PI
#define PI 3.141592653589793238462643
#endif
#define sqr(a) ((a)*(a))
#define max(a,b) ((a)<(b)?(b):(a))
#define min(a,b) ((a)<(b)?(a):(b))
#define sign(a) ((a) >= 0.0 ? 1. : -1.)
static double temp;
#define swap(a,b) {temp=(a);(a)=(b);(b)=temp;}

#if defined(__STDC__) || defined(ANSI) || defined(NRANSI) /* ANSI */

using namespace std;

template<class T > class vec
{
    public:
    T* a;
    int n;
    vec(int n)
    {
        (*this).n=n;
      a=new double [n];
    }
        vec(const vec& d)
    {
      n=d.n;
      a=new double [n];
      while(d.n--)    a[d.n]=d.a[d.n];
    }
    ~vec(){}
};
template<class T > class mat
{
    public:
    int m;
    int n;
    double **a;
    mat(int m,int n)
    {
      (*this).m=m;
      (*this).n=n;
      a=new double* [m];
      for (i=0;i<m;i++)
        {
          a[i]=new double [n];
        }
    }

    mat(const mat&d)
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
    ~mat(){}
  };


#endif /*pp.h*/
