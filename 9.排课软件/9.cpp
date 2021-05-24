#include <assert.h>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;



template <class T>
class Stack;

template <class T>
class StackNode {
	friend class Stack<T>;
private:
	T data;
	StackNode<T> *next;
};


template <class T>
class Stack {
public:
	Stack() :Top(NULL) {};
	void Push(T &elem);
	T Pop();
	T GetFirst();
	bool IsEmpty() { return Top == NULL; }
	~Stack();
private:
	StackNode<T> *Top;
};


template <class T>
class Queue {
public:
	Queue() :front(0), rear(0), maxSize(0) {}
	Queue(int size) :elements(new T[size + 1]), front(0), rear(0), maxSize(size + 1) {}
	~Queue() { delete[]elements; };

	void Push(T &newT);
	T Pop();
	int IsFull() { return (rear + 1) % maxSize == front; }
	int IsEmpty() { return rear == front; }
	int getSize() { return maxSize; }
private:
	T *elements;
	int rear;
	int front;
	int maxSize;
};




class ScheduleSystem;

class Course {					//课程
	friend class ScheduleSystem;
private:
	string Name, Number;		//名称，编号
	int Hours, Term, NumBefore; //课时数，指定开课学期，先修课程数
	Stack<int> Before;			//先修课程
	Stack<int> After;			//后继课程
};


class ScheduleSystem {			//排课系统
public:
	ScheduleSystem() :NumTermCourses(0), Sum(0) { for (int i = 0; i < 8; i++) { NumOfCourse[i] = 0; } }

	bool Input();
	//输入课程并将所有课程写入文件

	bool SelectNCourses(int currentTrem, int n, Queue<int> &currentCourse);
	//选择n个本学期能上的课（先修课程在本学期前修过）

	bool Paike();
	//排课，并将课程表与每学期要上的课程写入文件

private:
	Course *Courses;			//所有课程
	int NumOfCourse[8], Sum;	//各学期的课程数，总课程数
	int NumTermCourses;			//指定开课学期的课程数

	int Two(int start, int &CourseNum, int Schedule[5][2][5]);
	//从星期start开始，在课程表中找到一小节课，并填入CourseNum课程，返回填入的最后一节课在星期几

	int One(int start, int &CourseNum, int Schedule[5][2][5]);
	//从星期start开始，在课程表中找到两小节课，并填入CourseNum课程，返回填入的最后一节课在星期几

	int Three(int start, int &CourseNum, int Schedule[5][2][5]);
	//从星期start开始，在课程表中找到三小节课，并填入CourseNum课程，返回填入的最后一节课在星期几

	bool PaikeOneTerm(int TermNum, int Schedule[5][2][5]);
	//排课（第TermNum学期），并将该学期的课程写入文件

	void Clear(int Schedule[5][2][5]);
	//清空课程表

	void WriteSchedule(int TermNum, int Schedule[5][2][5]);
	//将第TermNum学期的课程表写入文件
};


template <class T>
void Stack<T>::Push(T &elem) {
	StackNode<T> *newNode = new StackNode<T>;
	newNode->data = elem;
	newNode->next = Top;
	Top = newNode;
}

template <class T>
T Stack<T>::Pop() {
	assert(!IsEmpty());
	T tmpData = Top->data;
	StackNode<T> *p = Top;
	Top = Top->next;
	delete p;
	return tmpData;
}

template <class T>
T Stack<T>::GetFirst() {
	assert(!IsEmpty());
	return Top->data;
}

template <class T>
Stack<T>::~Stack() {
	StackNode<T> *p;
	while (!IsEmpty()) {
		p = Top;
		Top = Top->next;
		delete p;
	}
}



template <class T>
void Queue<T>::Push(T &newT) {
	assert(!IsFull());
	rear = (rear + 1) % maxSize;
	elements[rear] = newT;
}

template <class T>
T Queue<T>::Pop() {
	assert(!IsEmpty());
	front = (front + 1) % maxSize;
	return elements[front];
}




