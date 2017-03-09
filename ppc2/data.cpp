#include <iostream>
#include <cstring>
using namespace std;

template<class T> class list{
protected:
int number;
T** item;
public:
list(int n=0):number(n), item(n ? new T*[n]:0){
} // constructor
list(int n, const T&t)
: number(n), item(n ? new T*[n] : 0){
for(int i=0; i<number; i++)
item[i] = new T(t);
} // constructor with a T argument
list(const list<T>&);
const list<T>& operator=(const list<T>&);
~list(){
for(int i=0; i<number; i++)
delete item[i];
delete [] item;
} // destructor
int size() const{
return number;
} // size of list
T& operator()(int i){
if(item[i])return *(item[i]);
} // read/write ith item
const T& operator[](int i)const{
if(item[i])return *(item[i]);
} // read only ith item
};
template<class T>
list<T>::list(const list<T>&l):number(l.number),
item(l.number ? new T*[l.number] : 0){
for(int i=0; i<l.number; i++)
if(l.item[i])
item[i] = new T(*l.item[i]);
else
item[i] = 0;
} // copy constructor
template<class T>
const list<T>&
list<T>::operator=(const list<T>& l){
    if(this != &l){
        if(number != l.number){
delete [] item;
item = new T*[l.number];
}
if(number != l.number){
delete [] item;
item = new T*[l.number];
}
for(int i = 0; i < l.number; i++)
if(l.item[i])
item[i] = new T(*l.item[i]);
else
item[i] = 0;
number = l.number;
}
return *this;
} // assignment operator
template<class T>
void print(const list<T>&l){
for(int i=0; i<l.size(); i++){
printf("i=%d:\n",i);
print(l[i]);
}
}
