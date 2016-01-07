#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include <map>
#include <cstdlib>
#include <utility>
#include <exception>
#include <set>

template <typename K>
struct classcomp {
	bool operator() (const K * lhs, const K * rhs) const{
		return *lhs < *rhs;
	}
};

struct PriorityQueueEmptyException : public std::exception{
	virtual const char* what() const noexcept {
		return "PriorityQueue is empty";
	}
};

struct PriorityQueueNotFoundException : public std::exception{
	virtual const char* what() const noexcept {
		return "This element does not exist";
	}
};

template <typename K, typename V> 
class  PriorityQueue {
	
	//mapa do trzymania wartości i kluczy
	std::multimap <V  const *, K  const *, classcomp<V> > values;
	//mapa ze wskaźnikiem na klucz (ułatwia wyszukiwanie) i iteratorem na pierwszą mapę
	std::multimap <K const *, typename std::multimap <V const *, K const *>::const_iterator , classcomp<K> > iterators;
	//multiset par <klucz , wartość > do porównań między kolejkami
	std::multiset < std::pair<K, V>  > ordered;
	
	public:
	// Konstruktor bezparametrowy tworzący pustą kolejkę [O(1)]
	PriorityQueue(){}

	// Konstruktor kopiujący [O(queue.size())], nie domyślny, aby uniknąć kopiowania wskaźników
	PriorityQueue(const PriorityQueue<K, V>& queue){
		PriorityQueue<K, V> temp;
		for(auto it = queue.ordered.begin(); it != queue.ordered.end(); it++){
			temp.insert(it->first, it->second);
		}
		this->swap(temp);
	} //SILNA

	// Konstruktor przenoszący [O(1)]
	PriorityQueue(PriorityQueue<K, V>&& queue) = default; //SILNA

	~PriorityQueue() = default; //NOTHROW
	
	// Operator przypisania [O(queue.size()) dla użycia P = Q, a O(1) dla użycia
	// P = move(Q)]
	PriorityQueue<K, V>& operator=(PriorityQueue<K, V>& queue){
		if(&*this == &queue)
			return *this;
		PriorityQueue<K, V> temp(queue);
		temp.swap(*this);
		return *this;
	} //SILNA

	PriorityQueue<K, V>& operator=(PriorityQueue<K, V>&& queue) = default; //SILNA

	// Metoda zwracająca true wtedy i tylko wtedy, gdy kolejka jest pusta [O(1)]
	bool empty() const noexcept{
		return values.empty();
	}//NOTHROW

	// Metoda zwracająca liczbę par (klucz, wartość) przechowywanych w kolejce
	// [O(1)]
	std::size_t size() const noexcept{
		return values.size();
	}//NOTHROW

	// Metoda wstawiająca do kolejki parę o kluczu key i wartości value
	// [O(log size())] (dopuszczamy możliwość występowania w kolejce wielu
	// par o tym samym kluczu)
	void insert(const K& key, const V& value){
		auto const it = ordered.emplace(key, value);
		auto it2 = values.begin(); //hack dla nadania it2 typu
		auto adr_key = &it->first;
		try{
			it2 = values.emplace(&it->second, &it->first);
		}catch(...){
			ordered.erase(it);
			throw;
		}
		try{
			iterators.emplace(adr_key, it2);
		}catch(...){
			values.erase(it2);
			ordered.erase(it);
			throw;
		}
	} //SILNA
	

	// Metody zwracające odpowiednio najmniejszą i największą wartość przechowywaną
	// w kolejce [O(1)]; w przypadku wywołania którejś z tych metod na pustej
	// strukturze powinien zostać zgłoszony wyjątek PriorityQueueEmptyException
	const V& minValue() const{
		if(values.empty())
			throw PriorityQueueEmptyException();
		return *values.begin()->first;
	} 
	const V& maxValue() const{
		if(values.empty())
			throw PriorityQueueEmptyException();
		return *values.rbegin()->first;
	}

	// Metody zwracające klucz o przypisanej odpowiednio najmniejszej lub
	// największej wartości [O(1)]; w przypadku wywołania którejś z tych metod
	// na pustej strukturze powinien zostać zgłoszony wyjątek
	// PriorityQueueEmptyException
	const K& minKey() const{
		if(values.empty())
			throw PriorityQueueEmptyException();
		return *values.begin()->second;
	}
	const K& maxKey() const{
		if(values.empty())
			throw PriorityQueueEmptyException();
		return *values.rbegin()->second;
	}
	private:
	void deleteElem(typename std::multimap <V  const *, K  const *, classcomp<V> >::iterator it){
		auto range = iterators.equal_range(it->second);
		auto it2 = ordered.find(std::make_pair(*it->second, *it->first));
		for(auto i = range.first; i != range.second; i++){
			if(i->second == it){
				iterators.erase(i);
				break;
			}
		}
		values.erase(it);
		ordered.erase(it2);
	} //SILNA
	public:
	// Metody usuwające z kolejki jedną parę o odpowiednio najmniejszej lub
	// największej wartości [O(n)]
	void deleteMin(){
		if(values.empty())
			return;
		auto it = values.begin();
		deleteElem(it);
	} //SILNA
	void deleteMax(){
		if(values.empty())
			return;
		auto it = std::next(values.rbegin()).base();
		deleteElem(it);
	}
 //SILNA
	
