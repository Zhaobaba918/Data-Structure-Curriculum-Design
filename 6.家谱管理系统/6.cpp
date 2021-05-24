#include <iostream>
#include <string>
using namespace std;


class Member {
	friend class FamilyTree;
private:
	string m_Name;
	//����

	Member *firstChild, *nextSibling;
	//����ָ�룬�ֵ�ָ��

public:
	Member(string &name) :m_Name(name), firstChild(NULL), nextSibling(NULL) {};
};

class FamilyTree {
public:
	FamilyTree() :Ancestor(NULL) {};
	~FamilyTree() { Destory(Ancestor); }

	void Build();
	//��ʼ����������

	void SetChildren();
	//��ָ����Ա������ͥ

	void AddChildren();
	//��ָ����Ա�����Ů

	void DeleteFamily();
	//��ָ����Ա��ɢ��ͥ

	void ChangeName();
	//��ָ����Ա����

	void Search();
	//����������ĳ����Ա���������Ϣ

	void test();
	//��������������

private:
	Member *Ancestor;
	//���Ƚڵ�

	Member * Find(string & name);
	//���������ҳ�Ա
	
	Member * Find(Member *start, string &name);
	//��ָ����Ա��ʼ����������
	
	void ShowChildren(Member * currentMem);
	//���ָ����Ա����Ů��Ϣ
	
	void Destory(Member * &currentMem);
	//��������������

};

void FamilyTree::Build() {
	string name;
	cout << "�����뽨�����ף�" << endl;
	cout << "���������ȵ�������";
	cin >> name;
	Ancestor = new Member(name);
	cout << "�˼��׵������ǣ�" << name << endl;
}

Member * FamilyTree::Find(string & name) {
	return Find(Ancestor, name);
}

Member * FamilyTree::Find(Member *start, string &name) {       //�ݹ�
	if (start->m_Name == name) {
		return start;
	}
	if (start->firstChild) {
		//������Ϊ������
		Member * tmpMem = Find(start->firstChild, name);
		if (tmpMem) { return tmpMem; }
	}
	if (start->nextSibling) {
		//�����ֵ�Ϊ������
		Member * tmpMem = Find(start->nextSibling, name);
		if (tmpMem) { return tmpMem; }
	}
	return NULL;
}

void FamilyTree::ShowChildren(Member * Parent) {
	Member * currentMem = Parent->firstChild;
	if (!currentMem) {
		cout << Parent->m_Name << "û�к���!" << endl;
		return;
	}
	cout << Parent->m_Name << "�ĵ�һ�������ǣ�";
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
	cout << "������Ҫ������ͥ���˵�������";
	cin >> name;
	Member *Parent = Find(name);
	if (!Parent) {
		cout << "�ó�Ա������! " << endl;
		return;
	}
	int ChildNum;
	cout << "������" << name << "�Ķ�Ů��: ";
	cin >> ChildNum;
	while (!cin.good()) {
		cin.clear();
		cin.ignore(1024, '\n');
		cout << "�Ƿ����룬���������� ��";
		cin >> ChildNum;
	}
	if (ChildNum <= 0) {
		cout << "��Ů����������㣡" << endl;
		return;
	}
	cout << "����������" << name << "�Ķ�Ů��������";
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
	cout << "������Ҫ��Ӷ��ӻ�Ů�����˵�������";
	cin >> name;
	Member *Parent = Find(name);
	if (!Parent) {
		cout << "�ó�Ա������! " << endl;
		return;
	}
	cout << "������" << name << "����ӵĶ��ӻ�Ů����������";
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
	cout << "������Ҫ��ɢ��ͥ���˵�������";
	cin >> name;
	Member *Parent = Find(name);
	if (!Parent) {
		cout << "�ó�Ա������! " << endl;
		return;
	}
	ShowChildren(Parent);
	if (Parent->firstChild) {
		Destory(Parent->firstChild);
	}
}

void FamilyTree::ChangeName() {
	string currentName,newName;
	cout << "������Ҫ�����������˵ĵ�ǰ������";
	cin >> currentName;
	Member *Mem = Find(currentName);
	if (!Mem) {
		cout << "�ó�Ա������! " << endl;
		return;
	}
	cout << "������Ҫ���ĺ��������";
	cin >> newName;
	Mem->m_Name = newName;
	cout << currentName << "�Ѹ���Ϊ" << newName << endl;
}

void FamilyTree::Search() {
	string name;
	cout << "������Ҫ���ҵ��˵�������";
	cin >> name;
	Member *Mem = Find(name);
	if (!Mem) {
		cout << "�ó�Ա������! " << endl;
		return;
	}
	ShowChildren(Mem);
}

void FamilyTree::test() {
	cout << "**         zzy�ļ��׹���ϵͳ        "<<'\t'<<"**" << endl;
	cout << "==========================================" << endl;
	cout << "**        A --- ���Ƽ���            " << '\t' << "**" << endl;
	cout << "**        B --- ��Ӽ�ͥ��Ա        " << '\t' << "**" << endl;
	cout << "**        C --- ��ɢ�ֲ���ͥ        " << '\t' << "**" << endl;
	cout << "**        D --- ���ļ�ͥ��Ա������   " << '\t' << "**" << endl;
	cout << "**        E --- ���ҳ�Ա            " << '\t' << "**" << endl;
	cout << "**        F --- �˳�����            " << '\t' << "**" << endl;
	cout << "==========================================" << endl;
	Build();
	cout << endl;
	while (1) {
		cout << "��ѡ����Ҫ���еĲ�����";
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
			cout << "û�д˲�����" << endl;
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