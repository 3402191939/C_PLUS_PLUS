#include <iostream>
#include <stdarg.h>
using namespace std;

#include "Template_List.hpp"
/////////////����////////////////
class SaveAdapter
{
public:
	virtual void Save(const char* format, ...) = 0;//���麯��,����Ϊ�����࣬����ʵ���������󣬸ú����ǿɱ�����б�
};
///����̨(�ն�)����������//////
class ConsoleAdapter :public SaveAdapter
{
public:
	virtual void Save(const char* format, ...)//��дSaveAdapter��Save
	{
		va_list arg;
		va_start(arg, format);//ȡ���ɱ�����б����һ���������ŵĲ�����������format���ĵ�ַ
		vfprintf(stdout, format, arg);//vfprintf�ѿɱ�����б���������������̨���նˣ�
		va_end(arg);                  //vfprintfִ�а����˲���va_arg(arg,char*);
	}
};
///�ļ�����������//////
class FileAdapter :public SaveAdapter
{
public:
	FileAdapter(const char* failname = "MemoryManagerReport.txt")
	{
		_fout = fopen(failname, "w");
	}
	virtual void Save(const char* format, ...)//��дSaveAdapter��Save
	{
		va_list arg;
		va_start(arg, format);//ȡ���ɱ�����б����һ���������ŵĲ�����������format���ĵ�ַ
		vfprintf(_fout, format, arg);//vfprintf�����������_fout�򿪵��ļ���
		va_end(arg);
	}
private:
	FILE* _fout;
};

struct MemoryBlockInfo
{
	void* _ptr;            //������ַ
	const char* _failname; //���������ļ���
	int _line;             //���������ļ��е��к�

	MemoryBlockInfo(void* ptr = NULL, const char* failname = "", int line = 0);
	bool operator== (const MemoryBlockInfo& info);
	friend ostream& operator<< (ostream& os, const MemoryBlockInfo& info);
};
class MemoryManager
{//����ģʽ
public:
	static MemoryManager* Get_sInstance();
	void* Alloc(size_t size, const char* failname, int line);
	void Dealloc(void* ptr);
	static void _Print();
	void SaveResult(SaveAdapter& s);     //�������,��ʵ�ֶ�̬
	void SaveResult();
private:
	MemoryManager()                      //��ֹ���⹹��ʵ������
	{
		atexit(_Print);                  //ע��_Print�������ڳ��������ʱ���Զ����ã�atexit�ķ���ֵ�Ͳ����б���붼��void
	}									 //��ע��ĺ��������� ���޷���ֵ�޲��������򱨴�
	MemoryManager(const MemoryManager& m)//��ֹ���⿽������ʵ������
	{}
	static MemoryManager* _sInstance;    //ʵ�������е����ݲ�����_sInstance����Ϊ���Ǿ�̬��Ա��Ϊ���ж�������
	DoubleList<MemoryBlockInfo> _infos;  //�����а���һ��˫����˫�����ÿ��Ԫ�أ�_data����һ���ṹ��
};

/***********************************************************************/

//DoubleList<MemoryBlockInfo> PList;     //����һ��ȫ��ģ��ʵ��(����ʹ�õ���ģʽ���Ż�)
										 //ȫ��ʵ������ȫ
/***********************************************************************/

//ģ��new��delete//
template<class T>
inline T* _NEW(const char* failname, int line)           //ģ��new
{
	return new(MemoryManager::Get_sInstance()->Alloc(sizeof(T), failname, line))T;
	//��λnew�����ѷ����ԭʼ�ڴ�ռ��г�ʼ��һ������
	//new(p1)int(3),p1��int*����3��ʼ��p1ָ��Ŀռ�
	//new(s1)String������Ĭ�ϵĻ����Զ���Ĺ��캯����s1������г�ʼ��
}

template<class T>
inline void _DELETE(T* ptr)                             //ģ��delete
{
	ptr->~T();//�������ͻ�ֱ�������˲���
	MemoryManager::Get_sInstance()->Dealloc(ptr);
}

//ģ��new[]��delete[]//
template<class T>
inline T* _NEW_ARRAY(size_t num, const char* failname, int line)   //num�ǿռ����
{
	T* ptr = (T*)MemoryManager::Get_sInstance()->Alloc(sizeof(T)*num + 4, failname, line);
	*(int*)ptr = num;
	T* ptrs = (T*)((char*)ptr + 4);
	//����T* ptrs = (T*)((int*)ptr + 1);
	for (size_t i = 0; i < num; i++)
	{
		new(ptrs + i)T;//���Ѿ����ڵĿռ���г�ʼ���������������������������Ϊ�����ͣ�
	}				   //�����Ĭ�ϵĹ��캯�������Զ���Ĺ��캯�����г�ʼ��
	return ptrs;
}

template<class T>
inline void _DELETE_ARRAY(T* ptr)                           
{
	T* selfPtr = (T*)((char*)ptr - 4);
	//����T* selfPtr = (T*)((int*)ptr - 1);
	int num = *(int*)selfPtr;
	for (int i = 0; i < num; i++)
	{
		ptr[i].~T();//�������ͻ�ֱ�������˲���
	}
	MemoryManager::Get_sInstance()->Dealloc(selfPtr);
}
 
/***********************************************************************/

//ģ��malloc��free//
#define Malloc(size)   \
	MemoryManager::Get_sInstance()->Alloc(size, __FILE__, __LINE__)

#define Free(ptr) \
	MemoryManager::Get_sInstance()->Dealloc(ptr)


#define NEW(type)	\
    _NEW<type>(__FILE__,__LINE__)           //����ģ�壬ʵ����һ��_NEW������ģ��new
#define DELETE(ptr)    \
	_DELETE(ptr)         //ģ��delete

#define _NEW_ARRAY(type,num)    \
	_NEW_ARRAY<type>(num,__FILE__, __LINE__)//����ģ�壬ʵ����һ��_NEW_ARRAY������ģ��new[]
#define _DELETE_ARRAR(ptr)      \
	_DELETE_ARRAR(ptr)   //ģ��delete[]