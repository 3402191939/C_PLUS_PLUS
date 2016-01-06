#define _CRT_SECURE_NO_WARNINGS

/////////˫������(����ѭ������)///////////

#include <iostream>
using namespace std;

template<class T>
class DoubleNode
{
	template<class T>          //��SListģ�����е�ʵ������Ϊ��Ԫ
	friend class DoubleList;
	friend class MemoryManager;
public:
	DoubleNode(const T& data)
		:_data(data)
		, _next(NULL)
		, _prev(NULL)
	{}
private:
	T _data;
	DoubleNode<T>* _next;
	DoubleNode<T>* _prev;
};

template<class T>
class DoubleList
{
	friend class MemoryManager;
public:
	///////////Ĭ�ϳ�Ա����///////////////////
	DoubleList()                              //���캯��
		:_head(NULL)
		, _tail(NULL)
	{}
	void Release()           //�ݻ�����
	{
		DoubleNode<T>* begin = _head;
		while (begin)
		{
			DoubleNode<T>* del = begin;
			begin = begin->_next;
			delete del;
		}
		_head = NULL;
		_tail = NULL;
	}
	~DoubleList()                             //��������
	{
		Release();
	}
	DoubleList(const DoubleList<T>& d)        //�������캯��
		:_head(NULL)
		, _tail(NULL)
	{
		DoubleNode<T>* begin = d._head;
		while (begin)
		{
			PushBack(begin->_data);
			begin = begin->_next;
		}
	}
	DoubleList<T>& operator= (const DoubleList<T>& d) //��ֵ���������غ���
	{
		if (this != &d)
		{
			Release();
			DoubleNode<T>* begin = d._head;
			while (begin)
			{
				PushBack(begin->_data);
				begin = begin->_next;
			}
		}
		return *this;
	}
	//////////////������������///////////////////
	void PrintDoubleList()                         //��ӡ����
	{
		DoubleNode<T>* begin = _head;
		cout << "SList->";
		while (begin)
		{
			cout << begin->_data << "->";
			begin = begin->_next;
		}
		cout << "NULL" << endl;
	}
	void PushBack(const T& data)               //β��
	{
		if (_head == NULL)
		{
			_head = new DoubleNode<T>(data);
			_tail = _head;
		}
		else
		{
			DoubleNode<T>* ret = new DoubleNode<T>(data);
			ret->_prev = _tail;
			_tail->_next = ret;
			_tail = _tail->_next;
		}
	}
	void PopBack()                //βɾ
	{
		if (_head == NULL)
		{
			return;
		}
		if (_head == _tail)
		{
			delete _head;
			_head = NULL;
			_tail = NULL;
		}
		else
		{
			DoubleNode<T>* del = _tail;
			_tail = _tail->_prev;
			_tail->_next = NULL;
			delete del;
		}
	}
	void PushFront(const T& data)            //ͷ��
	{
		if (_head == NULL)
		{
			_head = new DoubleNode<T>(data);
			_tail = _head;
		}
		else
		{
			DoubleNode<T>* begin = new DoubleNode<T>(data);
			begin->_next = _head;
			_head->_prev = begin;
			_head = begin;
		}
	}
	void PopFront()            //ͷɾ
	{
		if (_head == NULL)
		{
			return;
		}
		if (_head == _tail)
		{
			delete _head;
			_head = NULL;
			_tail = NULL;
		}
		else
		{
			DoubleNode<T>* begin = _head;
			_head = _head->_next;
			_head->_prev = NULL;
			delete begin;
		}
	}
	DoubleNode<T>* Find(T const& data)                //���ҽ��
	{
		DoubleNode<T>* begin = _head;
		while (begin)
		{
			if (begin->_data == data)
			{
				return begin;
			}
			begin = begin->_next;
		}
		return NULL;
	}
	void Erase(DoubleNode<T>* node)     //����ɾ��һ���ڵ�
	{
		if (_head == NULL)
		{
			cout << "Not Exist" << endl;
			return;
		}
		if (_head == _tail && _head == node)
		{
			delete _head;
			_head = NULL;
			_tail = NULL;
		}
		else
		{
			if (_head == node)
			{
				_head = _head->_next;
				_head->_prev = NULL;
			}
			else if (_tail == node)
			{
				_tail = _tail->_prev;
				_tail->_next = NULL;
			}
			else
			{
				DoubleNode<T>* prev = node->_prev;
				DoubleNode<T>* next = node->_next;
				prev->_next = next;
				next->_prev = prev;
			}
			delete node;
		}
	}
	void ReverseDoubleList()                      //��������
	{
		DoubleNode<T>* newhead = NULL;
		DoubleNode<T>* begin = _head;
		while (begin)
		{
			_head = _head->_next;
			begin->_next = newhead;
			newhead = begin;
			begin = _head;
		}
		_head = newhead;
	}
private:
	DoubleNode<T>* _head;        //ָ��ͷ�ڵ��ָ��
	DoubleNode<T>* _tail;        //ָ��β�ڵ��ָ��
};


