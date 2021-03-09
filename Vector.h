#include<cstdlib>
#include<iostream>
#include<ctime>
using namespace std;
#define DEFAULT_CAPACITY 3
typedef int Rank;

template<typename T>
class Vector {
protected:
	Rank _size;
	Rank _capacity;
	T* _elem;
	void expand();
	void shrink();
	Rank max(Rank lo, Rank hi);
	void swap(T* lo, T* hi);
	Rank binSearch(T const& e, Rank* lo, Rank* hi);
public:
	Vector(int c = DEFAULT_CAPACITY, Rank s = 0, T v = 0);
	Vector(T* lo, T* hi);
	Vector(Vector<T>& temp);
	~Vector();
	void push_back(T const& e);
	void pop_back();
	Rank size() const;
	bool empty() const;
	Rank size_c() const;
	T back();
	T front();
	T* end();
	T* begin();
	T& operator[](Rank r) const;
	void insert(T* r, T const& e);
	void erase(T* s1, T* e1);
	Rank erase(T* s1);
	void sort();
	void sort(T* lo, T* hi);
	void unsort();
	void unsort(T* lo, T* hi);
	Rank disordered() const;
	Rank find(T const& e);
	Rank find(T const& e, T* lo, T* hi);
	Rank deduplicate();
	Rank uniquify();
	void show();
	Rank search(T const& e, Rank* lo, Rank* hi);
	Rank search(T const& e);
	void bubbleSort(Rank* lo, Rank* hi);
	bool bubble(Rank* lo, Rank* hi);
	void mergeSort(Rank* lo, Rank* hi);
	void merge(Rank* lo, Rank* mi, Rank* hi);
	template<typename VST>
	void traverse(VST&);
};

template<typename T>
void Vector<T>::merge(Rank* lo, Rank* mi, Rank* hi) {
	T* A = lo;
	Rank lb = mi - lo;
	T* B = new T[lb];
	for (Rank i = 0; i < lb; i++)
		B[i] = A[i];
	Rank lc = hi - mi;
	T* C = mi;
	for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);) {
		if ((j < lb) && (!(k < lc) || (B[j] <= C[k])))
			A[i++] = B[j++];
		if ((k < lc) && (!(j < lb) || (C[k] < B[j])))
			A[i++] = C[k++];
	}
	delete[] B;
}

template<typename T>
void Vector<T>::mergeSort(Rank* lo, Rank* hi) {
	if (hi - lo < 2)
		return;
	T* mi = (hi - lo) / 2 + lo;
	mergeSort(lo, mi);
	mergeSort(mi, hi);
	merge(lo, mi, hi);
}

template<typename T>
bool Vector<T>::bubble(Rank* lo, Rank* hi) {
	bool sorted = true;
	T low = lo - _elem;
	T hig = hi - _elem;
	while (++low < hig) {
		if (_elem[low - 1] > _elem[low]) {
			sorted = false;
			swap(_elem + low - 1, _elem + low);
		}
	}
	return sorted;
}

template<typename T>
void Vector<T>::bubbleSort(Rank* lo, Rank* hi) {
	while (!bubble(lo, hi--));
}

template<typename T>
Rank Vector<T>::search(T const& e) {
	return binSearch(e, _elem, _elem + _size);
}

template<typename T>
Rank Vector<T>::search(T const& e, Rank* lo, Rank* hi) {
	return binSearch(e, lo, hi);
}

template<typename T>
Rank Vector<T>::binSearch(T const& e, Rank* lo, Rank* hi) {
	Rank low = lo - _elem;
	Rank hig = hi - _elem;
	Rank mi;
	while (low < hig) {
		mi = (low + hig) >> 1;
		if (e < _elem[mi])
			hig = mi;
		else if (e > _elem[mi])
			low = mi + 1;
		else
			return mi;
	}
	return -1;
}

template<typename T>
Rank Vector<T>::uniquify() {
	Rank i, j;
	i = j = 0;
	while (++j < _size)
		if (_elem[i] != _elem[j])
			_elem[++i] = _elem[j];
	_size = ++i;
	shrink();
	return j - i;
}

template<typename T>
template<typename VST>
void Vector<T>::traverse(VST& visit) {
	for (Rank i = 0; i < _size; i++)
		_elem[i] = visit(_elem[i]);
}

template<typename T>
Rank Vector<T>::deduplicate() {
	int oldsize = _size;
	Rank i = 1;
	while (i < _size) {
		(find(_elem[i], _elem, _elem + i) < 0) ? i++ : erase(_elem + i);
	}
	return oldsize - _size;
}

template<typename T>
Rank Vector<T>::disordered() const {
	Rank n = 0;
	for (Rank i = 1; i < _size; i++)
		n += (_elem[i - 1] > _elem[i]);
	return n;
}

