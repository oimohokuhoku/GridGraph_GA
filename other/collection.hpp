#pragma once

/// @brief consist of fixed-length array and this size
/// @tparam T type of stored element
template<class T>
class Collection {
public:
	Collection();
	Collection(int size);
	~Collection();

	Collection(const Collection<T>& obj);
	Collection<T>& operator= (const Collection<T>& obj);

	inline int size() const;
	T& operator[](int i) const;

private:
	int _size;
	T* _elements;
};

template<class T> Collection<T>::Collection() {
	this->_elements = nullptr;
	this->_size = -1;
}

template<class T> Collection<T>::Collection(int size) {
	this->_elements = new T[size];
	this->_size = size;
}

template<class T> Collection<T>::~Collection() {
	if(this->_elements == nullptr) return;
	delete[] this->_elements;
}

template<class T> Collection<T>::Collection(const Collection<T>& obj) {
	const int SIZE = obj._size;

	this->_size = SIZE;
	this->_elements = new T[SIZE];

	for(int i = 0; i < SIZE; ++i)
		this->_elements[i] = obj._elements[i];
}


template<class T> Collection<T>& Collection<T>::operator=(const Collection<T>& obj) {
	if(this->_elements != nullptr) delete[] this->_elements;

	const int SIZE = obj._size;

	this->_size = SIZE;
	this->_elements = new T[SIZE];

	for(int i = 0; i < SIZE; ++i) {
		this->_elements[i] = obj._elements[i];
	}

	return *this;
}

template<class T> int Collection<T>::size() const{
	return this->_size;
}

template<class T> T& Collection<T>::operator[](int index) const {
	return this->_elements[index];
}
