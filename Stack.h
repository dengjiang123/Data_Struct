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
