#include<iostream>
#include<ctime>
#define ListNodePosi(T) ListNode<T>*
typedef int Rank;
using namespace std;

template<typename T>
struct ListNode {
	T data;
	ListNodePosi(T) pred;
	ListNodePosi(T) succ;

	ListNode() = default;
	ListNode(T e, ListNodePosi(T) p = nullptr, ListNodePosi(T) s = nullptr) :
		data(e), pred(p), succ(s) {}

	ListNodePosi(T) insertAsPred(T const& e);
	ListNodePosi(T) insertAsSucc(T const& e);
};

template<typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const& e) {
	ListNodePosi(T) p = new ListNode(e, pred, this);
	pred->succ = p;
	return pred = p;
}

template<typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e) {
	ListNodePosi(T) p = new ListNode(e, this, succ);
	succ->pred = p;
	return succ = p;
}

////////////////////////

template<typename T>
class List {
private:
	Rank _size;
	ListNodePosi(T) header;
	ListNodePosi(T) trailer;
protected:
	void init();
	void copyNode(ListNodePosi(T), int);
	int clear();
	ListNodePosi(T) Get_Np(Rank r);
	void merge(ListNodePosi(T)&, int, List<T>&, ListNodePosi(T), int);    // 有序列表区间归并
	void mergeSort(ListNodePosi(T)&, int);    // 对从p 开始连续的n 个节点归并排序
public:
	List() { init(); }
	List(List<T> const& L);
	List(List<T> const& L, Rank r, int n);
	List(List<T> const& L, int n);
	~List();

	Rank size() { return _size; }
	bool empty() { return _size <= 0; }
	T& operator[] (Rank r) const;
	bool valid(ListNodePosi(T) p) { return p && (p != header) && (p != trailer); }
	int disordered() const;

	void insertAsFirst(T const& e);
	void insertAsLast(T const& e);
	void insertA(ListNodePosi(T) p, T const& e);
	void insertB(ListNodePosi(T) p, T const& e);

	T erase(Rank r);
	void show();

	void unsort();
	void bubblesort();
	void insertSort();
	void mergeSort();

	ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p);
	Rank deduplicate();
	T remove(ListNodePosi(T) p);
	template<typename VST>
	void traverse(VST&);
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const;
};

template<typename T>
void List<T>::insertSort() {
	ListNodePosi(T) p = header->succ;
	for (Rank i = 0; i < _size; i++) {
		insertA(search(p->data, i, p), p->data);
		p = p->succ;
		remove(p->pred);
	}
}

template<typename T>
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const {
	while (0 <= n--)
		if ((p = p->pred)->data <= e)
			break;
	return p;
}

template<typename T>
void List<T>::insertA(ListNodePosi(T) p, T const& e) {
	_size++;
	p->insertAsSucc(e);
}

template<typename T>
void List<T>::insertB(ListNodePosi(T) p, T const& e) {
	_size++;
	p->insertAsPred(e);
}

template<typename T>
template<typename VST>
void List<T>::traverse(VST& visit) {
	for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
		visit(p->data);
}

template<typename T>
Rank List<T>::deduplicate() {
	if (_size < 2)
		return 0;
	int oldsize = _size;
	Rank r = 0;
	ListNodePosi(T) p = header;
	ListNodePosi(T) q;
	while (trailer != (p = p->succ)) {
		q = find(p->data, r, p);
		q ? remove(q) : r++;
	}
	return oldsize - _size;
}

template<typename T>
T List<T>::remove(ListNodePosi(T) p) {
	p->pred->succ = p->succ;
	p->succ->pred = p->pred;
	T temp = p->data;
	delete p;
	_size--;
	return temp;
}

template<typename T>
ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p) {
	while (0 < n--)
		if (e == (p = p->pred)->data)
			return p;
	return nullptr;
}

template<typename T>
void List<T>::bubblesort() {
	ListNodePosi(T) i;
	ListNodePosi(T) j;
	T temp;
	for (i = header->succ; i != trailer; i = i->succ) {
		for (j = i->succ; j != trailer; j = j->succ) {
			if (i->data > j->data) {
				temp = i->data;
				i->data = j->data;
				j->data = temp;
			}
		}
	}
}

