#include "int_queue.hpp"
#include<iostream>

IntQueue::IntQueue(int maxSize) {
	_container = new int[maxSize];
	_maxSize = maxSize;
	_head = 0;
	_tail = 0;
}

IntQueue::~IntQueue() {
	delete[] _container;
}

void IntQueue::enqueue(int value) {
	_container[_tail] = value;
	_tail = (_tail + 1) % _maxSize;
}

int IntQueue::dequeue() {
	if(empty()) {
		std::cerr << "Exception IntQueue::dequeue()" << std::endl;
		exit(0);
	}

	int temp = _container[_head];
	_head = (_head + 1) % _maxSize;
	return temp;
}

int IntQueue::count() {
	if (_head <= _tail) return _tail - _head;
	else return _maxSize - (_head - _tail);
}

bool IntQueue::empty() {
	return (_head == _tail);
}

bool IntQueue::contain(int element) {
	for (int i = _head; i < _tail; ++i) {
		if (_container[i] == element) return true;
	}
	return false;
}

void IntQueue::clear() {
	_head = _tail;
}