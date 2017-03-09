#include<stdio.h>
void sw(float *a,float *b);
int main()
{
float a,b;
scanf("%f%f",&a,&b);
sw(&a,&b);
printf("%f,%f",a,b);
}
void sw(float *a,float *b)
{
float temp;
temp=*a;
*a=*b;
*b=temp;
}
