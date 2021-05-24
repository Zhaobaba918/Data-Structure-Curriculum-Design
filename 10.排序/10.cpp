#include <iostream>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <iomanip>
using namespace std;




template <class T>
class Sortor {
public:
	Sortor();
	~Sortor() { delete[]m_DataList; }

	void BubbleSort() const;
	//冒泡排序

	void SelectSort() const;
	//选择排序
	
	void InsertionSort() const;
	//直接插入排序
	
	void ShellSort() const;
	//希尔排序
	
	void QuickSort() const;
	//快速排序
	
	void HeapSort() const;
	//堆排序
	
	void MergeSort() const;
	//归并排序
	
	void RadixSort() const;
	//基数排序

private:
	T *m_DataList;
	//数据表

	int m_Count;
	//排序元素个数
	
	T* Copy() const;
	//数据表复制函数（不带哨兵）
	
	T* Copy(int guard) const;
	//数据表复制函数（带哨兵），用于直接插入排序
	
	void Swap(T *list, const int x, const int y,long long &swaptime)const;
	//交换
	
	void QuickSort(T *DataList,int left,int right, long long &swaptime, long long &ConTime) const;
	//用于快速排序
	
	void FliterDown(T* Heap,int start,int end, long long &swaptime, long long &ConTime) const;
	//堆的向下调整，用于堆排序

	void Merge(T* oldData, T* newData, int Acurrent, int Bcurrent, int Bhigh, long long &swaptime, long long &ConTime)const;
	//用于归并排序

};

template <class T>
Sortor<T>::Sortor() {
	srand(time(0));
	cout << "请输入随机数个数：";
	cin >> m_Count;
	m_DataList = new T[m_Count];
	for (int i=0;i<m_Count;i++){
		m_DataList[i] = rand();
	}
	cout << endl;
}

template <class T>
void Sortor<T>::Swap(T *list, const int x, const int y,long long &swaptime)const {
	T tmp = list[x];
	list[x] = list[y];
	list[y] = tmp;
	swaptime += 3;
}

template <class T>
T* Sortor<T>::Copy() const {
	T *newData = new T[m_Count];
	for (int i = 0; i < m_Count; i++) {
		newData[i] = m_DataList[i];
	}
	return newData;
}

template <class T>
T* Sortor<T>::Copy(int guard) const {
	T *newData = new T[m_Count + 1];
	for (int i = 1; i <= m_Count; i++) {
		newData[i] = m_DataList[i-1];
	}
	return newData;
}

template <class T>
void Sortor<T>::BubbleSort() const {
	T* DataList = Copy();
	long long SwapTime = 0, ConTime = 0;
	clock_t start, end;
	start = clock();
	bool exchangeTag = false;
	for (int i = m_Count - 1; i > 0; i--) {
		int end = m_Count - 1 - i;
		for (int j = m_Count - 1; j > end; j--) {
			if (ConTime++,DataList[j - 1] > DataList[j]) {
				Swap(DataList, j - 1, j, SwapTime);
				exchangeTag = true;
			}//if
		}//for
		if (!exchangeTag) { break; }
		exchangeTag = false;
	}//for
	end = clock();
	delete[]DataList;
	cout << "比较次数：" << ConTime << endl;
	cout << "交换次数：" << SwapTime << endl;
	cout << "运行时间：" << end - start << endl;
}

template <class T>
void Sortor<T>::SelectSort() const {
	T* DataList = Copy();
	long long SwapTime = 0, ConTime = 0;
	clock_t start, end;
	start = clock();
	for (int i = 1; i < m_Count; i++) {
		T min = DataList[i - 1];
		int minNum = i - 1;
		for (int j = i; j < m_Count; j++) {
			if (ConTime++, DataList[i] < min) {
				min = DataList[i];
				minNum = i;
			}//if
		}//for
		Swap(DataList, i - 1, minNum,SwapTime);
	}
	end = clock();
	delete[]DataList;
	cout << "比较次数：" << ConTime << endl;
	cout << "交换次数：" << SwapTime << endl;
	cout << "运行时间：" << end - start << endl;
}

template <class T>
void Sortor<T>::InsertionSort() const {
	T* DataList = Copy(1);
	long long SwapTime = 0, ConTime = 0;
	clock_t start, end;
	start = clock();
	for (int i = 2; i <= m_Count; i++) {
		DataList[0] = DataList[i];
		int icpy = i;
		while(ConTime++,DataList[--icpy]> DataList[0]){}
		for (int j = i - 1; j > icpy; j--) { 
			DataList[j + 1] = DataList[j];
			SwapTime++;
		}
		DataList[icpy + 1] = DataList[0];
		SwapTime++;
	}//for
	end = clock();
	delete[]DataList;
	cout << "比较次数：" << ConTime << endl;
	cout << "交换次数：" << SwapTime << endl;
	cout << "运行时间：" << end - start << endl;
}

