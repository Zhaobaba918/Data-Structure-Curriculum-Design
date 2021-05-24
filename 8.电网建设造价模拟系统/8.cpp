#include <assert.h>
#include <iostream>
#include <string>
using namespace std;



template <class T>
class MinHeap {
public:
	MinHeap() :CurrentSize(0), Heap(NULL) {}
	MinHeap(int size) :MaxSize(size), CurrentSize(0) { Heap = new T[size]; }
	~MinHeap() { delete[]Heap, Heap = NULL; }
	void FliterDown();
	void FliterUp();
	void Insert(T &x);
	bool IsEmpty() { return CurrentSize <= 0; }
	T Pop();
	void SetSize(int size);
private:
	T *Heap;
	int MaxSize, CurrentSize;
};

template <class T>
void MinHeap<T>::SetSize(int size) {
	if (Heap) { delete[]Heap; }
	MaxSize = size;
	CurrentSize = 0;
	Heap = new T[size];
}

template <class T>
void MinHeap<T>::FliterDown() {
	assert(CurrentSize > 0);
	T temp = Heap[0];
	int Parent = 0, LeftChild = 1, LittleChild;
	while (LeftChild <= CurrentSize - 1) {
		if (LeftChild + 1 <= CurrentSize - 1 && Heap[LeftChild] > Heap[LeftChild + 1]) { LittleChild = LeftChild + 1; }
		else { LittleChild = LeftChild; }
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
	int Parent = CurrentSize - 1, CurrentLoc = Parent;
	T temp = Heap[Parent];
	while (Parent > 0) {
		CurrentLoc = Parent;
		Parent = (CurrentLoc - 1) / 2;
		if (temp < Heap[Parent]) { Heap[CurrentLoc] = Heap[Parent]; }
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






class UFSets {
public:
	UFSets() :m_size(0), parent(NULL) {}
	UFSets(int Size);
	~UFSets() { delete[]parent; }
	void Union(int root1, int root2);
	int Find(int x);
	void SetSize(int size);
	bool IsConnected();
private:
	int *parent;
	int m_size;
};

bool UFSets::IsConnected() {
	int root = 0;
	for (int i = 0; i < m_size; i++) {
		if (parent[i] < 0) { root++; }
	}
	if (root > 1) { return false; }
	return true;
}

void UFSets::SetSize(int size) {
	if (parent) { delete[]parent; }
	m_size = size;
	parent = new int[size];
	for (int i = 0; i < m_size; i++) { parent[i] = -1; }
}

UFSets::UFSets(int Size) {
	m_size = Size;
	parent = new int[m_size];
	for (int i = 0; i < m_size; i++) { parent[i] = -1; }
}

int UFSets::Find(int x) {
	if (x >= m_size) { return -1; }
	while (parent[x] >= 0) {
		x = parent[x];
	}
	return x;
}

void UFSets::Union(int root1, int root2) {
	if (parent[root1] > parent[root2]) {
		parent[root1] = root2;
	}
	else if (parent[root1] < parent[root2]) {
		parent[root2] = root1;
	}
	else {
		parent[root1] = root2;
		parent[root2]--;
	}
}



class ElectricityNet;
class Edge {
	friend class ElectricityNet;
public:
	Edge() {};
	Edge(int start, int end, float cost) :Start(start), End(end), Cost(cost) {};
	void Set(int start, int end, float cost) { Start = start, End = end, Cost = cost; }
	bool operator<(const Edge &right) { return Cost < right.Cost; }
	bool operator>(const Edge &right) { return Cost > right.Cost; }
	bool operator==(const Edge &right) { return Cost > right.Cost&&Start == right.Cost&&End == right.End; }
private:
	float Cost;
	//权值

	int Start, End;
	//起点、终点（顶点下标）

};

class ElectricityNet {
public:
	ElectricityNet() :Node(NULL), NeedEdges(NULL) {}
	~ElectricityNet() { delete[]Node, delete[]NeedEdges; }

	void BuildNodes();
	//创建电网顶点

	void AddEdge();
	//添加电网的边

	void BuildTree();
	//构造最小生成树

	void PrintTree();
	//显示最小生成树

	void PrintMenu();
	//输出可选操作列表

private:
	string *Node;
	//节点名称

	MinHeap<Edge> UnUsedEdges;
	//储存未构建生成树的边

	UFSets NodeSet;
	//顶点并查集

	Edge *NeedEdges;
	//生成树中的边

	int NodeNum;
	//顶点数

};

void ElectricityNet::PrintMenu() {
	cout << "**       电网造价模拟系统      **" << endl;
	cout << "=================================" << endl;
	cout << "**       A-创建电网顶点        **" << endl;
	cout << "**       B-建立电网的边        **" << endl;
	cout << "**       C-构造最小生成树      **" << endl;
	cout << "**       D-显示最小生成树      **" << endl;
	cout << "**       E-退出程序            **" << endl;
	cout << "=================================" << endl;
}

void ElectricityNet::BuildNodes() {
	cout << "请输入顶点个数：";
	cin >> NodeNum;
	while (NodeNum < 2) {
		cout << "顶点个数至少为2！" << endl;
		cin >> NodeNum;
	}
	NodeSet.SetSize(NodeNum);			//初始化顶点并查集
	UnUsedEdges.SetSize(NodeNum*(NodeNum - 1) / 2);//初始化边的最小堆
	if (Node != NULL) { delete Node; }
	cout << "请输入顶点名称：";
	Node = new string[NodeNum];			//初始化顶点名称数组
	NeedEdges = new Edge[NodeNum - 1];	//初始化组成最小生成树的边数组
	for (int i = 0; i < NodeNum; i++) { cin >> Node[i]; }
}

void ElectricityNet::AddEdge() {
	if (!Node) {
		cout << "请先创建电网节点！" << endl;
		return;
	}
	string start, end;
	int startNum = 0, endNum = 0;
	float cost;
	Edge e;
	cout << "请输入两个顶点及花费（输入ok ok 0结束）：";
	cin >> start >> end >> cost;
	while (start != "ok") {
		while (Node[startNum] != start) { startNum++; }
		while (Node[endNum] != end) { endNum++; }
		e.Set(startNum, endNum, cost);
		UnUsedEdges.Insert(e);
		startNum = endNum = 0;
		cout << "请输入两个顶点及花费：";
		cin >> start >> end >> cost;
	}
}

void ElectricityNet::BuildTree() {
	if (UnUsedEdges.IsEmpty()) {
		cout << "请先建立电网的边！" << endl;
		return;
	}
	Edge e;
	int ParStart, ParEnd;
	int currentEdges = 0;
	while (!UnUsedEdges.IsEmpty()) {
		e = UnUsedEdges.Pop();
		ParStart = NodeSet.Find(e.Start), ParEnd = NodeSet.Find(e.End);
		if (ParEnd != ParStart) {
			//如果两个顶点不在同一类，则选中这条边，并合并顶点所在的类
			NeedEdges[currentEdges] = e;
			NodeSet.Union(ParEnd, ParStart);
			currentEdges++;
		}
	}
	if (!NodeSet.IsConnected()) {
		//如果没有连通，给出错误提示，并重新初始化各辅助存储变量
		cout << "电网未连通，请重新建立电网的边！" << endl;
		NodeSet.SetSize(NodeNum);
		UnUsedEdges.SetSize(NodeNum*(NodeNum - 1) / 2);
		delete[]NeedEdges;
		NeedEdges = new Edge[NodeNum - 1];
		return;
	}
	else {
		cout << "构造成功" << endl;
	}
}

void ElectricityNet::PrintTree() {
	if (!NodeSet.IsConnected()) {
		cout << "请先构建最小生成树！" << endl;
		return;
	}
	int tmp = NodeNum - 1;
	cout << "最小成树的边为：" << endl;
	for (int i = 0; i < tmp; i++) {
		cout << Node[NeedEdges[i].Start] << "--" << NeedEdges[i].Cost << "--" << Node[NeedEdges[i].End] << ' ' << endl;
	}
}

int main() {
	ElectricityNet net;
	net.PrintMenu();
	char operation = 'a';
	while (operation != 'E') {
		cout << "请选择操作：";
		cin >> operation;
		switch (operation) {
		case 'A':
			net.BuildNodes();
			break;
		case 'B':
			net.AddEdge();
			break;
		case 'C':
			net.BuildTree();
			break;
		case 'D':
			net.PrintTree();
			break;
		default:
			operation = 'E';
			break;
		}
		cout << endl;
	}
	system("pause");
	return 0;
}