#include<iostream>
using namespace std;
template<class T> class dynamicVector{
protected:
int dimension;
T* component;
public:
dynamicVector(int, const T&);
dynamicVector(const dynamicVector&);
const dynamicVector& operator=(const dynamicVector&);
const dynamicVector& operator=(const T&);
~dynamicVector(){
delete [] component;
} // destructor
int dim() const{
return dimension;
} // return the dimension
T& operator()(int i){
return component[i];
} // read/write ith component
const T& operator[](int i) const{
return component[i];
} // read only ith component
const dynamicVector& operator+=(const dynamicVector&);
const dynamicVector& operator-=(const dynamicVector&);
const dynamicVector& operator*=(const T&);
const dynamicVector& operator/=(const T&);
};
template<class T>
dynamicVector<T>::dynamicVector(
int dim = 0, const T& a = 0)
: dimension(dim), component(dim ? new T[dim] : 0){
for(int i = 0; i < dim; i++)
component[i] = a;
} // constructor
template<class T>
dynamicVector<T>::dynamicVector(const dynamicVector<T>& v)
: dimension(v.dimension),
component(v.dimension ? new T[v.dimension] : 0){
for(int i = 0; i < v.dimension; i++)
component[i] = v.component[i];
} // copy constructor
template<class T>
const dynamicVector<T>&
dynamicVector<T>::operator=(const dynamicVector<T>& v){
if(this != &v){
    if(dimension != v.dimension){
delete [] component;
component = new T[v.dimension];
}
 for(int i = 0; i < v.dimension; i++)
component[i] = v.component[i];
dimension = v.dimension;
}
return *this;
} // assignment operator
 template<class T>
const dynamicVector<T>&
dynamicVector<T>::operator=(const T& a){
for(int i = 0; i < dimension; i++)
component[i] = a;
return *this;
} // assignment operator with a scalar argument
template<class T>
const dynamicVector<T>&
dynamicVector<T>::operator+=( const dynamicVector<T>&v){
for(int i = 0; i < dimension; i++)
component[i] += v[i];
return *this;
}// adding a dynamicVector to the current one
template<class T>
const dynamicVector<T>
operator+(const dynamicVector<T>&u,
const dynamicVector<T>&v){
return dynamicVector<T>(u) += v;
} // dynamicVector plus dynamicVector
template<class T>
const dynamicVector<T>
operator-(const dynamicVector<T>&u){
return dynamicVector<T>(u) *= -1.;
} // negative of a dynamicVector
template<class T>
void print(const dynamicVector<T>&v){
printf("(");
for(int i = 0;i < v.dim(); i++){
printf("v[%d]=",i);
printf("%f",v[i]);
}
printf(")\n");
} // printing a dynamicVector
template<class T>
const dynamicVector<T>&
dynamicVector<T>::operator*=( const T&k){
for(int i = 0; i < dimension; i++)
component[i] *= k;
return *this;
}// adding a dynamicVector to the current one
template<class T>
const dynamicVector<T>&
dynamicVector<T>::operator/=( const T& k){
for(int i = 0; i < dimension; i++)
component[i] /= k;
return *this;
}// adding a dynamicVector to the current one
//****************************************
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
} // printing a list
//**************************************
template<class T> class linkedList{
protected:
T item;
linkedList* next;
public:
linkedList():next(0){
} // default constructor
linkedList(T&t, linkedList* N=0)
: item(t),next(N){
} // constructor
const T& operator()() const{
return item;
} // read item field
const linkedList* readNext() const{
return next;
} // read next
const linkedList& operator=(const linkedList&);
linkedList(const linkedList&l):item(l()),
next(l.next ? new linkedList(*l.next):0){
} // copy constructor
~linkedList(){
delete next;
next = 0;
} // destructor
linkedList& last(){
return next ? next->last() : *this;
} // last item
int length() const{
return next ? next->length() + 1 : 1;
} // number of items
void append(T&t){
last().next = new linkedList(t);
} // append a new item
void insertNextItem(T&t){
next = new linkedList(t,next);
} // insert item in the second place
void insertFirstItem(T&t){
    next = new linkedList(item,next);
    item = t;
} // insert a new item at the beginning
void dropNextItem();
void dropFirstItem();
const linkedList& operator+=(linkedList&);
linkedList& order(int);
};
template<class T>
const linkedList<T>&
linkedList<T>::operator=(const linkedList<T>&L){
    if(this != &L){
        item = L();
        if(next){
if(L.next)
*next = *L.next;
else{
delete next;
next = 0;
}
}
else
if(L.next)next = new linkedList(*L.next);
}
return *this;
} // assignment operator
template<class T>
void linkedList<T>::dropNextItem(){
    if(next){
        if(next->next){
            linkedList<T>* keep = next;
            next = next->next;
            keep->item.~T();
}
else{
delete next;
next = 0;
}
}
else
printf("error: cannot drop nonexisting next item\n");
} // drop the second item from the linked list
template<class T>
void linkedList<T>::dropFirstItem(){
    if(next){
        item = next->item;
        dropNextItem();
}
else
printf("error: cannot drop the only item.\n");
} // drop the first item in the linked list
template<class T>
void print(const linkedList<T>&l){
printf("item:\n");
print(l());
if(l.readNext())print(*l.readNext());
} // print a linked list recursively
template<class T>
const linkedList<T>&
linkedList<T>::operator+=(linkedList<T>&L){
linkedList<T>* runner = this;
linkedList<T>* Lrunner = &L;
if(L.item < item){
insertFirstItem(L.item);
Lrunner = L.next;
}
for(; runner->next; runner=runner->next){
    if(Lrunner&&(Lrunner->item == runner->item)){
runner->item += Lrunner->item;
Lrunner = Lrunner->next;
}
for(; Lrunner&&(Lrunner->item < runner->next->item);
Lrunner = Lrunner->next){
runner->insertNextItem(Lrunner->item);
runner = runner->next;
}
}
if(Lrunner&&(Lrunner->item == runner->item)){
runner->item += Lrunner->item;
Lrunner = Lrunner->next;
}
if(Lrunner)
runner->next = new linkedList<T>(*Lrunner);
return *this;
} // merge two linked lists while preserving order
template<class T>
linkedList<T>&
linkedList<T>::order(int length){
    if(length>1){
linkedList<T>* runner = this;
for(int i=0; i<length/2-1; i++)
runner = runner->next;
linkedList<T>* second = runner->next;
runner->next = 0;
order(length/2);
*this += second->order(length-length/2);
}
return *this;
} // order a disordered linked list
/*
template<class T>
class stack : public linkedList<T>{
int empty;
public:
stack():empty(1){
} // default constructor
int isEmpty() const{
return empty;
}
} // check whether the stack is empty or not

void push(const T&t){
if(empty){
item = t;
empty = 0;
}
else{
T insert = t;
insertFirstItem(insert);
}
} // push an item at the top of the stack
const T pop(){
if(empty)
printf("error: no item to pop\n");
else
if(next){
const T out = item;
dropFirstItem();
return out;
}
else{
empty = 1;
return item;
}// pop the top item out of the stack
};
*/
