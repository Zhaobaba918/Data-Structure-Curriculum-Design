#include <assert.h>
#include <iostream>
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


class Maze {
public:
	Maze();										//包含迷宫的建立
	~Maze();
	void Go();									//寻找有效路径并输出（走迷宫）
private:
	char **Map;									//二维字符数组存储迷宫
	bool **sign;								//二维布尔数组存储每个节点是否访问过
	int row, col;								//迷宫的行数，列数
	void showMap() const;						//打印迷宫
	void Go(int &x1, int &y1, int &x2, int &y2);	//被Go()调用
};


Maze::Maze() {
	cout << "首先请建立迷宫！" << endl;
	cout << "请输入迷宫的行数与列数：";
	cin >> row >> col;
	if (row < 2 || col < 2) {
		cout << "行数与列数至少为2！" << endl;
		system("pause");
		exit(0);
	}//if
	cout << "无障碍用 0 表示，有障碍用 # 表示" << endl;
	row += 2, col += 2;
	Map = new char*[row];
	Map[0] = new char[col];
	Map[row - 1] = new char[col];
	for (int i = 0; i < col; i++) { Map[0][i] = Map[row - 1][i] = '#'; }
	for (int i = 0; i < row - 2; i++) {
		Map[i + 1] = new char[col];
		Map[i + 1][0] = Map[i + 1][col - 1] = '#';
		cout << "请输入迷宫的第" << i + 1 << "行：";;
		for (int j = 0; j < col - 2; j++) {
			cin >> Map[i + 1][j + 1];
		}//for
	}//for
	sign = new bool*[7];
	for (int i = 0; i < 7; i++) {
		sign[i] = new bool[7];
		for (int j = 0; j < 7; j++) {
			sign[i][j] = false;
		}//for
	}//for
	system("cls");
}


void Maze::Go() {
	showMap();
	int x1, y1, x2, y2;
	cout << "请输入起点与终点坐标：";
	cin >> x1 >> y1 >> x2 >> y2;
	Go(x1, y1, x2, y2);
}

void Maze::Go(int &x1, int &y1, int &x2, int &y2) {
	//(x1,y1)为起点，（x2,y2）为终点
	if (x1 >= row || x2 >= row || y1 >= col || y2 >= col
		|| x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0
		|| Map[x1][y1] != '0' || Map[x2][y2] != '0' || ((x1 == x2) && (y1 == y2))) {
		cout << endl << "*** 非法的起点或终点!!! ***" << endl << endl;			//起点终点左边不合理给出提示
		return;
	}//if
	Stack<int> Route;								//用于存储查找路径，将点的坐标转化为一维处理
	int current = x1 * col + y1, goal = x2 * col + y2;//当前访问的节点、目标节点的一维坐标
	Route.Push(current);
	while (!Route.IsEmpty()) {
		current = Route.GetFirst();					//从当前栈顶元素开始查找		
		if (current == goal) { break; }				//到达终点
		x1 = current / col, y1 = current % col;		//转化为二维坐标
		sign[x1][y1] = true;						//当前节点标记为已访问
		if (Map[x1][y1 + 1] == '0'&&sign[x1][y1 + 1] == false) { y1++; }//寻找下一个可以到达且未访问的点
		else if (Map[x1][y1 - 1] == '0'&&sign[x1][y1 - 1] == false) { y1--; }
		else if (Map[x1 + 1][y1] == '0'&&sign[x1 + 1][y1] == false) { x1++; }
		else if (Map[x1 - 1][y1] == '0'&&sign[x1 - 1][y1] == false) { x1--; }
		else { Route.Pop(); continue; }	//从当前节点无路可走，将当前节点从栈中弹出（下一次循环进行回溯）
		current = x1 * col + y1;		//寻找下一个可以到达且未访问的点得坐标转化为一维
		Route.Push(current);			//入栈
	}//while
	if (current != goal) { cout << "没有可行的路径!" << endl; }//该情况下回溯到起点，Route为空
	else {												//该情况下找到了有效路径
		Stack<int> trueRoute;							//该栈用于将Route倒置
		while (!Route.IsEmpty()) {
			current = Route.Pop();
			Map[current / col][current%col] = '*';		//标记路径
			trueRoute.Push(current);
		}//while
		system("cls");
		showMap();
		//一下内容为打印路径
		current = trueRoute.Pop();
		cout << "迷宫路径：" << endl << endl << "(" << current / col << ',' << current % col << ")";//
		while (!trueRoute.IsEmpty()) {
			current = trueRoute.Pop();
			cout << "->(" << current / col << ',' << current % col << ")";
		}
		cout << endl << endl;
	}//else
}


Maze::~Maze() {
	for (int i = 0; i < row; i++) {
		delete[]Map[i];
		delete[]sign[i];
	}
	delete[]Map;
	delete[]sign;
}

void Maze::showMap() const {				//打印迷宫地图
	cout << "迷宫地图 ：" << endl;
	int realCol = col - 2, realRow = row - 2;
	cout << "        ";
	for (int i = 1; i <= realCol; i++) {
		cout << "\t第" << i << "列";
	}
	cout << endl << endl;
	for (int j = 0; j <= col; j++) { cout << '\t' << Map[0][j]; }
	cout << endl << endl;
	for (int i = 1; i <= realRow; i++) {
		cout << "第" << i << "行";
		for (int j = 0; j <= col; j++) { cout << '\t' << Map[i][j]; }
		cout << endl << endl;
	}
	for (int j = 0; j <= col; j++) { cout << '\t' << Map[row - 1][j]; }
	cout << endl << endl;
}

int main() {
	cout << "为便于测试，我已将应输入的所有内容保存在'输入.txt'，只需复制全部内容并粘贴到exe" << endl;
	Maze maze1;
	maze1.Go();
	system("pause");
	return 0;
}