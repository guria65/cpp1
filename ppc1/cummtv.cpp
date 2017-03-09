#include<iostream>
using namespace std;
int main()
{
   int i,n;
   double *x,sum=0.0;
   cin>>n;
   x=new double[n];
   for(i=0;i<n;i++) cin>>x[i];
   for(i=0;i<n;i++) cout<<x[i]<<"\n";
   for(i=0;i<n;i++){sum+=x[i];
   cout<<sum<<"\n";}
   return 0;
}
