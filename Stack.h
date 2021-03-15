#include"Vector.h"

template<typename T>
class Stack :public Vector<T> {
public:
	void push(T const& e) {
		this->insert(this->end(), e);
	}
	void pop() {
		this->erase(this->end() - 1);
	}
	T top() {
		return this->back();
	}
};


void convert(Stack<char>& S, __int64 n, int base) {
	static char digit[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	if (0 < n) {
		S.push(digit[n % base]);
		convert(S, n / base, base);
	}
}

void convertRecursive(Stack<char>& S, __int64 n, int base) {
	static char digit[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	while (n) {
		S.push(digit[n % base]);
		n /= base;
	}
}
