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

	qtest() {}
	qtest(int i) { l = i; }

	bool secret() const {
		if (l % 2 == 0)
			throw randomException();
		return 1;
	}
	friend bool operator<(const qtest& lhs, const qtest& rhs);
	friend bool operator==(const qtest& lhs, const qtest& rhs);
};


class qtest2 {
	public: 
	int *p;

	qtest2(int i) {
		p = new int;
		*p = i;
	}

	~qtest2() {
		delete p;
	}

	bool secret() const {
		if (*p % 2 == 0)
			throw randomException();
		return 1;
	}
	friend bool operator<(const qtest2& lhs, const qtest2& rhs);
};

bool operator<(const qtest2& lhs, const qtest2& rhs) {
	lhs.secret();
	return *lhs.p < *rhs.p;
}

bool operator<(const qtest& lhs, const qtest& rhs) {
	lhs.secret();
	return lhs.l < rhs.l;	
}

bool operator==(const qtest& lhs, const qtest& rhs) {
	rhs.secret();
	return lhs.l == rhs.l;
}

int main() {
	srand(time(NULL));
	qtest asd0(0);
	qtest asd1(1);
	qtest asd2(2);
	qtest asd3(3);
	qtest asd4(4);
	
	qtest2 qwe0(0);
	qtest2 qwe1(1);
	qtest2 qwe2(2);
	qtest2 qwe3(3);
	qtest2 qwe4(4);

	PriorityQueue<qtest, qtest2> kolejka1;
	PriorityQueue<qtest, qtest2> kolejka2;

	assert(kolejka1.empty());
	kolejka1.insert(asd0, qwe0);
	kolejka1.insert(asd1, qwe1);
	kolejka1.insert(asd3, qwe1);
	try {
		kolejka1.insert(asd2, qwe2);
	}
	catch(...){
		for (auto it: kolejka1.values)
			std::cout << *it.first.p << " " << it.second.l << std::endl;
	}	
	return 0;
}
