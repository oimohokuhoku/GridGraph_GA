#pragma once

/// @brief consist of fixed-length array and this size
/// @tparam T type of stored element
template<class T>
class Collection {
public:
	Collection();
	Collection(int size);
	Collection(const Collection<T>& obj);
	~Collection();

	Collection<T>& operator= (const Collection<T>& obj);
	T& operator[](int i) const;
	Collection<T> concat(const Collection<T>& collection) const;
	inline int size() const;

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

template<class T> Collection<T> Collection<T>::concat(const Collection<T>& collection) const {
	int size = this->_size + collection._size;
	Collection<T> result(size);

	for(int i = 0; i < this->size(); ++i) {
		result._elements[i] = this->_elements[i];
	}

	for(int i = 0; i < collection.size(); ++i) {
		result._elements[this->size() + i] = collection._elements[i];
	}

	return result;
}

template<class T> int Collection<T>::size() const{
	return this->_size;
}

template<class T> T& Collection<T>::operator[](int index) const {
	return this->_elements[index];
}
