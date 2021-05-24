#include <iostream>
#include <assert.h>
using namespace std;

template <class T>
class MinHeap {
public:
	MinHeap(int size) :MaxSize(size), CurrentSize(0) { Heap = new T[size]; }
	~MinHeap() { delete[]Heap; }
	
	void FliterDown();
	//���µ���

	void FliterUp();
	//���ϵ���
	
	void Insert(T &x);
	//������Ԫ��
	
	T Pop();
	//�����Ѷ�Ԫ��

private:
	T *Heap;
	//�ѵ�Ԫ��

	int MaxSize, CurrentSize;
	//�ѵ������������ǰ����

};

template <class T>
void MinHeap<T>::FliterDown() {
	assert(CurrentSize > 0);
	T temp = Heap[0];
	int Parent = 0, LeftChild = 1, LittleChild;
	while (LeftChild <= CurrentSize - 1) {
		//ѡ����С�ĺ���
		if (LeftChild + 1 <= CurrentSize - 1 && Heap[LeftChild] > Heap[LeftChild + 1]) { 
			LittleChild = LeftChild + 1; 
		}
		else { LittleChild = LeftChild; }
		//�����С�ĺ���С��Ҫ������Ԫ�������λ��
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
	T temp = Heap[Parent];	//tempΪҪ������Ԫ�أ����һ��Ԫ�أ�
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
	cout << "������ľͷ�Ŀ�����";
	cin >> n;
	assert(n > 0);
	MinHeap<int> Lengthes(n);
	cout << "������ľͷ�ĳ��ȣ�";
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
	cout << "��С����Ϊ��" << Cost << "Ԫ" << endl;
	system("pause");
	return 0;
}
