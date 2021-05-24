#include <iostream>
#include <string>
#include <iomanip>
#include <stdio.h>
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

	ListNode<Type> *Find(int i);

	int Insert(Type value, int i);

	Type *Remove(int i);

	Type *Get(int i);

	int NotEmpty() { return first != last; };
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
	while (p != NULL && i--) {         //优化，没有开辟另一个int j
		p = p->link;
	}
	return p;
}

template <class Type>
int List<Type>::Insert(Type value, int i) {
	ListNode<Type> *p = Find(i - 1);
	if (p == NULL) { return 0; }
	ListNode<Type> *newnode = p->GetNode(value, p->link); //?????????????????
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


class ExamRegistrationSystem;

class Student {
	friend class ExamRegistrationSystem;
	string m_number;
	//考号

	string m_name;
	//姓名

	string m_sex;
	//性别

	int m_age;
	//年龄

	string m_type;
	//报考类型

public:
	Student() {};
	Student(string number, string name, string sex, int age, string type) :m_number(number), m_name(name), m_sex(sex), m_age(age), m_type(type) {}
};

class Subject {
	friend class ExamRegistrationSystem;
public:
	Subject() {};
	Subject(string sub) :m_subject(sub), m_count(1) {}
	int operator==(Subject &sub) { return sub.m_subject == m_subject; }
	void Show() { cout << left << setw(15) << m_subject << m_count << endl; }
private:
	string m_subject;
	//报考类型名称

	int m_count;
	//报考人数
};

class ExamRegistrationSystem {
private:
	List<Student> Students;
	//学生信息

public:
	ExamRegistrationSystem();

	int Input();
	//输入考生信息，初始化报名系统

	int Insert();
	//插入

	Student * Find(string num);
	//查找

	int Revise(string num);
	//修改

	Student * Remove(string num);
	//删除

	void Show();
	//输出当前所有考生信息

	List<Subject> *Statistics();
	//统计

	Student *GetStudent();
	//输入一个考生的信息并返回指向该考生的指针
};


ExamRegistrationSystem::ExamRegistrationSystem() {
	cout << "首先请建立考生信息系统！" << endl;
	Input();
}

int ExamRegistrationSystem::Input() {
	int count;
	cout << "请输入考生人数: ";
	cin >> count;
	while (!cin.good() || count <= 0) {
		cin.clear();
		cin.ignore(1024, '\n');
		cout << "非法输入，请重新输入 ：";
		cin >> count;
	}
	Student stu;
	cout << "请依次输入考生的考号，姓名，性别，年龄及报考类别！" << endl;
	while (count--) {
		cin >> stu.m_number >> stu.m_name >> stu.m_sex >> stu.m_age >> stu.m_type;
		while (!cin.good()) {
			cin.clear();
			cin.ignore(1024, '\n');
			cout << "非法输入，请重新输入!" << endl;
			cout << "请依次输入考生的考号，姓名，性别，年龄及报考类别！" << endl;
			cin >> stu.m_number >> stu.m_name >> stu.m_sex >> stu.m_age >> stu.m_type;
		}
		Students.Insert(stu, Students.Length());
	}
	cout << endl;
	cout << "创建成功：" << endl;
	return 1;
}

void ExamRegistrationSystem::Show() {
	ListIterator<Student> iter(Students);
	Student *p;
	cout << left << setw(15) << "考号" << setw(10) << "姓名" << setw(10) << "性别" << setw(10) << "年龄" << setw(10) << "报考类别" << endl;
	while (p = iter.Next()) {
		cout << left << setw(15) << p->m_number << setw(10) << p->m_name << setw(10) << p->m_sex << setw(10) << p->m_age << setw(10) << p->m_type << endl;
	}
}

int ExamRegistrationSystem::Insert() {
	int i;
	cout << "请输入您要插入的位置：";
	cin >> i;
	while (!cin.good()) {
		cin.clear();
		cin.ignore(1024, '\n');
		cout << "非法输入，请重新输入 ：";
		cin >> i;
	}
	i -= 1;
	if (i < 0 || i>Students.Length()) {
		cin.ignore(1024, '\n');
		cout << "非法的位置！" << endl;
		return 0;
	}
	Student *stu = GetStudent();
	Students.Insert(*stu, i);
	cout << "插入成功" << endl;
	return 1;
}

Student *ExamRegistrationSystem::Find(string num) {
	ListIterator<Student> iter(Students);
	Student *p;
	while (p = iter.Next()) {
		if (num == p->m_number) {
			cout << "考号" << '\t' << "姓名" << '\t' << "性别" << '\t' << "年龄" << '\t' << "报考类别" << endl;
			cout << p->m_number << '\t' << p->m_name << '\t' << p->m_sex << '\t' << p->m_age << '\t' << p->m_type << endl;
			return p;
		}
	}
	cout << "系统中没有该考生！" << endl;
	return NULL;
}

int ExamRegistrationSystem::Revise(string num) {
	Student *p = Find(num);
	if (p) {
		cout << "请依次输入修改后考生的考号，姓名，性别，年龄及报考类别！" << endl;
		cin >> p->m_number >> p->m_name >> p->m_sex >> p->m_age >> p->m_type;
		cout << "修改成功" << endl;
		return 1;
	}
	return 0;
}

Student *ExamRegistrationSystem::Remove(string num) {
	ListIterator<Student> iter(Students);
	Student *p;
	int i = 0;
	while (p = iter.Next()) {
		if (num == p->m_number) {
			cout << "删除成功，删除的信息是：" << endl;
			cout << "考号" << '\t' << "姓名" << '\t' << "性别" << '\t' << "年龄" << '\t' << "报考类别" << endl;
			cout << p->m_number << '\t' << p->m_name << '\t' << p->m_sex << '\t' << p->m_age << '\t' << p->m_type << endl;
			return Students.Remove(i);
		}
		i++;
	}
	cout << "系统中没有该考生！" << endl;
	return NULL;
}

Student *ExamRegistrationSystem::GetStudent() {
	string number;
	string name;
	string sex;
	int age;
	string type;
	cout << "请依次输入考生的考号，姓名，性别，年龄及报考类别：" << endl;
	cin >> number >> name >> sex >> age >> type;
	Student *stu = new Student(number, name, sex, age, type);
	return stu;
}

List<Subject> *ExamRegistrationSystem::Statistics() {
	List<Subject> *p = new List<Subject>;	//报考科目链表
	ListIterator<Student> iter(Students);	//学生信息迭代器
	while (iter.NextNotNull()) {
		//遍历考生信息链表，找出所有不同科目，链入报考科目链表并计数
		string type = iter.Next()->m_type;
		Subject sub(type);
		Subject *pp = p->Find(sub);
		if (pp) {
			(pp->m_count)++;
		}
		else {
			p->Insert(sub, 0);
		}
	}
	if (p->NotEmpty()) {
		//输出结果
		cout << "统计结果为：" << endl;
		cout << left << setw(15) << "报考类型" << "报考人数" << endl;
		ListIterator<Subject> iterSub(*p);
		while (iterSub.NextNotNull()) {
			iterSub.Next()->Show();
		}
		return p;
	}
	return NULL;
}





int main() {
	//FILE *stream;
	//freopen_s(&stream, "输入.txt", "r", stdin);
	ExamRegistrationSystem sys;
	int operation;
	while (1) {
		sys.Show();
		cout << "请选择您要进行的操作，1为插入，2为删除，3为查找，4为修改，5为统计，0为退出：";
		cin >> operation;
		while (!cin.good()) {
			cin.clear();
			cin.ignore(1024, '\n');
			cout << "非法输入，请重新输入 ：";
			cin >> operation;
		}
		switch (operation) {
		case 1: {
			sys.Insert(); }
				break;
		case 2: {
			string a;
			cout << "请输入您删除的考号：";
			cin >> a;
			sys.Remove(a); }
				break;
		case 3: {
			string a;
			cout << "请输入您查找的考号：";
			cin >> a;
			sys.Find(a); }
				break;
		case 4: {
			string a;
			cout << "请输入您修改的考号：";
			cin >> a;
			sys.Revise(a); }
				break;
		case 5: {
			sys.Statistics(); }
				break;
		case 0: 
			break;
		default:
			cout << "输入有误！" << endl;
		}
		if (operation == 0)break;
		cout << endl;
	}
	return 0;
}
