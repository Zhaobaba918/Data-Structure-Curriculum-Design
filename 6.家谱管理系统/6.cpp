#include <iostream>
#include <string>
using namespace std;


class Member {
	friend class FamilyTree;
private:
	string m_Name;
	//姓名

	Member *firstChild, *nextSibling;
	//长子指针，兄弟指针

public:
	Member(string &name) :m_Name(name), firstChild(NULL), nextSibling(NULL) {};
};

class FamilyTree {
public:
	FamilyTree() :Ancestor(NULL) {};
	~FamilyTree() { Destory(Ancestor); }

	void Build();
	//初始化建立家谱

	void SetChildren();
	//对指定成员建立家庭

	void AddChildren();
	//对指定成员添加子女

	void DeleteFamily();
	//对指定成员解散家庭

	void ChangeName();
	//对指定成员改名

	void Search();
	//按姓名查找某个成员并输出其信息

	void test();
	//用于主函数调用

private:
	Member *Ancestor;
	//祖先节点

	Member * Find(string & name);
	//按姓名查找成员
	
	Member * Find(Member *start, string &name);
	//从指定成员开始按姓名查找
	
	void ShowChildren(Member * currentMem);
	//输出指定成员的子女信息
	
	void Destory(Member * &currentMem);
	//用于析构函数中

};

void FamilyTree::Build() {
	string name;
	cout << "首先请建立家谱！" << endl;
	cout << "请输入祖先的姓名：";
	cin >> name;
	Ancestor = new Member(name);
	cout << "此家谱的祖先是：" << name << endl;
}

Member * FamilyTree::Find(string & name) {
	return Find(Ancestor, name);
}

Member * FamilyTree::Find(Member *start, string &name) {       //递归
	if (start->m_Name == name) {
		return start;
	}
	if (start->firstChild) {
		//以左孩子为起点查找
		Member * tmpMem = Find(start->firstChild, name);
		if (tmpMem) { return tmpMem; }
	}
	if (start->nextSibling) {
		//以右兄弟为起点查找
		Member * tmpMem = Find(start->nextSibling, name);
		if (tmpMem) { return tmpMem; }
	}
	return NULL;
}

void FamilyTree::ShowChildren(Member * Parent) {
	Member * currentMem = Parent->firstChild;
	if (!currentMem) {
		cout << Parent->m_Name << "没有孩子!" << endl;
		return;
	}
	cout << Parent->m_Name << "的第一代子孙是：";
	while (currentMem) {
		cout << ' ' << currentMem->m_Name;
		currentMem = currentMem->nextSibling;
	}
	cout << endl;
}

void FamilyTree::Destory(Member * &currentMem) {            
	if (currentMem->firstChild) { Destory(currentMem->firstChild); }
	if (currentMem->nextSibling) { Destory(currentMem->nextSibling); }
	delete currentMem;
	currentMem = NULL;
}

void FamilyTree::SetChildren() {
	string name;
	cout << "请输入要建立家庭的人的姓名：";
	cin >> name;
	Member *Parent = Find(name);
	if (!Parent) {
		cout << "该成员不存在! " << endl;
		return;
	}
	int ChildNum;
	cout << "请输入" << name << "的儿女数: ";
	cin >> ChildNum;
	while (!cin.good()) {
		cin.clear();
		cin.ignore(1024, '\n');
		cout << "非法输入，请重新输入 ：";
		cin >> ChildNum;
	}
	if (ChildNum <= 0) {
		cout << "儿女数必须大于零！" << endl;
		return;
	}
	cout << "请依次输入" << name << "的儿女的姓名：";
	string ChildName;
	cin >> ChildName;
	Member *currentMem = Parent->firstChild = new Member(ChildName);
	for (int i = 1; i < ChildNum; i++) {
		cin >> ChildName;
		currentMem->nextSibling = new Member(ChildName);
		currentMem = currentMem->nextSibling;
	}
	ShowChildren(Parent);
}

void FamilyTree::AddChildren() {
	string name;
	cout << "请输入要添加儿子或女儿的人的姓名：";
	cin >> name;
	Member *Parent = Find(name);
	if (!Parent) {
		cout << "该成员不存在! " << endl;
		return;
	}
	cout << "请输入" << name << "新添加的儿子或女儿的姓名：";
	string ChildName;
	cin >> ChildName;
	Member *currentMem = Parent->firstChild;
	if (!currentMem) { Parent->firstChild = new Member(ChildName); }
	else {
		while (currentMem->nextSibling) { currentMem = currentMem->nextSibling; }
		currentMem->nextSibling = new Member(ChildName);
	}
	ShowChildren(Parent);
}

void FamilyTree::DeleteFamily() {
	string name;
	cout << "请输入要解散家庭的人的姓名：";
	cin >> name;
	Member *Parent = Find(name);
	if (!Parent) {
		cout << "该成员不存在! " << endl;
		return;
	}
	ShowChildren(Parent);
	if (Parent->firstChild) {
		Destory(Parent->firstChild);
	}
}

void FamilyTree::ChangeName() {
	string currentName,newName;
	cout << "请输入要更改姓名的人的当前姓名：";
	cin >> currentName;
	Member *Mem = Find(currentName);
	if (!Mem) {
		cout << "该成员不存在! " << endl;
		return;
	}
	cout << "请输入要更改后的姓名：";
	cin >> newName;
	Mem->m_Name = newName;
	cout << currentName << "已更名为" << newName << endl;
}

void FamilyTree::Search() {
	string name;
	cout << "请输入要查找的人的姓名：";
	cin >> name;
	Member *Mem = Find(name);
	if (!Mem) {
		cout << "该成员不存在! " << endl;
		return;
	}
	ShowChildren(Mem);
}

void FamilyTree::test() {
	cout << "**         zzy的家谱管理系统        "<<'\t'<<"**" << endl;
	cout << "==========================================" << endl;
	cout << "**        A --- 完善家谱            " << '\t' << "**" << endl;
	cout << "**        B --- 添加家庭成员        " << '\t' << "**" << endl;
	cout << "**        C --- 解散局部家庭        " << '\t' << "**" << endl;
	cout << "**        D --- 更改家庭成员的姓名   " << '\t' << "**" << endl;
	cout << "**        E --- 查找成员            " << '\t' << "**" << endl;
	cout << "**        F --- 退出程序            " << '\t' << "**" << endl;
	cout << "==========================================" << endl;
	Build();
	cout << endl;
	while (1) {
		cout << "请选择您要进行的操作：";
		char Operation;
		cin >> Operation;
		switch (Operation)
		{
		case 'A':
			SetChildren();
			cout << endl;
			break;
		case 'B':
			AddChildren();
			cout << endl;
			break;
		case 'C':
			DeleteFamily();
			cout << endl;
			break;
		case 'D':
			ChangeName();
			cout << endl;
			break;
		case 'E':
			Search();
			cout << endl;
			break;
		case 'F':
			return;
		default:
			cout << "没有此操作！" << endl;
			cin.ignore(1024, '\n');
			cout << endl;
			break;
		}
	}
}

int main() {
	FamilyTree myTree;
	myTree.test();
	system("pause");
	return 0;
}