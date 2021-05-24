#include <iostream>
#include <assert.h>
using namespace std;

template<class T>
class Stack;

template<class T>
class Node {
	friend class Stack<T>;
public:
	Node() :Next(NULL) {}
private:
	T Data;
	Node<T> * Next;
};

template<class T>
class Stack {
public:
	Stack() :Top(NULL) {}
	~Stack();
	T getFirst();
	T Pop();
	void Push(T item);
	bool IsEmpty() { return Top == NULL; }
private:
	Node<T> *Top;
};




class Sign {
	friend class Calculator;
public:
	Sign() {};
	Sign(char signChar);
	//初始化优先级

	bool operator<(const Sign & rightSign) { return Priority < rightSign.Priority; }
	//优先级比较

	char getSign() { return m_sign; }
	//取运算符
private:
	char m_sign;
	//运算符

	short Priority;
	//优先级
};

class Calculator {
public:
	Calculator() { SignStack.Push('@'); }
	//将‘@’推入运算符栈（作为哨兵）

	bool Claculate(double &result);
	//输入表达式并计算，返回是否计算成功，result为运算结果
private:
	Stack<double> Number;
	//存数字

	Stack<Sign> SignStack;
	//存运算符

	bool CalculateOnce();
	//进行一次计算（处理一个运算符）

	double pow(double &base, double &power);
	//幂运算
};



template<class T>
Stack<T>::~Stack() {
	Node<T> *p;
	while (Top) {
		p = Top;
		Top = Top->Next;
		delete p;
	}
}

template<class T>
T Stack<T>::getFirst() {
	assert(Top != NULL);
	return Top->Data;
}

template<class T>
T Stack<T>::Pop() {
	assert(Top != NULL);
	Node<T> *p = Top;
	Top = Top->Next;
	T temp = p->Data;
	delete p;
	return temp;
}

template<class T>
void Stack<T>::Push(T item) {
	if (!Top) {
		Top = new Node<T>;
		Top->Data = item;
		return;
	}
	else {
		Node<T> * newnode = new Node<T>;
		newnode->Data = item;
		newnode->Next = Top;
		Top = newnode;
	}
}



Sign::Sign(char signChar) :m_sign(signChar) {
	switch (signChar)
	{
	case '+':
		Priority = 3;
		break;
	case '-':
		Priority = 3;
		break;
	case '*':
		Priority = 2;
		break;
	case '/':
		Priority = 2;
		break;
	case '%':
		Priority = 2;
		break;
	case '^':
		Priority = 1;
		break;
	case '@':
		Priority = 5;
		break;
	case '(':
		Priority = 4;
		break;
	case ')':
		Priority = 4;
		break;
	default:
		Priority = 6;
		break;
	}
}


bool Calculator::Claculate(double &result) {
	char ch;
	int n = 0;//左括号的个数
	cin >> ch;
	if (ch == '+' || ch == '-') {
		//单目运算符处理
		Number.Push(0);
		SignStack.Push(ch);
		cin >> ch;
	}
	while (1) {
		if (ch >= '0'&&ch <= '9') {
			//数字处理
			double num = 0;
			do { num = num * 10 + ch - 48; } while (cin >> ch, ch >= '0'&&ch <= '9');
			if (ch == '.') {
				double i = 10;
				while (cin >> ch, ch >= '0'&&ch <= '9') {
					num = num + (ch - 48) / i;
					i *= 10;
				}//while
			}//if
			Number.Push(num);
		}//if
		switch (ch) {
		case '(': {
			n++;
			SignStack.Push(ch);
			cin >> ch;
			if (ch == '+' || ch == '-') {
				//单目运算符处理
				Number.Push(0);
				SignStack.Push(ch);
				cin >> ch;
			}
			continue;
		}
		case ')': {
			if (n-- <= 0) {
				cout << "右括号多于左括号！" << endl;
				return false;
			}
			if (SignStack.getFirst().getSign() == '(') {
				cout << "存在空的括号！" << endl;
				return false;
			}
			while (SignStack.getFirst().getSign() != '(') {
				bool b = CalculateOnce();
				if (!b) {
					cout << "不合理的运算符！" << endl;
					return false;
				}
			}
			SignStack.Pop();
			break;
		}
		case '=': {
			if (cin.get() != '\n') {
				cout << "等号位置错误！" << endl;
				return false;
			}
			if (n != 0) {
				cout << "左括号多于右括号！" << endl;
				return false;
			}
			while (SignStack.getFirst().getSign() != '@') {
				bool b = CalculateOnce();
				if (!b) {
					cout << "不合理的运算符！" << endl;
					return false;
				}
			}
			result = Number.Pop();
			return true;
		}
		default: {
			Sign sign(ch);
			if (sign.Priority == 6) {				//其他字符的优先级为6
				cout << "存在非法字符！" << endl;
				return false;
			}
			while (!(sign < SignStack.getFirst())) {//当前优先级不高就计算
				bool b = CalculateOnce();
				if (!b) {
					cout << "不合理的运算符！" << endl;
					return false;
				}
			}
			SignStack.Push(sign);
		}
		}//switch
		cin >> ch;
	}
}

bool Calculator::CalculateOnce() {
	if (Number.IsEmpty()) {
		return false;
	}
	double num2 = Number.Pop();
	if (Number.IsEmpty()) {
		return false;
	}
	double num1 = Number.Pop();
	switch (SignStack.Pop().getSign()) {
	case '+':
		Number.Push(num1 + num2);
		break;
	case '-':
		Number.Push(num1 - num2);
		break;
	case '*':
		Number.Push(num1 * num2);
		break;
	case '/':
		Number.Push(num1 / num2);
		break;
	case '%':
		Number.Push((int)num1 % int(num2));
		break;
	case '^':
		Number.Push(pow(num1, num2));
		break;
	}
	return true;
}

double Calculator::pow(double &base, double &power) {
	double result = 1;
	if (base == 0) { return 0; }
	if (power > 0) {
		while (power-- > 0) { result *= base; }
	}
	else if (power < 0) {
		while (power++ < 0) { result /= base; }
	}
	return result;
}




int main() {
	char stop = 'y';
	bool b;
	double result;
	while ('y' == stop) {
		Calculator Calculator1;
		cout << "输入表达式：" << endl;
		b = Calculator1.Claculate(result);
		if (b) {
			cout << result << endl;
		}
		else {
			cin.ignore(1024, '\n');
			cin.sync();
		}
		cout << "是否继续（y，n）？";
		cin >> stop;
	}
	system("pause");
	return 0;
}