template<typename T>
int List<T>::disordered() const {
	ListNodePosi(T) p = header->succ;
	for (Rank i = 0; i < _size-1; i++) {
		if (p->data > p->succ->data)
			return 0;
	}
	return 1;
}

template<typename T>
ListNodePosi(T) List<T>::Get_Np(Rank r) {
	ListNodePosi(T) p = header->succ;
	if (r >= _size)
		return trailer->pred;
	while (r--) {
		p = p->succ;
	}
	return p;
}

template<typename T>
void List<T>::unsort() {
	ListNodePosi(T) p;
	ListNodePosi(T) last;
	T temp;
	last = trailer->pred;
	srand(time(0));
	for (Rank i = _size - 1; i > 0; i--) {
		p = Get_Np(rand() % i);
		//cout << p->data << " " << last->data << endl;
		temp = last->data;
		last->data = p->data;
		p->data = temp;
		last = last->pred;
	}
}

template<typename T>
void List<T>::show() {
	ListNodePosi(T) p = header->succ;
	Rank r = _size;
	std::cout << _size << ":  ";
	while (r--) {
		std::cout << p->data << " ";
		p = p->succ;
	}
	std::cout << std::endl;
}

template<typename T>
int List<T>::clear() {
	int oldsize = _size;
	while (_size--) {
		erase(0);
	}
	return oldsize;
}

template<typename T>
List<T>::~List() {
	clear();
	delete header;
	delete trailer;
	//std::cout << "Delete Ok!\n";
}

template<typename T>
List<T>::List(List<T> const& L, int n) {
	copyNode(L.header->succ, n);
}

template<typename T>
List<T>::List(List<T> const& L, Rank r, int n) {
	ListNodePosi(T) p;
	p = L.header->succ;
	while (r--) {
		p = p->succ;
	}
	copyNode(p, n);
}

template<typename T>
List<T>::List(List<T> const& L) {
	copyNode(L.header->succ, L._size);
}

template<typename T>
void List<T>::copyNode(ListNodePosi(T) p, int n) {
	init();
	while (n--) {
		insertAsLast(p->data);
		p = p->succ;
	}
}

template<typename T>
T List<T>::erase(Rank r) {
	ListNodePosi(T) p = header->succ;
	while (0 < r--) {
		p = p->succ;
	}
	p->pred->succ = p->succ;
	p->succ->pred = p->pred;
	_size--;
	T temp = p->data;
	delete p;
	return temp;
}

template<typename T>
void List<T>::insertAsFirst(T const& e) {
	_size++;
	header->insertAsSucc(e);
}

template<typename T>
void List<T>::insertAsLast(T const& e) {
	_size++;
	trailer->insertAsPred(e);
}

template<typename T>
T& List<T>::operator[] (Rank r) const {
	ListNodePosi(T) p = header->succ;
	while (0 < r--)
		p = p->succ;
	return p->data;
}

template<typename T>
void List<T>::init() {
	header = new ListNode<T>;
	trailer = new ListNode<T>;
	header->pred = trailer;
	header->succ = nullptr;
	trailer->pred = header;
	trailer->succ = nullptr;
	_size = 0;
}


/*   有bug!!!!!!!
template <typename T> void List<T>::mergeSort(ListNodePosi(T)& p, int n) {
	if (n < 2)
		return;
	int m = n >> 1;
	ListNodePosi(T) q = p;
	for (int i = 0; i < m; i++)
		q = q->succ;
	mergeSort(p, m);
	mergeSort(q, n - m);
	merge(p, m, *this, q, n - m);
}

template <typename T> 
void List<T>::merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m) {
	ListNodePosi(T) pp = p->pred;
	while (0 < m) {
		if ((0 < n) && (p->data <= q->data)) {
			if (q == (p = p->succ)) {
				break;
			}
			--n;
		}
		else {
			insertB(p, L.remove((q = q->succ)->pred));
			--m;
		};
	}
	p = pp->succ;
}

template <typename T>
void List<T>::mergeSort() {
	mergeSort(header->succ, _size);
}
*/