bool ScheduleSystem::Input() {
	//输入课程并将所有课程写入文件
	int sum;
	cout << "请输入课程总数:";
	cin >> sum;
	if (sum <= 0) {
		cout << "课程总数应大于零!" << endl;
		return false;
	}
	cout << "请输入各个学期开设的课程总数:";
	for (int i = 0; i < 8; i++) {
		cin >> NumOfCourse[i];
		if (NumOfCourse[i] < 0) {
			cout << "每学期的课程数应大于或等于零!" << endl;
			return false;
		}
		Sum += NumOfCourse[i];
	}
	if (sum != Sum) {
		cout << "每学期课程数的和与课程总数不相等！" << endl;
		return false;
	}
	Courses = new Course[Sum + 1];
	int tmp;
	for (int i = 1; i <= Sum; i++) {
		cout << "请以此输入课程编号，课程名称，周学时数，指定开课学期，先决条件:";
		cin >> Courses[i].Number >> Courses[i].Name >> Courses[i].Hours >> Courses[i].Term;
		if (Courses[i].Term != 0) {			//指定学期
			Courses[i].NumBefore = -1;
			NumTermCourses++;
		}
		else { Courses[i].NumBefore = 0; }
		cin >> tmp;							//每行输入以0结束
		while (tmp > 0) {					//将此课程的先修课程进行处理
			Courses[i].Before.Push(tmp);
			Courses[tmp].After.Push(i);
			Courses[i].NumBefore++;
			cin >> tmp;
		}
	}

	//将所有课程写入文件
	ofstream ofile;
	ofile.open("Courses.txt");
	ofile << setw(20) << "课程编号" << setw(20) << "课程名称" << setw(20) << "学时数" << setw(20) << "指定开课学期" << setw(20) << "先修课程" << endl;
	for (int i = 1; i <= Sum; i++) {
		ofile << setw(20) << Courses[i].Number << setw(20) << Courses[i].Name << setw(20) << Courses[i].Hours << setw(20) << Courses[i].Term << setw(20);
		while (!Courses[i].Before.IsEmpty()) { ofile << Courses[Courses[i].Before.Pop()].Number << ' '; }
		ofile << endl;
	}
	ofile.close();
	cout << endl;
	return true;
}

bool ScheduleSystem::SelectNCourses(int currentTrem, int n, Queue<int> &currentCourse) {
	//选择n个本学期能上的课（先修课程在本学期前修过）
	int count = 0, tmp;
	int NoTermCourses = Sum - NumTermCourses;
	for (int i = NoTermCourses + 1; i <= Sum; i++) {
		//首先扫描指定开课学期的课程
		if (Courses[i].Term == currentTrem) {
			if (count >= n) {
				cout << "第" << currentTrem << "学期课程数过少，小于指定在该学期上的课程数！" << endl;
				return false;
			}
			currentCourse.Push(i);
			Courses[i].NumBefore--;
			count++;
			//if (count == n) { break; }
		}//if
	}//for
	for (int i = 1; i <= NoTermCourses; i++) {
		//再扫描不指定开课学期的课程
		if (Courses[i].NumBefore == 0) {
			if (count >= n) { break; }
			currentCourse.Push(i);
			Courses[i].NumBefore--;
			count++;
		}//if
	}//for
	if (count != n) {
		cout << "每学期的课程数不合理,第" << currentTrem << "学期没有这么多可以上的课" << endl;
		return false;
	}
	for (int j = 0; j < n; j++) {
		//将选出的课程的后继课程的前驱课程数减1
		tmp = currentCourse.Pop();
		while (!Courses[tmp].After.IsEmpty()) {
			Courses[Courses[tmp].After.Pop()].NumBefore--;
		}//while
		currentCourse.Push(tmp);
	}//for
	return true;
}

bool ScheduleSystem::Paike() {
	//排课，并将课程表与每学期要上的课程写入文件
	int Schedule[5][2][5];	//课程表
	for (int i = 1; i <= 8; i++) {
		//分别排每学期的课
		Clear(Schedule);
		if (!PaikeOneTerm(i, Schedule)) {
			return false;
		}
		WriteSchedule(i, Schedule);
	}
	return true;
}

int ScheduleSystem::Three(int start, int &CourseNum, int Schedule[5][2][5]) {
	//从星期start开始，在课程表中找到三小节课，并填入CourseNum课程，返回填入的最后一节课在星期几
	int hours = Courses[CourseNum].Hours;
	int weekday = start;
	for (int i = 0; i < 5; i++) {
		for (int k = 0; k < 2; k++) {
			if (Schedule[weekday][k][2] == 0) {
				//首先搜索连着的三节小课
				Schedule[weekday][k][2] = Schedule[weekday][k][3] = Schedule[weekday][k][4] = CourseNum;
				Courses[CourseNum].Hours -= 3;
				return weekday;
			}//if
		}//for
		weekday = (weekday + 1) % 5;
	}//for

	//执行到此处说明没有连着的三节小课，接下来搜索2+1或1+1+1
	int tmp = Two(start, CourseNum, Schedule);
	if (tmp >= 0) {
		tmp = One((tmp + 2) % 5, CourseNum, Schedule);
		if (tmp >= 0) { return tmp; }
	}
	return -1;		//执行到此处说明课程表已满，没有搜索到空位
}

