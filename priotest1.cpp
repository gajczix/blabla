#include <iostream>
#include <exception>
#include <cassert>
#include <time.h>
#include <stdlib.h>

struct randomException: public std::exception{
	virtual const char* what() const noexcept {
		return "Random exception";
	}
};

class qtest {
	public:
	int l = 1;
	int r = rand() % 100;	
	qtest() {
		secret();
	}
	bool secret() const {
		try {
			if (rand() % 2)
				throw randomException();
		}
		catch (...) {
			std::cout << "woops" << std::endl;
			return 0;
		}
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
		secret();
	}

	~qtest2() {
		if (!p)
			delete p;
	}

	bool secret() const {
		try {
			if (rand() % 2)
				throw randomException();
		}
		catch (...) {
			std::cout << "woopi" << std::endl;
			if (!p)
				delete p;
			return 0;
		}
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
	qtest elo;
	qtest mordo;
	qtest2 dupa;
	qtest2 zbita;
	if (dupa < zbita)
		std::cout << "xd" << std::endl;
	if (elo < mordo)
		std::cout << "wops x2" << std::endl;
	return 0;
}
