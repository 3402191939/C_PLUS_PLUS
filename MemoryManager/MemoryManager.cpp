#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdlib.h>
using namespace std;

#include "MemoryManager.hpp"
///////////定义//////////////////

/***********MemoryBlockInfo定义*****************/
MemoryBlockInfo::MemoryBlockInfo(void* ptr, const char* failname, int line)
		:_ptr(ptr)           //类外定义不能带有默认值
		, _failname(failname)
		, _line(line)
	{}
bool MemoryBlockInfo::operator== (const MemoryBlockInfo& info)
{
	return _ptr == info._ptr;//只需要检查地址是否一致即可判断两个数据（_data）是否相等
}
ostream& operator<< (ostream& os, const MemoryBlockInfo& info)
{
	os << "_ptr:" << info._ptr << endl;
	os << "_failname:" << info._failname << endl;
	os << "_line:" << info._line << endl;
	return os;
}

/***********MemoryManager定义*****************/
MemoryManager* MemoryManager::Get_sInstance()
{   //懒汉模式，优点：线程安全，缺点：加载类的时候慢
	//还有一种饿汉模式，直接在public部分定义static MemoryManager* _inStance = new MemoryManager();
	//这种方法加载类比较快，static成员在编译时已经存在，但是线程不安全
	if (_sInstance == NULL)
	{
		_sInstance = new MemoryManager();
	}
	return _sInstance;
}
inline void*  MemoryManager::Alloc(size_t size, const char* failname, int line)//常被调用的函数就定义为内联函数
{
	void* ptr = malloc(size);          //以字节去开辟空间,因为MemoryBlockInfo是结构体
	if (ptr)						   //故可以直接访问到MemoryBlockInfo数据成员
	{
		_infos.PushBack(MemoryBlockInfo(ptr, failname, line));//把信息写入到链表中
	}
	return ptr;
}
inline void  MemoryManager::Dealloc(void* ptr)//inline要和函数定义放一块才能说明是内联函数
{
	if (ptr)
	{
		DoubleNode<MemoryBlockInfo>* ret = _infos.Find(MemoryBlockInfo(ptr));//只需要通过指针寻找即可
		_infos.Erase(ret);          //删除链表里的对应节点信息
	}
	return free(ptr);               //释放在外开辟空间的指针
}
void MemoryManager::_Print()   //设计无返回值、无参数列表，为了用atexit函数注册
{
	//MemoryManager::Get_sInstance()->_infos.PrintDoubleList();
	MemoryManager::Get_sInstance()->SaveResult();
}
void MemoryManager::SaveResult(SaveAdapter& s) //有参数SaveResult(SaveAdapter& s)函数
{
	DoubleNode<MemoryBlockInfo>* begin = _infos._head;
	while (begin)
	{
		s.Save("ptr ：%p\nfilename : %s\nline : %d\n",//可变参数列表（printf也是可变参数列表函数）
			begin->_data._ptr,
			begin->_data._failname,
			begin->_data._line);
		begin = begin->_next;
	}
}
void MemoryManager::SaveResult()                 //无参数SaveResult()函数，构成重载
{
	MemoryManager::Get_sInstance()->SaveResult(ConsoleAdapter()); //写到控制台
	MemoryManager::Get_sInstance()->SaveResult(FileAdapter());    //写到文件，打开一个文件
}
MemoryManager* MemoryManager::_sInstance = NULL;


///////String类///////////
class String
{
public:
	String(const char* str = "")
		:_str(new char[strlen(str)+1])
	{
		strcpy(_str, str);
	}
	~String()
	{
		if (_str)
		{
			delete[] _str;
		}
	}
private:
	char* _str;
};
void Test1()
{
	int* p1 = (int*)Malloc(sizeof(int)* 5);//传递总的字节数目，强制类型转化
	int* p2 = (int*)Malloc(sizeof(int)* 5);
	String* p6 = (String*)Malloc(sizeof(String)* 5);
	Free(p1);
	Free(p6);
	int* p3 = NEW(int);                    //不必强制类型转化
	char* p4 = NEW(char);
	String* s = NEW(String);
	int* p5 = _NEW_ARRAY(int, 5);         //数组
	String* s1 = _NEW_ARRAY(String,4);
	_DELETE(p3);
	_DELETE(s);
	_DELETE_ARRAY(p5);
	_DELETE_ARRAY(s1);
}

int main()
{
	Test1();
	//PList.PrintDoubleList();
	//atexit(Print);//注册函数Print()，atexit要求这个函数返回值和参数列表都为空，
	                //程序结束后自动去调用Print函数，不用自己手动调用Print函数
	//MemoryManager::Get_sInstance()->_Print();
	//MemoryManager::Get_sInstance()->SaveResult();
	//getchar();
	system("pause");
	return 0;
}