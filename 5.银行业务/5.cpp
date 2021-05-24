#include <iostream>
#include <assert.h>
using namespace std;


template <class T>
class Stack {
public:
	Stack() :Top(-1), MaxSize(0) {}
	Stack(int size) :Data(new T[size]), Top(-1), MaxSize(size) {}
	~Stack() { delete[]Data; };

	void Push(T &newT);
	T Pop();
private:
	T *Data;
	int Top;
	int MaxSize;
};





template <class T>
class Queue {
public:
	Queue() :front(0), rear(0), maxSize(0) {}
	Queue(int size) :elements(new T[size]), front(0), rear(0), maxSize(size) {}
	~Queue() { delete[]elements; };

	void Push(T &newT);
	//入队

	T Pop();
	//出队

	int IsFull() { return (rear + 1) % maxSize == front; }
	//判断队列是否满

	int IsEmpty() { return rear == front; }
	//判断队列是否空

	int getSize() { return maxSize; }
	//返回最多元素个数

private:
	T *elements;
	//队列的元素

	int rear;
	//队尾

	int front;
	//队头

	int maxSize;
	//最多元素个数

};



template <class T>
void Stack<T>::Push(T &newT) {
	assert(Top != MaxSize - 1);
	Data[++Top] = newT;
}

template <class T>
T Stack<T>::Pop() {
	assert(Top != -1);
	return Data[Top--];
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



void Bank() {
	int Num, count, countA = 0, countB;
	//输入的顾客的编号，顾客总数，A、B窗口的顾客数
	cin >> count;
	Queue<int> QueueA(count + 1), QueueB(count + 1);	//创建A、B窗口的队列
	//输入编号并将用户编号入相应的队列
	for (int i = 0; i < count; i++) {
		cin >> Num;
		if (Num % 2 == 1) {
			QueueA.Push(Num);
			countA++;
		}
		else { QueueB.Push(Num); }
	}
	countB = count - countA;
	//按时间顺序输出完成的顾客编号（A窗口处理完一个顾客的时间为时间单位）
	int Time = countA < 2 * countB ? 2 * countB - 1 : countA - 1;
	for (int i = 0; i < Time; i++) {
		if (i % 2 == 0) {
			if (countA-- > 0) { cout << QueueA.Pop() << " "; }
		}
		else {
			if (countA-- > 0) { cout << QueueA.Pop() << " "; }
			if (countB-- > 0) { cout << QueueB.Pop() << " "; }
		}
	}
	//要求输出的最后一个编号后不能有多于的空格，故最后一个编号单独处理
	if (countA > 0 && countB > 0) {
		cout << QueueA.Pop() << " " << QueueB.Pop();
	}
	else {
		if (countA > 0) { cout << QueueA.Pop(); }
		if (countB > 0) { cout << QueueB.Pop(); }
	}
}

int main() {
	Bank();
	cout << endl;
	system("pause");
	return 0;
}