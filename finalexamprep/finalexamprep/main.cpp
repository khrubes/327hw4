#include <iostream>
using namespace std;

class A {
    public:
    A() {
        value = 10;
        cout << "A ctr" << endl;
    }
    ~A() {cout << "A dstr" << endl; }
    int value;
    A operator+(int value){
        this->value+=value;
        return *this;
    }
};

class B : public A {
    public:
        B(){ cout << "B ctr" << endl; }
        ~B() {cout << "B dstr" << endl; }
};

int main(int argc, const char * argv[]) {
    A a;
    B* aptr = new B(); // need to call delete with every new
    
    a = a + 99;
    cout << a.value << endl;
    
    delete aptr;
    return 0;
}
