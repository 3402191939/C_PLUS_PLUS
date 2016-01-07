
/************************************************
 *C语言怎么模拟C++面向对象过程 :
 *即怎么在类外面通过《对象.》访问成员函数，
 *从而在这个成员函数里面访问类的私有数据成员的这个过程
 *思路：在C语言中把函数（C++中的成员函数）定义在外面，通过传参访问类内数据成员
 ************************************************/
#include<iostream>
using namespace std;

class AA
{
public:
	void Print()
	{
		_aa = 55;
		cout << _aa << endl;
	}
private:
	int _aa;
};

//在c语言中结构体里面不可以定义成员函数
//（C++可以，因为结构体和类是同等的，只是类内默认的数据成员是私有的，而结构体里默认的数据成员是公有的）
struct BB  
{
	typedef void(*F1)(BB* self);//函数指针
	F1 _f1;                     //f1是一个函数指针，指向一个形参是BB*的函数
	int _bb;
};
void f1(BB* self)
{
	printf("%d\n", self->_bb);
}

int main()
{
	//C++中直接通过《对象.》即可访问类内的成员函数
	//AA aa;
	//aa.Print();
	
	BB bb;
	bb._bb = 66;
	bb._f1 = f1;
	bb._f1(&bb);
	getchar();
}
