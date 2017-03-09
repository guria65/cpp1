#include<iostream>
class prog18
{
    private:
    int i;
    double b[3];
    public:
    void array();
};
void prog18::array()
    {
        b[0]=0.5;b[1]=1.5;b[2]=-2.5;
        cout<<"\n Components are:"<<endl;
        for(i=0;i<3;i++)
        cout<<(2*(*(b+i)))<<endl;
    }
