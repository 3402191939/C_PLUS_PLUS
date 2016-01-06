#define _CRT_SECURE_NO_WARNINGS

/////////双向链表(不是循环链表)///////////

#include <iostream>
using namespace std;

template<class T>
class DoubleNode
{
	template<class T>          //让SList模板所有的实例都成为友元
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
	///////////默认成员函数///////////////////
	DoubleList()                              //构造函数
		:_head(NULL)
		, _tail(NULL)
	{}
	void Release()           //摧毁链表
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
	~DoubleList()                             //析构函数
	{
		Release();
	}
	DoubleList(const DoubleList<T>& d)        //拷贝构造函数
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
	DoubleList<T>& operator= (const DoubleList<T>& d) //赋值操作符重载函数
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
	//////////////基本操作函数///////////////////
	void PrintDoubleList()                         //打印链表
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
	void PushBack(const T& data)               //尾插
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
	void PopBack()                //尾删
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
	void PushFront(const T& data)            //头插
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
	void PopFront()            //头删
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
	DoubleNode<T>* Find(T const& data)                //查找结点
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
	void Erase(DoubleNode<T>* node)     //任意删除一个节点
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
	void ReverseDoubleList()                      //链表逆置
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
	DoubleNode<T>* _head;        //指向头节点的指针
	DoubleNode<T>* _tail;        //指向尾节点的指针
};