template <class T>
void Sortor<T>::ShellSort() const {
	T* DataList = Copy();
	long long SwapTime = 0, ConTime = 0;
	clock_t start, end;
	start = clock();
	int gap = m_Count / 2;
	while (gap) {
		for (int i = gap; i < m_Count; i++) {
			T tmp = DataList[i];
			int icpy = i - gap;
			while (ConTime++, DataList[icpy] > tmp&&icpy >= 0) {
				DataList[icpy + gap] = DataList[icpy];
				SwapTime++;
				icpy -= gap;
			}//while
			DataList[icpy + gap] = tmp;
		}//for
		gap = gap == 2 ? 1 : (int)gap / 2.2;
	}//while
	end = clock();
	delete[]DataList;
	cout << "比较次数：" << ConTime << endl;
	cout << "交换次数：" << SwapTime << endl;
	cout << "运行时间：" << end - start << endl;
}

template <class T>
void Sortor<T>::QuickSort(T *DataList, int left, int right, long long &swaptime, long long &ConTime) const {
	T tmp = DataList[left];
	int small=left, big=right+1;
	bool tag = true;
	while (big > small) {
		switch (tag){
		case true:
			while(ConTime++,DataList[--big]>=tmp){}
			if (big > small) {
				DataList[small] = DataList[big];
				swaptime++;
				tag = false;
			}
			else { 
				DataList[small] = tmp;
				swaptime++;
				if (small - left > 1) { QuickSort(DataList, left, small - 1, swaptime, ConTime); }
				if (right - small > 1) { QuickSort(DataList, small + 1, right, swaptime, ConTime); }
			}
			break;
		default:
			while (ConTime++, DataList[++small] <= tmp) {}
			if (big > small) {
				DataList[big] = DataList[small];
				swaptime++;
				tag = true;
			}
			else { 
				DataList[big] = tmp;
				swaptime++;
				if (big - left > 1) { QuickSort(DataList, left, big - 1, swaptime, ConTime); }
				if (right - big > 1) { QuickSort(DataList, big + 1, right, swaptime, ConTime); }
			}
			break;
		}
	}
}

template <class T>
void Sortor<T>::QuickSort() const {
	T* DataList = Copy();
	long long SwapTime = 0, ConTime = 0;
	clock_t start, end;
	start = clock();
	QuickSort(DataList, 0, m_Count - 1, SwapTime, ConTime);
	end = clock();
	delete[]DataList;
	cout << "比较次数：" << ConTime << endl;
	cout << "交换次数：" << SwapTime << endl;
	cout << "运行时间：" << end - start << endl;
}

template <class T>
void Sortor<T>::FliterDown(T* Heap, int start, int end, long long &swaptime, long long &ConTime)const {
	if (2 * start + 1 > end) { return; }
	T temp = Heap[start];
	int Parent = start, LeftChild = 2*start+1, BigChild;
	while (LeftChild <= end) {
		//选出较大的孩子
		if (LeftChild + 1 <= end && (ConTime++,Heap[LeftChild] < Heap[LeftChild + 1])) {
			BigChild = LeftChild + 1;
		}
		else { BigChild = LeftChild; }
		//如果较小的孩子小于要调整的元素则调换位置
		if (ConTime++, Heap[BigChild] > temp) { 
			Heap[Parent] = Heap[BigChild];
			swaptime++;
		}
		else { break; }
		LeftChild = 2 * BigChild + 1;
		Parent = BigChild;
	}
	Heap[Parent] = temp;
}

template <class T>
void Sortor<T>::HeapSort() const {
	T* DataList = Copy();
	long long SwapTime = 0, ConTime = 0;
	clock_t start, end;
	start = clock();
	for (int i = 0; i < m_Count; i++) {
		FliterDown(DataList, m_Count - 1 - i, m_Count - 1, SwapTime, ConTime);
	}
	for (int i = m_Count - 1; i > 0; i--) {
		Swap(DataList, 0, i,SwapTime);
		FliterDown(DataList, 0, i - 1, SwapTime, ConTime);
	}
	end = clock();
	delete[]DataList;
	cout << "比较次数：" << ConTime << endl;
	cout << "交换次数：" << SwapTime << endl;
	cout << "运行时间：" << end - start << endl;
}

template <class T>
void Sortor<T>::Merge(T* oldData, T* newData, int Acurrent, int Bcurrent, int Bhigh,long long &swaptime, long long &ConTime)const {
	int Ahigh = Bcurrent - 1, newCurrent = Acurrent;
	while (Acurrent <= Ahigh && Bcurrent <= Bhigh) {
		if (ConTime++, oldData[Acurrent] <= oldData[Bcurrent]) {
			newData[newCurrent++] = oldData[Acurrent++];
			swaptime++;
		}
		else {
			newData[newCurrent++] = oldData[Bcurrent++];
			swaptime++;
		}
	}//while
	if (Acurrent > Ahigh) {
		for (int i = Bhigh - Bcurrent + 1; i > 0; i--) {
			newData[newCurrent++] = oldData[Bcurrent++];
			swaptime++;
		}
	}//if
	else {
		for (int i = Ahigh - Acurrent + 1; i > 0; i--) {
			newData[newCurrent++] = oldData[Acurrent++];
			swaptime++;
		}
	}//else
}

