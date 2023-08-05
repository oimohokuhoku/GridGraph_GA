#pragma once

class IntQueue {
public:
	IntQueue(int maxSize);
	~IntQueue();
	void enqueue(int value);
	int dequeue();
	int count();
	bool isEmpty();
	bool contain(int element);
	void clear();

private:
	int _head;
	int _tail;
	int* _container;
	int _maxSize;

	IntQueue();
};