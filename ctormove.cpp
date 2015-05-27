#include <iostream>
#include <string>
#include <cstring>    // memcpy
#include <exception>  // throw std::bad_alloc
using namespace std;

void print(int &  i) { cout << i <<  " is a l-value\n"; }
void print(int && i) { cout << i <<  " is a r-value\n"; }

class X {
  const static int SIZE = 125000000;
  double *trouble;
  string name;
public:
 X()                   {cout<<"X ctor\n"; trouble = new double[SIZE];}  // ctor
 X(string n) :name(n)  {cout<<"X ctor:"<<name<<"\n"; trouble = new double[SIZE];}  // ctor
 void setname(string n) { name = n; }
 void printtrouble(string label)   { cout << label << ":" << name << ".trouble=" << (void*) trouble << "\n"; }
 ~X()                  {cout<<"X dtor:"<<name<<"\n"; if(trouble) delete [] trouble;}  // dtor
 X(X& rhs) { // copy ctor C++98/C++03
   cout<<"X copy ctor (deep copy of rhs)\n"; 
   trouble = new double[SIZE]; 
   memcpy(trouble, rhs.trouble, SIZE * sizeof(double)); // deep copy
   name = rhs.name;
 }
 X& operator= (const X& rhs) {  // assignment operator C++98/C++03
   cout << "assignment operator: " << name << "=" << rhs.name << "\n";  
   if(this != &rhs) {
     if(trouble) { delete [] trouble; }
     trouble = new double [ SIZE];
     memcpy(trouble, rhs.trouble, SIZE * sizeof(double));
     name = rhs.name;
   } else {
     cout << "somebody called assignment operator with same value, eg x=x\n";
   }
   return *this;
 }
 X(X&& rhs) { // move ctor C++11
   cout<<"X move ctor (move rhs pointer)\n"; 
   trouble = rhs.trouble;
   rhs.trouble = nullptr;
   name = rhs.name;
   rhs.name = "";
 }
 X&& operator= (const X&& rhs) {  // assignment operator C++11
   cout << "move assignment operator: " 
        << name << "=" << rhs.name 
        << " and " <<  rhs.name << " is an r-value"
        << "\n";  
   if(this != &rhs) {
     if(trouble) { delete [] trouble; }
     trouble = new double [ SIZE];
     memcpy(trouble, rhs.trouble, SIZE * sizeof(double));
     name = rhs.name;
   } else {
     cout << "somebody called assignment operator with same value, eg x=x\n";
   }
   return std::move(*this);
 }
 
};

X global("global");

int main(int argc, char** argv)
{
  cout << "main start\n";
  try {

    // 244 copy ctor + assignment operator
    int i;        // ctor;
    i = 27;       // assignment operator
    int j = i;    // copy ctor
    j = 19;       // assignment operator
    j = i;        // assignment operator

    // C++11 r-Value
    print(9);
    print(9+5);
    print(i);
    print(i+6);
    print(std::move(i));
    print(i);


    X x("x");    // ctor 244
    x.printtrouble("x");

    X y(x);      // copy ctor 244
    y.setname("y");
    x.printtrouble("x");
    y.printtrouble("y");

    x = y;       // assignment operator 244
    x.setname("x");
    x.printtrouble("x");
    y.printtrouble("y");

    x = std::move(y);  // move assignment operator C++11
    x.printtrouble("x");
    y.printtrouble("y");

    X z(std::move(x)); // move constructor C++11
    x.printtrouble("x");
    z.printtrouble("z");

  } catch(exception& e) {
    cerr << "It threw! :-( " << e.what() << "\n";
  }

  cout << "main finish\n";

  return 0;
}
