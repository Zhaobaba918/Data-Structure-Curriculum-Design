#include <iostream>
#include <string>
using namespace std;

template <class Type>
class List;
template <class Type>
class ListIterator;

template <class Type>
class ListNode {
	friend class List<Type>;
	friend class ListIterator<Type>;
	Type data;
	ListNode<Type> *link;
public:
	//构造函数
	ListNode() :link(NULL) {};
	ListNode(const Type& item) :data(item), link(NULL) {};

	//返回下一节点地址
	ListNode<Type> *NextNode() { return link; };

	//创建数据为item，指针为next的新结点
	ListNode<Type> *GetNode(const Type &item, ListNode<Type> *next);

	//当前节点后插入节点p
	void InsertAfter(ListNode<Type> *p);

	//摘下当前节点的下一节点
	ListNode<Type> *RemoveAfter();
};

template <class Type>
class List {
	friend class ListIterator<Type>;
	ListNode<Type> *first, *last;
public:
	//构造函数
	List();                      //???
	List(const Type &value);

	//析构函数
	~List();

	void MakeEmpty();

	int Length() const;

	Type *Find(Type value);

	ListNode<Type> *Find(int i);                //重载问题int

	int Insert(Type value, int i);

	Type *Remove(int i);

	Type *Get(int i);

	int NotEmpty() { return first != last; };

	void Show();
};

template <class Type>
class ListIterator {

public:
	ListIterator(const List<Type> &l) :list(l), current(l.first) {}

	//检查当前指针是否为空
	int NotNull();

	//检查下一节点是否为空
	int NextNotNull();

	//返回链表头指针
	ListNode<Type> *First();

	//返回下一节点地址
	Type *Next();

private:
	const List<Type> &list;
	ListNode<Type> *current;
};



//链表结点部分操作的实现
template <class Type>
ListNode<Type> *ListNode<Type>::GetNode(const Type &item, ListNode<Type> *next) {
	ListNode<Type> *newnode = new ListNode<Type>(item);
	newnode->link = next;
	return newnode;
}

template <class Type>
void ListNode<Type>::InsertAfter(ListNode<Type> *p) {
	p->link = link;
	link = p;
}

template <class Type>
ListNode<Type>* ListNode<Type>::RemoveAfter() {
	if (link == NULL) {
		return NULL;
	}
	ListNode<Type> *p = link;
	link = p->link;
	return p;
}


//链表的公共操作
template <class Type>
List<Type>::List() {
	last = first = new ListNode<Type>;
}

template <class Type>
List<Type>::List(const Type &value) {
	first = new ListNode<Type>;
	last = new ListNode<Type>(value);
	first->link = last;
}

template <class Type>
List<Type>::~List() {
	MakeEmpty();
	delete first;
}

template <class Type>
void List<Type>::MakeEmpty() {
	ListNode<Type> *q;
	while (first->link != NULL) {
		q = first->link;
		first->link = q->link;
		delete q;
	}
	last = first;
}

template <class Type>
int List<Type>::Length() const {
	int count = 0;
	ListNode<Type> *p = first;
	while (p->link != NULL) {
		count++;
		p = p->link;
	}
	return count;
}
template <class Type>
Type *List<Type>::Find(Type value) {          //value为什么不引用
	ListNode<Type> *p = first->link;
	while (p != NULL && !(p->data == value)) {
		p = p->link;                                   //修改过
	}
	return &(p->data);
}

template <class Type>
ListNode<Type> *List<Type>::Find(int i) {
	if (i < -1) {
		return NULL;
	}
	if (i == -1) {
		return first;
	}
	ListNode<Type> *p = first->link;
	while (p != NULL && i--) {
		p = p->link;
	}
	return p;
}

//输入-1插入表尾
template <class Type>
int List<Type>::Insert(Type value, int i) {
	if (i == -1) {
		ListNode<Type> *newnode = last->GetNode(value, NULL);
		last->link = newnode;
		last = newnode;
		return 1;
	}
	ListNode<Type> *p = Find(i - 1);
	if (p == NULL) { return 0; }
	ListNode<Type> *newnode = p->GetNode(value, p->link);
	p->link = newnode;
	if (p == last) { last = newnode; }
	return 1;
}

template <class Type>
Type *List<Type>::Remove(int i) {
	ListNode<Type> *p = Find(i - 1);
	if (p == NULL) { return NULL; }
	ListNode<Type> *toRemove = p->link;
	if (toRemove == NULL) { return NULL; }
	p->link = toRemove->link;
	if (toRemove == last) { last = p; }
	Type *value = new Type(toRemove->data);
	delete toRemove;
	return value;
}

template <class Type>
Type* List<Type>::Get(int i) {
	ListNode<Type> *p = Find(i);
	if (p == NULL || p == first) { return NULL; }
	return &p->data;
}

template <class Type>
void List<Type>::Show() {
	if (!this->NotEmpty()) {
		cout << "NULL";
		return;
	}
	ListIterator<Type> iter(*this);
	cout << *(iter.Next());
	while (iter.NextNotNull()) {
		cout << ' ' << *(iter.Next());       //重载<<
	}
}


template <class Type>
int ListIterator<Type>::NotNull() {
	return current != NULL;
}

template <class Type>
int ListIterator<Type>::NextNotNull() {
	return current->link != NULL;
}

template <class Type>
ListNode<Type> *ListIterator<Type>::First() {
	return list.first;
}

template <class Type>
Type *ListIterator<Type>::Next() {
	if (NotNull() && NextNotNull()) {
		current = current->link;
		return &current->data;
	}
	return NULL;
}




void Intersection(const List<float> &list1, const List<float> &list2, List<float> &result) {
	ListIterator<float> iter1(list1), iter2(list2);
	float *num1 = iter1.Next(), *num2 = iter2.Next();
	while (num1 && num2) {
		if (*num1 < *num2) {
			num1 = iter1.Next();
		}
		else if (*num1 > *num2) {
			num2 = iter2.Next();
		}
		else {
			result.Insert(*num1, -1);
			num1 = iter1.Next();
			num2 = iter2.Next();
		}
	}
}

void test() {
	List<float> list1, list2, result;
	int tempInt;
	while (cin >> tempInt, tempInt != -1) {
		list1.Insert(tempInt, -1);
	}
	while (cin >> tempInt, tempInt != -1) {
		list2.Insert(tempInt, -1);
	}
	Intersection(list1, list2, result);
	result.Show();
}



int main() {
	test();
	cout << endl;
	system("pause");
	return 0;
}