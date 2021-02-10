#include <vector>
#include <iostream>
class A{
public:
	virtual int a(){
		return 0;
	}
};
class B:public A{
public:
	int a(){
		return 1;
	}
};
class c{
public:
	auto attri;
	c():attri(B()){}
	int meth(){
		return attri.a();
	}
};

int main(){
	c ob=c();
	std::cout<<ob.meth();
}