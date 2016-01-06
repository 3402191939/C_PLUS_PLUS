#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdlib.h>
using namespace std;

#include "MemoryManager.hpp"
///////////����//////////////////

/***********MemoryBlockInfo����*****************/
MemoryBlockInfo::MemoryBlockInfo(void* ptr, const char* failname, int line)
		:_ptr(ptr)           //���ⶨ�岻�ܴ���Ĭ��ֵ
		, _failname(failname)
		, _line(line)
	{}
bool MemoryBlockInfo::operator== (const MemoryBlockInfo& info)
{
	return _ptr == info._ptr;//ֻ��Ҫ����ַ�Ƿ�һ�¼����ж��������ݣ�_data���Ƿ����
}
ostream& operator<< (ostream& os, const MemoryBlockInfo& info)
{
	os << "_ptr:" << info._ptr << endl;
	os << "_failname:" << info._failname << endl;
	os << "_line:" << info._line << endl;
	return os;
}

/***********MemoryManager����*****************/
MemoryManager* MemoryManager::Get_sInstance()
{   //����ģʽ���ŵ㣺�̰߳�ȫ��ȱ�㣺�������ʱ����
	//����һ�ֶ���ģʽ��ֱ����public���ֶ���static MemoryManager* _inStance = new MemoryManager();
	//���ַ���������ȽϿ죬static��Ա�ڱ���ʱ�Ѿ����ڣ������̲߳���ȫ
	if (_sInstance == NULL)
	{
		_sInstance = new MemoryManager();
	}
	return _sInstance;
}
inline void*  MemoryManager::Alloc(size_t size, const char* failname, int line)//�������õĺ����Ͷ���Ϊ��������
{
	void* ptr = malloc(size);          //���ֽ�ȥ���ٿռ�,��ΪMemoryBlockInfo�ǽṹ��
	if (ptr)						   //�ʿ���ֱ�ӷ��ʵ�MemoryBlockInfo���ݳ�Ա
	{
		_infos.PushBack(MemoryBlockInfo(ptr, failname, line));//����Ϣд�뵽������
	}
	return ptr;
}
inline void  MemoryManager::Dealloc(void* ptr)//inlineҪ�ͺ��������һ�����˵������������
{
	if (ptr)
	{
		DoubleNode<MemoryBlockInfo>* ret = _infos.Find(MemoryBlockInfo(ptr));//ֻ��Ҫͨ��ָ��Ѱ�Ҽ���
		_infos.Erase(ret);          //ɾ��������Ķ�Ӧ�ڵ���Ϣ
	}
	return free(ptr);               //�ͷ����⿪�ٿռ��ָ��
}
void MemoryManager::_Print()   //����޷���ֵ���޲����б�Ϊ����atexit����ע��
{
	//MemoryManager::Get_sInstance()->_infos.PrintDoubleList();
	MemoryManager::Get_sInstance()->SaveResult();
}
void MemoryManager::SaveResult(SaveAdapter& s) //�в���SaveResult(SaveAdapter& s)����
{
	DoubleNode<MemoryBlockInfo>* begin = _infos._head;
	while (begin)
	{
		s.Save("ptr ��%p\nfilename : %s\nline : %d\n",//�ɱ�����б�printfҲ�ǿɱ�����б�����
			begin->_data._ptr,
			begin->_data._failname,
			begin->_data._line);
		begin = begin->_next;
	}
}
void MemoryManager::SaveResult()                 //�޲���SaveResult()��������������
{
	MemoryManager::Get_sInstance()->SaveResult(ConsoleAdapter()); //д������̨
	MemoryManager::Get_sInstance()->SaveResult(FileAdapter());    //д���ļ�����һ���ļ�
}
MemoryManager* MemoryManager::_sInstance = NULL;


///////String��///////////
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
	int* p1 = (int*)Malloc(sizeof(int)* 5);//�����ܵ��ֽ���Ŀ��ǿ������ת��
	int* p2 = (int*)Malloc(sizeof(int)* 5);
	String* p6 = (String*)Malloc(sizeof(String)* 5);
	Free(p1);
	Free(p6);
	int* p3 = NEW(int);                    //����ǿ������ת��
	char* p4 = NEW(char);
	String* s = NEW(String);
	int* p5 = _NEW_ARRAY(int, 5);         //����
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
	//atexit(Print);//ע�ắ��Print()��atexitҪ�������������ֵ�Ͳ����б�Ϊ�գ�
	                //����������Զ�ȥ����Print�����������Լ��ֶ�����Print����
	//MemoryManager::Get_sInstance()->_Print();
	//MemoryManager::Get_sInstance()->SaveResult();
	//getchar();
	system("pause");
	return 0;
}