template<typename T>
Rank Vector<T>::find(T const& e, T* lo, T* hi) {
	Rank low, hig;
	low = lo - _elem;
	hig = hi - _elem;
	while ((low < hig--) && (e != _elem[hig]));
	return hig;
}

template<typename T>
void Vector<T>::sort(T* lo, T* hi) {
	T temp;
	Rank low, hig;
	low = lo - _elem;
	hig = hi - _elem;
	for (Rank i = low; i < hig; i++) {
		for (Rank j = i + 1; j < hig; j++) {
			if (_elem[i] > _elem[j]) {
				temp = _elem[i];
				_elem[i] = _elem[j];
				_elem[j] = temp;
			}
		}
	}
}

template<typename T>
void Vector<T>::sort() {
	sort(_elem, _elem + _size);
}

template<typename T>
void Vector<T>::show() {
	for (int i = 0; i < _size; i++)
		cout << _elem[i] << " ";
	cout << endl << endl;
}

template<typename T>
void Vector<T>::unsort() {
	unsort(_elem, _elem + _size);
}

template<typename T>
void Vector<T>::unsort(T* lo, T* hi) {
	Rank low, hig, temp;
	low = lo - _elem;
	hig = hi - _elem;
	if (hig < low) {
		temp = low;
		low = hig;
		hig = temp;
	}
	srand(time(0));
	for (Rank i = hig; i > low; i--)
		swap((_elem + i - 1), (_elem + low + (rand() % (i - low))));
}

template<typename T>
void Vector<T>::swap(T* lo, T* hi) {
	T temp;
	temp = *lo;
	*lo = *hi;
	*hi = temp;
}

template<typename T>
Vector<T>::Vector(Vector<T>& temp) {
	_capacity = max(temp._capacity, DEFAULT_CAPACITY);
	_elem = new T[_capacity];
	_size = 0;
	for (; _size < temp._size; _size++)
		_elem[_size] = temp[_size];
}

template<typename T>
Vector<T>::Vector(T* lo, T* hi) {
	_capacity = max(hi - lo, DEFAULT_CAPACITY);
	_elem = new T[_capacity];
	_size = 0;
	while (lo + _size < hi) {
		_elem[_size] = *(lo + _size);
		_size++;
	}
}

template<typename T>
Rank Vector<T>::erase(T* s1) {
	erase(s1, s1 + 1);
	return 1;
}

template<typename T>
void Vector<T>::erase(T* s1, T* e1) {
	Rank lo = s1 - _elem;
	Rank hi = e1 - _elem;
	if (lo == hi)
		return;
	while (hi < _size)
		_elem[lo++] = _elem[hi++];
	_size = lo;
	shrink();
}

template<typename T>
void Vector<T>::insert(T* r, T const& e) {
	Rank ii = r - _elem;
	expand();
	for (Rank i = _size - 1; i >= ii; i--) {
		_elem[i + 1] = _elem[i];
	}
	_elem[ii] = e;
	_size++;
}

template<typename T>
T* Vector<T>::begin() {
	return _elem;
}

template<typename T>
T* Vector<T>::end() {
	return _elem + _size;
}

template<typename T>
T Vector<T>::front() {
	return _elem[0];
}

template<typename T>
T Vector<T>::back() {
	return _elem[_size - 1];
}

template<typename T>
bool Vector<T>::empty() const {
	return !_size;
}

template<typename T>
void Vector<T>::pop_back() {
	_size--;
	shrink();
}

template<typename T>
T& Vector<T>::operator[](Rank r) const {
	return _elem[r];
}

template<typename T>
Rank Vector<T>::size() const {
	return _size;
}

template<typename T>
Rank Vector<T>::size_c() const {
	return _capacity;
}

template<typename T>
Vector<T>::~Vector() {
	delete[] _elem;
}

template<typename T>
Vector<T>::Vector(int c, Rank s, T v) {
	_elem = new T[_capacity = c];
	for (_size = 0; _size < s; _size++)
		_elem[_size] = v;
}

template<typename T>
void Vector<T>::push_back(T const& e) {
	expand();
	_elem[_size++] = e;
}

template<typename T>
Rank Vector<T>::max(Rank lo, Rank hi) {
	return (lo > hi) ? lo : hi;
}

template<typename T>
void Vector<T>::expand() {
	if (_size < _capacity)
		return;
	_capacity = max(_capacity, DEFAULT_CAPACITY);
	T* oldelem = _elem;
	_elem = new T[_capacity <<= 1];
	for (Rank i = 0; i < _size; i++)
		_elem[i] = oldelem[i];
	delete[] oldelem;
}

template<typename T>
void Vector<T>::shrink() {
	if (_size >= (_capacity >> 1))
		return;
	_capacity >>= 1;
	_capacity = max(DEFAULT_CAPACITY, _capacity);
	T* oldelem = _elem;
	_elem = new T[_capacity];
	for (Rank i = 0; i < _size; i++)
		_elem[i] = oldelem[i];
	delete[] oldelem;
}
