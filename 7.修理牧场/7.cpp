#include <iostream>
#include <assert.h>
using namespace std;

template <class T>
class MinHeap {
public:
	MinHeap(int size) :MaxSize(size), CurrentSize(0) { Heap = new T[size]; }
	~MinHeap() { delete[]Heap; }
	
	void FliterDown();
	//向下调整

	void FliterUp();
	//向上调整
	
	void Insert(T &x);
	//插入新元素
	
	T Pop();
	//弹出堆顶元素

private:
	T *Heap;
	//堆的元素

	int MaxSize, CurrentSize;
	//堆的最大容量，当前容量

};

template <class T>
void MinHeap<T>::FliterDown() {
	assert(CurrentSize > 0);
	T temp = Heap[0];
	int Parent = 0, LeftChild = 1, LittleChild;
	while (LeftChild <= CurrentSize - 1) {
		//选出较小的孩子
		if (LeftChild + 1 <= CurrentSize - 1 && Heap[LeftChild] > Heap[LeftChild + 1]) { 
			LittleChild = LeftChild + 1; 
		}
		else { LittleChild = LeftChild; }
		//如果较小的孩子小于要调整的元素则调换位置
		if (Heap[LittleChild] < temp) { Heap[Parent] = Heap[LittleChild]; }
		else { break; }
		LeftChild = 2 * LittleChild + 1;
		Parent = LittleChild;
	}
	Heap[Parent] = temp;
}

template <class T>
void MinHeap<T>::FliterUp() {
	assert(CurrentSize > 0);
	int Parent = CurrentSize - 1, CurrentLoc;
	T temp = Heap[Parent];	//temp为要调整的元素（最后一个元素）
	while (Parent > 0) {
		CurrentLoc = Parent;
		Parent = (CurrentLoc - 1) / 2;
		if (temp < Heap[Parent]) { 
			Heap[CurrentLoc] = Heap[Parent];
		}
		else {
			Parent = 1;
			Heap[CurrentLoc] = temp;
			break;
		}
	}
	if (Parent == 0) { Heap[0] = temp; }
}

template <class T>
void MinHeap<T>::Insert(T &x) {
	assert(CurrentSize < MaxSize);
	Heap[CurrentSize++] = x;
	FliterUp();
}

template <class T>
T MinHeap<T>::Pop() {
	assert(CurrentSize > 0);
	T temp = Heap[0];
	Heap[0] = Heap[--CurrentSize];
	if (CurrentSize != 0) { FliterDown(); }
	return temp;
}

int main() {
	int n, Length;
	cout << "请输入木头的块数：";
	cin >> n;
	assert(n > 0);
	MinHeap<int> Lengthes(n);
	cout << "请输入木头的长度：";
	for (int i = 0; i < n; i++) {
		cin >> Length;
		Lengthes.Insert(Length);
	}
	int Cost = 0,currentCost;
	for (int i = 1; i < n; i++) {
		currentCost = Lengthes.Pop() + Lengthes.Pop();
		Lengthes.Insert(currentCost);
		Cost += currentCost;
	}
	cout << "最小费用为：" << Cost << "元" << endl;
	system("pause");
	return 0;
}
