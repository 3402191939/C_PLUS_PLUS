#include <iostream>
#include <stdarg.h>
using namespace std;

#include "Template_List.hpp"
/////////////声明////////////////
class SaveAdapter
{
public:
	virtual void Save(const char* format, ...) = 0;//纯虚函数,此类为抽象类，不能实例化出对象，该函数是可变参数列表
};
///控制台(终端)保存适配器//////
class ConsoleAdapter :public SaveAdapter
{
public:
	virtual void Save(const char* format, ...)//重写SaveAdapter的Save
	{
		va_list arg;
		va_start(arg, format);//取到可变参数列表最后一个靠近逗号的参数（这里是format）的地址
		vfprintf(stdout, format, arg);//vfprintf把可变参数列表的内容输出到控制台（终端）
		va_end(arg);                  //vfprintf执行包括了操作va_arg(arg,char*);
	}
};
///文件保存适配器//////
class FileAdapter :public SaveAdapter
{
public:
	FileAdapter(const char* failname = "MemoryManagerReport.txt")
	{
		_fout = fopen(failname, "w");
	}
	virtual void Save(const char* format, ...)//重写SaveAdapter的Save
	{
		va_list arg;
		va_start(arg, format);//取到可变参数列表最后一个靠近逗号的参数（这里是format）的地址
		vfprintf(_fout, format, arg);//vfprintf把内容输出到_fout打开的文件中
		va_end(arg);
	}
private:
	FILE* _fout;
};

struct MemoryBlockInfo
{
	void* _ptr;            //变量地址
	const char* _failname; //变量所在文件名
	int _line;             //变量所在文件中的行号

	MemoryBlockInfo(void* ptr = NULL, const char* failname = "", int line = 0);
	bool operator== (const MemoryBlockInfo& info);
	friend ostream& operator<< (ostream& os, const MemoryBlockInfo& info);
};
class MemoryManager
{//单例模式
public:
	static MemoryManager* Get_sInstance();
	void* Alloc(size_t size, const char* failname, int line);
	void Dealloc(void* ptr);
	static void _Print();
	void SaveResult(SaveAdapter& s);     //父类对象,可实现多态
	void SaveResult();
private:
	MemoryManager()                      //防止在外构造实例对象
	{
		atexit(_Print);                  //注册_Print函数，在程序结束的时候自动调用，atexit的返回值和参数列表必须都是void
	}									 //被注册的函数必须是 ：无返回值无参数，否则报错
	MemoryManager(const MemoryManager& m)//防止在外拷贝构造实例对象
	{}
	static MemoryManager* _sInstance;    //实例对象中的内容不包含_sInstance，以为它是静态成员，为所有对象所有
	DoubleList<MemoryBlockInfo> _infos;  //对象中包含一个双链表，双链表的每个元素（_data）是一个结构体
};

/***********************************************************************/

//DoubleList<MemoryBlockInfo> PList;     //定义一个全局模板实例(可以使用单例模式来优化)
										 //全局实例不安全
/***********************************************************************/

//模仿new和delete//
template<class T>
inline T* _NEW(const char* failname, int line)           //模仿new
{
	return new(MemoryManager::Get_sInstance()->Alloc(sizeof(T), failname, line))T;
	//定位new，在已分配的原始内存空间中初始化一个对象。
	//new(p1)int(3),p1是int*，用3初始化p1指向的空间
	//new(s1)String，调用默认的或者自定义的构造函数对s1对象进行初始化
}

template<class T>
inline void _DELETE(T* ptr)                             //模仿delete
{
	ptr->~T();//内置类型会直接跳过此操作
	MemoryManager::Get_sInstance()->Dealloc(ptr);
}

//模仿new[]和delete[]//
template<class T>
inline T* _NEW_ARRAY(size_t num, const char* failname, int line)   //num是空间个数
{
	T* ptr = (T*)MemoryManager::Get_sInstance()->Alloc(sizeof(T)*num + 4, failname, line);
	*(int*)ptr = num;
	T* ptrs = (T*)((char*)ptr + 4);
	//或者T* ptrs = (T*)((int*)ptr + 1);
	for (size_t i = 0; i < num; i++)
	{
		new(ptrs + i)T;//对已经存在的空间进行初始化，如果是内置类型则跳过，若为类类型，
	}				   //则调用默认的构造函数或者自定义的构造函数进行初始化
	return ptrs;
}

template<class T>
inline void _DELETE_ARRAY(T* ptr)                           
{
	T* selfPtr = (T*)((char*)ptr - 4);
	//或者T* selfPtr = (T*)((int*)ptr - 1);
	int num = *(int*)selfPtr;
	for (int i = 0; i < num; i++)
	{
		ptr[i].~T();//内置类型会直接跳过此操作
	}
	MemoryManager::Get_sInstance()->Dealloc(selfPtr);
}
 
/***********************************************************************/

//模仿malloc与free//
#define Malloc(size)   \
	MemoryManager::Get_sInstance()->Alloc(size, __FILE__, __LINE__)

#define Free(ptr) \
	MemoryManager::Get_sInstance()->Dealloc(ptr)


#define NEW(type)	\
    _NEW<type>(__FILE__,__LINE__)           //调用模板，实例化一个_NEW函数，模仿new
#define DELETE(ptr)    \
	_DELETE(ptr)         //模仿delete

#define _NEW_ARRAY(type,num)    \
	_NEW_ARRAY<type>(num,__FILE__, __LINE__)//调用模板，实例化一个_NEW_ARRAY函数，模仿new[]
#define _DELETE_ARRAR(ptr)      \
	_DELETE_ARRAR(ptr)   //模仿delete[]