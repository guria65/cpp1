// VariableSize - output the size of each type of
// variable
 #include <iostream>
using namespace std;
//Function prototype
double calcAvg(double [], int);

int main(int nNumberofArgs, char* pszArgs[])
{
int n=5;
double *x, avg;
//Function call
x[0]=1;x[1]=2;x[2]=3;x[3]=4;x[4]=5;
calcAvg(x,n);
cout<<avg<<endl;
//system("PAUSE");
return 0;
}
double calcAvg(double score[], int SIZE)
{

int index; //Counter set to index array.
double total = 0; //Accumulator.
double avg = 0; //Variable to hold average score.

//Index through array to get test scores
for(index = 0; index < 10; index++)//Note 10 is the actual size of array


//Variable to hold total of all scores.
total += score[index];
avg = (total/SIZE);
return avg;
}