int ScheduleSystem::One(int start, int &CourseNum, int Schedule[5][2][5]) {
	//从星期start开始，在课程表中找到一小节课，并填入CourseNum课程，返回填入的最后一节课在星期几
	int weekday = start;
	for (int i = 0; i < 5; i++) {
		for (int k = 0; k < 2; k++) {
			//首先所搜索被占用了一节小课的两节小课（上下午第一节大课）
			if (Schedule[weekday][k][0] != 0 && Schedule[weekday][k][1] == 0) {
				Schedule[weekday][k][1] = CourseNum;
				Courses[CourseNum].Hours -= 1;
				return weekday;
			}//if
			//再所搜索被占用了两节小课的三节小课（上下午第二节大课）
			else if (Schedule[weekday][k][2] != 0 && Schedule[weekday][k][3] != 0 && Schedule[weekday][k][4] == 0) {
				Schedule[weekday][k][4] = CourseNum;
				Courses[CourseNum].Hours -= 1;
				return weekday;
			}
		}//for
		weekday = (weekday + 1) % 5;
	}//for
	for (int i = 0; i < 5; i++) {
		for (int k = 0; k < 2; k++) {
			//再所搜索被占用了一节小课的三节小课（上下午第二节大课）
			if (Schedule[weekday][k][2] != 0 && Schedule[weekday][k][3] == 0) {
				Schedule[weekday][k][3] = CourseNum;
				Courses[CourseNum].Hours -= 1;
				return weekday;
			}//if
		}//for
		weekday = (weekday + 1) % 5;
	}//for
	for (int i = 0; i < 5; i++) {
		for (int k = 0; k < 2; k++) {
			//再所搜索没有被占用的两节小课（上下午第一节大课）
			if (Schedule[weekday][k][0] == 0) {
				Schedule[weekday][k][0] = CourseNum;
				Courses[CourseNum].Hours -= 1;
				return weekday;
			}//if
		}//for
		weekday = (weekday + 1) % 5;
	}//for
	return -1;	//执行到此处说明课程表已满，没有搜索到空位
}

int ScheduleSystem::Two(int start, int &CourseNum, int Schedule[5][2][5]) {
	//从星期start开始，在课程表中找到两小节课，并填入CourseNum课程，返回填入的最后一节课在星期几
	int hours = Courses[CourseNum].Hours;
	int weekday = start;
	for (int i = 0; i < 5; i++) {
		for (int k = 0; k < 2; k++) {
			//首先所搜索上下午的第一节大课
			if (Schedule[weekday][k][0] == 0 && Schedule[weekday][k][1] == 0) {
				Schedule[weekday][k][0] = Schedule[weekday][k][1] = CourseNum;
				Courses[CourseNum].Hours -= 2;
				return weekday;
			}//if
		}//for
		weekday = (weekday + 1) % 5;
	}//for
	for (int i = 0; i < 5; i++) {
		for (int k = 0; k < 2; k++) {
			//首先所搜索上下午的第二节大课
			if (Schedule[weekday][k][2] == 0) {
				Schedule[weekday][k][2] = Schedule[weekday][k][3] = CourseNum;
				Courses[CourseNum].Hours -= 2;
				return weekday;
			}//if
			else if (Schedule[weekday][k][3] == 0) {
				Schedule[weekday][k][3] = Schedule[weekday][k][4] = CourseNum;
				Courses[CourseNum].Hours -= 2;
				return weekday;
			}//else if
		}//for
		weekday = (weekday + 1) % 5;
	}//for
	for (int i = 0; i < 5; i++) {
		for (int k = 0; k < 2; k++) {
			//执行到此处说明没有连着的两节小课，再搜索找两节单独的小课
			if (Schedule[weekday][k][1] == 0) {
				Schedule[weekday][k][1] = CourseNum;
				Courses[CourseNum].Hours -= 1;
				if (hours - Courses[CourseNum].Hours == 2) { return weekday; }
			}//if
			if (Schedule[weekday][k][4] == 1) {
				Schedule[weekday][k][4] = CourseNum;
				Courses[CourseNum].Hours -= 1;
				if (hours - Courses[CourseNum].Hours == 2) { return weekday; }
			}//if
		}//for
		weekday = (weekday + 1) % 5;
	}//for
	return -1;		//执行到此处说明课程表已满，没有搜索到空位
}