	// Metoda zmieniająca dotychczasową wartość przypisaną kluczowi key na nową
	// wartość value [O(log size())]; w przypadku gdy w kolejce nie ma pary
	// o kluczu key, powinien zostać zgłoszony wyjątek
	// PriorityQueueNotFoundException(); w przypadku kiedy w kolejce jest kilka par
	// o kluczu key, zmienia wartość w dowolnie wybranej parze o podanym kluczu
	void changeValue(const K& key, const V& value){
		auto const it = iterators.find(&key);
		if(it == iterators.end()){
			throw PriorityQueueNotFoundException();
		}
		auto const it2 = ordered.find(std::make_pair(key, *(it->second->first)));
		assert(it2 != ordered.end());
		
		this->insert(key, value);
		
		ordered.erase(it2);
		values.erase(it->second);
		iterators.erase(it);
	} //SILNA
	
	// Metoda scalająca zawartość kolejki z podaną kolejką queue; ta operacja usuwa
	// wszystkie elementy z kolejki queue i wstawia je do kolejki *this
	// [O(size() + queue.size() * log (queue.size() + size()))]
	void merge(PriorityQueue<K, V>& queue){
		if(&*this == &queue )
			return;
		PriorityQueue temp(*this);
		try{
			for(auto elem : queue.ordered){
				this->insert(elem.first, elem.second);	
			}
			queue.ordered.clear();
			queue.iterators.clear();
			queue.values.clear();
		}catch(...){
			this->swap(temp);
			throw;
		}
	} //SILNA

	// Metoda zamieniającą zawartość kolejki z podaną kolejką queue (tak jak
	// większość kontenerów w bibliotece standardowej) [O(1)]
	void swap(PriorityQueue<K, V>& queue) noexcept{
		this->values.swap(queue.values);
		this->iterators.swap(queue.iterators);
		this->ordered.swap(queue.ordered);
	} //NOTHROW
	template <typename S, typename T>
	friend bool operator==(const PriorityQueue<S,T> &lhs, const PriorityQueue<S,T> &rhs);
	template <typename S, typename T>
	friend bool operator<(const PriorityQueue<S,T> &lhs, const PriorityQueue<S,T> &rhs);
	template <typename S, typename T>
	friend bool operator>(const PriorityQueue<S,T> &lhs, const PriorityQueue<S,T> &rhs);
	template <typename S, typename T>
	friend bool operator!=(const PriorityQueue<S,T> &lhs, const PriorityQueue<S,T> &rhs);
	template <typename S, typename T>
	friend bool operator<=(const PriorityQueue<S,T> &lhs, const PriorityQueue<S,T> &rhs);
	template <typename S, typename T>
	friend bool operator>=(const PriorityQueue<S,T> &lhs, const PriorityQueue<S,T> &rhs);
	template<typename S, typename T>
	friend void swap(PriorityQueue<S,T>& queue1, PriorityQueue<S,T>& queue2 );
};
template <typename K, typename V>
bool operator<(const PriorityQueue<K,V> &lhs, const PriorityQueue<K,V> &rhs){
return lhs.ordered < rhs.ordered;
} //SILNA
template<typename S, typename T>
bool operator==(const PriorityQueue<S,T> &lhs, const PriorityQueue<S,T> &rhs){
	return !(rhs < lhs) && !(lhs < rhs);
}

template <typename S, typename T>
bool operator>(const PriorityQueue<S,T> &lhs, const PriorityQueue<S,T> &rhs){
	return rhs < lhs;
}
template <typename S, typename T>
bool operator!=(const PriorityQueue<S,T> &lhs, const PriorityQueue<S,T> &rhs){
	return !(rhs == lhs);
}
template <typename S, typename T>
bool operator<=(const PriorityQueue<S,T> &lhs, const PriorityQueue<S,T> &rhs){
	return !(lhs > rhs);
}
template <typename S, typename T>
bool operator>=(const PriorityQueue<S,T> &lhs, const PriorityQueue<S,T> &rhs){
	return !(lhs < rhs);
}

template <typename K, typename V>
void swap(PriorityQueue<K,V> & lhs, PriorityQueue<K,V> & rhs){
	lhs.swap(rhs);
}

#endif
