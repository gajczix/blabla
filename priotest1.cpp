#include <iostream>
#include <exception>
#include <cassert>
#include <time.h>
#include <stdlib.h>

#include "priorityqueue.hh"

struct randomException: public std::exception{
	virtual const char* what() const noexcept {
		return "Random exception";
	}
};

class qtest {
	public:
	int l = 1;
	int r = rand() % 100;	

	qtest() {}

	bool secret() const {
		if (rand() % 2)
			throw randomException();
		return 1;
	}
	friend bool operator<(const qtest& lhs, const qtest& rhs);
	friend bool operator==(const qtest& lhs, const qtest& rhs);
};


class qtest2 {
	public: 
	int *p;
	int r = rand() & 100;
	qtest2() {
		p = new int;
		*p = rand() % 100;
	}

	~qtest2() {
		if (!p)
			delete p;
	}

	bool secret() const {
		if (rand() % 2)
			throw randomException();
		return 1;
	}
	friend bool operator<(const qtest2& lhs, const qtest2& rhs);
};

bool operator<(const qtest2& lhs, const qtest2& rhs) {
	lhs.secret();
	rhs.secret();
	if (!lhs.p && !rhs.p)
		return true;
	if (!lhs.p)
		return false;
	if (!rhs.p)
		return true;
	return *lhs.p < *rhs.p;
}

bool operator<(const qtest& lhs, const qtest& rhs) {
	lhs.secret();
	rhs.secret();
	return lhs.r < rhs.r;	
}

bool operator==(const qtest& lhs, const qtest& rhs) {
	lhs.secret();
	rhs.secret();
	return lhs.r == rhs.r;
}

int main() {
	srand(time(NULL));
	qtest asd0;
	qtest asd1;
	qtest asd2;
	qtest asd3;
	qtest asd4;
	qtest2 qwe0;
	qtest2 qwe1;
	qtest2 qwe2;
	qtest2 qwe3;
	qtest2 qwe4;

	PriorityQueue<qtest, qtest2> kolejka1;
	PriorityQueue<qtest, qtest2> kolejka2;

	assert(kolejka1.empty());
	kolejka1.insert(asd0, qwe0);
	kolejka1.insert(asd1, qwe1);
	kolejka1.insert(asd2, qwe2);

	kolejka2.insert(asd3, qwe3);
	kolejka2.insert(asd4, qwe4);

	assert(kolejka1.size() == 3);
	assert(kolejka2.size() == 2);

	if (kolejka1 < kolejka2) 
		std::cout << "mniejsza" << std::endl;

	kolejka1.deleteMax();
	kolejka1.deleteMax();
	kolejka1.deleteMax();
	assert(kolejka1.empty());
	kolejka2.deleteMin();
	kolejka2.deleteMin();
	assert(kolejka2.empty());
	return 0;
}