bool ScheduleSystem::PaikeOneTerm(int TermNum, int Schedule[5][2][5]) {
	//排课（第TermNum学期），并将该学期的课程写入文件
	Queue<int> CurrentTermCourse(NumOfCourse[TermNum - 1]);
	if (!SelectNCourses(TermNum, NumOfCourse[TermNum - 1], CurrentTermCourse)) { return false; }
	ofstream ofile;
	if (TermNum == 1) {
		ofile.open("CoursesByTerm.txt");
	}
	else {
		ofile.open("CoursesByTerm.txt", ios::app);
	}
	ofile << "第" << TermNum << "学期的课程" << endl;
	int tmp, Cour;
	for (int i = 0; i < NumOfCourse[TermNum - 1]; i++) {
		Cour = CurrentTermCourse.Pop();
		ofile << Courses[Cour].Number << ' ' << Courses[Cour].Name << ' ' << Courses[Cour].Hours << ' ' << Courses[Cour].Term << ' ';
		while (!Courses[Cour].Before.IsEmpty()) { ofile << Courses[Courses[Cour].Before.Pop()].Number << ' '; }
		ofile << endl;
		tmp = -2;
		while (Courses[Cour].Hours > 0) {
			if (Courses[Cour].Hours % 3 == 0 || Courses[Cour].Hours % 2 == 1) {
				tmp = Three((tmp + 2) % 5, Cour, Schedule);
				if (tmp < 0) {
					cout << "第" << TermNum << "学期课太多，排不开" << endl;
					return false;
				}
			}
			else {
				tmp = Two((tmp + 2) % 5, Cour, Schedule);
				if (tmp < 0) {
					cout << "第" << TermNum << "学期课太多，排不开" << endl;
					return false;
				}
			}//if
		}//while
	}
	ofile << endl;
	ofile.close();
	return true;
}

void ScheduleSystem::Clear(int Schedule[5][2][5]) {
	//清空课程表
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 5; k++) {
				Schedule[i][j][k] = 0;
			}
		}
	}
}

void ScheduleSystem::WriteSchedule(int TermNum, int Schedule[5][2][5]) {
	//将第TermNum学期的课程表写入文件
	ofstream ofile;
	if (TermNum == 1) {
		ofile.open("Schedule.txt");
	}
	else {
		ofile.open("Schedule.txt", ios::app);
	}
	ofile << setw(45) << "第" << TermNum << "学期课程表" << endl;
	for (int i = 0; i < 5; i++) {
		ofile << setw(5) << "|";
		for (int j = 0; j < 5; j++) {
			if (Schedule[j][0][i] != 0) {
				ofile << setw(15) << Courses[Schedule[j][0][i]].Name << setw(5) << "|";
			}
			else {
				ofile << setw(15) << "无" << setw(5) << "|";
			}
		}
		ofile << endl;
	}
	for (int i = 0; i < 5; i++) {
		ofile << setw(5) << "|";
		for (int j = 0; j < 5; j++) {
			if (Schedule[j][1][i] != 0) {
				ofile << setw(15) << Courses[Schedule[j][1][i]].Name << setw(5) << "|";
			}
			else {
				ofile << setw(15) << "无" << setw(5) << "|";
			}
		}
		ofile << endl;
	}
	ofile << endl << endl;
	ofile.close();
}



int main() {
	cout << "为便于测试，我已将应输入的所有内容保存在'输入.txt'，只需复制全部内容并粘贴到exe" << endl;
	ScheduleSystem s;
	bool b;
	b = s.Input();			//输入课程
	if (b) {
		cout << "输入成功！" << endl;
		cout << "全部课程已写入文件 Courses.txt" << endl << endl;;
	}
	else {
		cout << "输入失败！" << endl;
		system("pause");
		return 0;
	}
	b = s.Paike();			//排课
	if (b) {
		cout << "排课成功!" << endl;
		cout << "每学期的课程表已写入文件 Schedule.txt" << endl;
		cout << "每学期的课程已写入文件 CoursesByTerm.txt" << endl;
	}
	else {
		cout << "排课失败" << endl;
	}
	system("pause");
	return 0;
}