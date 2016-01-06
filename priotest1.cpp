#include <iostream>
#include <exception>
#include <cassert>
#include <time.h>
#include <stdlib.h>
#include <set>

#include "test1.hh"

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
	qtest(const qtest& k) {
		l = k.l;
	}
	qtest(qtest&& k) = default;

	bool secret() const {
		if (l % 2 == 0)
			throw randomException();
		return 1;
	}
	friend bool operator<(const qtest& lhs, const qtest& rhs);
	friend bool operator>(const qtest& lhs, const qtest& rhs);
	friend bool operator==(const qtest& lhs, const qtest& rhs);
};


class qtest2 {
	public: 
	int *p;

	qtest2(int i) {
		p = new int;
		*p = i;
	}
	qtest2(const qtest2& k) {
		p = new int;
		*p = *k.p;
	}

	qtest2(qtest2&& k) = default;

	~qtest2() {
		delete p;
	}

	bool secret() const {
		if (*p % 2 == 0)
			throw randomException();
		return 1;
	}
	friend bool operator<(const qtest2& lhs, const qtest2& rhs);
	friend bool operator>(const qtest2& lhs, const qtest2& rhs);
	friend bool operator==(const qtest2& lhs, const qtest2& rhs);
};

bool operator<(const qtest2& lhs, const qtest2& rhs) {
	lhs.secret();
	return *lhs.p < *rhs.p;
}

bool operator>(const qtest2& lhs, const qtest2& rhs) {
	lhs.secret();
	return *lhs.p > *rhs.p;
}

bool operator==(const qtest2& lhs, const qtest2& rhs) {
	lhs.secret();
	return *lhs.p == *rhs.p;
}

bool operator<(const qtest& lhs, const qtest& rhs) {
	lhs.secret();
	return lhs.l < rhs.l;	
}

bool operator>(const qtest& lhs, const qtest& rhs) {
	lhs.secret();
	return lhs.l > rhs.l;	
}

bool operator==(const qtest& lhs, const qtest& rhs) {
	lhs.secret();
	return lhs.l == rhs.l;
}

int main() {
	srand(time(NULL));
	qtest asd0(0);
	qtest asd1(1);
	qtest asd2(2);
	qtest asd3(3);
	qtest asd4(4);
	qtest asd5(5);
	
	qtest2 qwe0(0);
	qtest2 qwe1(1);
	qtest2 qwe2(2);
	qtest2 qwe3(3);
	qtest2 qwe4(4);
	qtest2 qwe5(5);

	PriorityQueue<qtest, qtest2> kolejka1;
	PriorityQueue<qtest, qtest2> kolejka2;
	PriorityQueue<qtest, qtest2> kolejka3;
	PriorityQueue<qtest, qtest2> kolejka4;

	kolejka1.insert(asd0, qwe0);
	kolejka1.insert(asd1, qwe1);
	kolejka1.insert(asd3, qwe3);
	kolejka2.insert(asd5, qwe5);
	kolejka2.insert(asd3, qwe3);

	try {
		kolejka1.insert(asd2, qwe2);
	}
	catch(...) {
		std::cout << "Error przy insercie" << std::endl;
		for (auto it: kolejka1.values)
			std::cout << *it.first.p << " " << it.second.l << std::endl;
		std::cout << "Rozmiar iterators: " <<  kolejka1.iterators.size() << std::endl;
		std::cout << "Ordered: " <<  kolejka1.ordered.size() << std::endl;
	}

	try {
		kolejka2.merge(kolejka1);
	}
	catch(...) {
		std::cout << "Error przy merge" << std::endl;
		for (auto it: kolejka1.values)
			std::cout << *it.first.p << " " << it.second.l << std::endl;
		std::cout << "Rozmiar iterators: " <<  kolejka1.iterators.size() << std::endl;
		std::cout << "Ordered: " <<  kolejka1.ordered.size() << std::endl;
	}

	try {
		kolejka1.changeValue(asd0, qwe5);
	}
	catch(...) {
		std::cout << "Error przy changeValue" << std::endl;
		for (auto it: kolejka1.values)
			std::cout << *it.first.p << " " << it.second.l << std::endl;
		std::cout << "Rozmiar iterators: " <<  kolejka1.iterators.size() << std::endl;
		std::cout << "Ordered: " <<  kolejka1.ordered.size() << std::endl;
	}

	kolejka3.insert(asd1, qwe1);
	kolejka3.insert(asd3, qwe3);
	kolejka3.insert(asd5, qwe5);
	kolejka4.insert(asd3, qwe3);
	kolejka3.merge(kolejka4);
	assert(kolejka4.empty());
	assert(kolejka4.values.size() == 0);
	assert(kolejka4.iterators.size() == 0);
	assert(kolejka4.ordered.size() == 0);

	kolejka1.values.clear();
	kolejka1.iterators.clear();
	kolejka1.ordered.clear();
	kolejka2.values.clear();
	kolejka2.iterators.clear();
	kolejka2.ordered.clear();

	kolejka1.insert(asd0, qwe0);
	kolejka1.insert(asd3, qwe3);
	kolejka1.insert(asd5, qwe5);

	kolejka2.insert(asd0, qwe0);
	kolejka2.insert(asd3, qwe3);
	kolejka2.insert(asd5, qwe3);
	try {
		if (kolejka1 == kolejka2);
	}
	catch(...) {
		std::cout << "Error przy rownosci ==" << std::endl;
		for (auto it: kolejka1.values)
			std::cout << *it.first.p << " " << it.second.l << std::endl;
		std::cout << "Rozmiar iterators: " <<  kolejka1.iterators.size() << std::endl;
		std::cout << "Ordered: " <<  kolejka1.ordered.size() << std::endl;
	}
	
	try {
		if (kolejka1 <= kolejka2);
	}
	catch(...) {
		std::cout << "Error przy mniejsze <" << std::endl;
		for (auto it: kolejka1.values)
			std::cout << *it.first.p << " " << it.second.l << std::endl;
		std::cout << "Rozmiar iterators: " <<  kolejka1.iterators.size() << std::endl;
		std::cout << "Ordered: " <<  kolejka1.ordered.size() << std::endl;
	}
	try {
		kolejka1.deleteMin();
		kolejka1.deleteMax();
		assert(kolejka1 != kolejka2);
	}
	catch(...) {
		std::cout << "Error przy usuwaniu" << std::endl;
		for (auto it: kolejka1.values)
			std::cout << *it.first.p << " " << it.second.l << std::endl;
		std::cout << "Rozmiar iterators: " <<  kolejka1.iterators.size() << std::endl;
		std::cout << "Ordered: " <<  kolejka1.ordered.size() << std::endl;
	}

	// ???
	PriorityQueue<int *, int> kolej1;
	int *k = new int;
	int *s = new int;
	*k = 8;
	*s = 5;
	kolej1.insert(k, *k);
	kolej1.insert(s, *s);
	kolej1.changeValue(k, 128);
	for (auto it: kolej1.values)
		std::cout << it.first << " " << it.second << std::endl;
	// ???
	return 0;
}