template <class T>
void Sortor<T>::MergeSort() const {
	T* DataList1 = Copy();
	T* DataList2 = new T[m_Count];
	T *Old, *New;
	long long SwapTime = 0, ConTime = 0, sortedNum;
	clock_t start, end;
	start = clock();
	bool List1Emp = false;
	for (int len = 1; len < m_Count; len *= 2) {
		sortedNum = 0;
		if (List1Emp) {
			Old = DataList2;
			New = DataList1;
			List1Emp = false;
		}
		else {
			New = DataList2;
			Old = DataList1;
			List1Emp = true;
		}
		while (sortedNum + 2 * len <= m_Count) {
			Merge(Old, New, sortedNum, sortedNum + len, sortedNum + 2 * len - 1, SwapTime, ConTime);
			sortedNum += 2 * len;
		}//while
		if (m_Count - sortedNum > len) {
				Merge(Old, New, sortedNum, sortedNum + len, m_Count - 1, SwapTime, ConTime);
		}//if
	}
	end = clock();
	cout << "比较次数：" << ConTime << endl;
	cout << "交换次数：" << SwapTime << endl;
	cout << "运行时间：" << end - start << endl;
	delete[]DataList1;
	delete[]DataList2;
}

template <class T>
void Sortor<T>::RadixSort() const {
	class Node {
	public:
		T data;
		int link;
	};
	Node *DataList = new Node[m_Count];
	for (int i = 0; i < m_Count; i++) {
		DataList[i].data = m_DataList[i];
		DataList[i].link = i + 1;
	}
	DataList[m_Count - 1].link = 0;
	long long SwapTime = 0, ConTime = 0;
	clock_t start, end;
	start = clock();
	const int Radix = 10;
	int rear[Radix], front[Radix], n = 1, Max = 32767, allFront = 0, current = 0;
	while ((Max /= 10) > 0) { 
		n++; 
	}
	int tmp = 1, currentUnit;
	for (int i = 0; i < n; i++) {
		for (int i = 0; i < Radix; i++) {
			rear[i] = front[i] = -1;
		}
		for (int j = 0; j < m_Count; j++) {
			currentUnit = DataList[current].data / tmp % 10;
			if (front[currentUnit] == -1) { front[currentUnit] = rear[currentUnit] = current; }
			else {
				DataList[rear[currentUnit]].link = current;
				rear[currentUnit] = current;
			}
			current = DataList[current].link;
		}
		int lastFull = 0;
		bool first = true;
		for (int j = 0; j < Radix; j++) {
			if (front[j] != -1) {
				if (first) { 
					allFront = front[j];
					lastFull = j;
					first = false;
				}
				DataList[rear[lastFull]].link = front[j]; 
				lastFull = j; 
			}
		}
		tmp *= 10;
	}
	end = clock();
	delete[]DataList;
	cout << "比较次数：" << ConTime << endl;
	cout << "交换次数：" << SwapTime << endl;
	cout << "运行时间：" << end - start << endl;
}

void Menu() {
	cout << "**      排序算法比较      **" << endl;
	cout << "============================" << endl;
	cout << "**      1-冒泡排序        **" << endl;
	cout << "**      2-选择排序        **" << endl;
	cout << "**      3-直接插入排序    **" << endl;
	cout << "**      4-希尔排序        **" << endl;
	cout << "**      5-快速排序        **" << endl;
	cout << "**      6-堆排序          **" << endl;
	cout << "**      7-归并排序        **" << endl;
	cout << "**      8-基数排序        **" << endl;
	cout << "**      9-退出程序        **" << endl;
	cout << "============================" << endl;
	cout << endl;
}


int main(){
	Menu();
	Sortor<int> mySortor;
	int operation=0;
	while (operation!=9) {
		cout << "请选择排序算法：";
		cin >> operation;
		switch (operation) {
		case 1:
			cout << "冒泡排序" << endl;
			mySortor.BubbleSort();
			break;
		case 2:
			cout << "选择排序" << endl;
			mySortor.SelectSort();
			break;
		case 3:
			cout << "直接插入排序" << endl;
			mySortor.InsertionSort();
			break;
		case 4:
			cout << "希尔排序" << endl;
			mySortor.ShellSort();
			break;
		case 5:
			cout << "快速排序" << endl;
			mySortor.QuickSort();
			break;
		case 6:
			cout << "堆排序" << endl;
			mySortor.HeapSort();
			break;
		case 7:
			cout << "归并排序" << endl;
			mySortor.MergeSort();
			break;
		case 8:
			cout << "基数排序" << endl;
			mySortor.RadixSort();
			break;
		default:
			operation = 9;
			break;
		}
	}//while	
	system("pause");
	return 0;
}