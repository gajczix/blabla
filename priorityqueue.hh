#include <map>
#include <cstdlib>
#include <utility>
#include <cassert>
#include <exception>
using namespace std;

template <typename K>
struct classcomp {
  bool operator() (const K * lhs, const K * rhs) const
  {return *lhs < *rhs;}
};

struct PriorityQueueEmptyException : public exception{
	virtual const char* what() const noexcept {
		return "PriorityQueue is empty";
	}
};

struct PriorityQueueNotFoundException : public exception{
	virtual const char* what() const noexcept {
		return "This element does not exist";
	}
};
template <typename K, typename V> 
class PriorityQueue {
	
	std::multimap <V, K> values;
	std::multimap <K const *, typename std::multimap <V, K>::const_iterator , classcomp<K> > iterators;
	
	public:
	// Konstruktor bezparametrowy tworzący pustą kolejkę [O(1)]
	PriorityQueue(){}

	// Konstruktor kopiujący [O(queue.size())]
	PriorityQueue(const PriorityQueue<K, V>& queue){
		for(auto it = queue.values.begin(); it != queue.values.end(); it++){
			this->insert(it->second, it->first);
		}
	}

	// Konstruktor przenoszący [O(1)]
	PriorityQueue(PriorityQueue<K, V>&& queue) = default;

	// Operator przypisania [O(queue.size()) dla użycia P = Q, a O(1) dla użycia
	// P = move(Q)]
	PriorityQueue<K, V>& operator=(PriorityQueue<K, V>& queue){
		PriorityQueue<K, V> temp(queue);
		temp.swap(*this);
		return *this;
	}

	PriorityQueue<K, V>& operator=(PriorityQueue<K, V>&& queue) = default;

	// Metoda zwracająca true wtedy i tylko wtedy, gdy kolejka jest pusta [O(1)]
	bool empty() const{
		return values.empty();
	}//NOTHROW

	// Metoda zwracająca liczbę par (klucz, wartość) przechowywanych w kolejce
	// [O(1)]
	std::size_t size() const{ //TODO: poprawic na size_type
		return values.size();
	}//NOTHROW

	// Metoda wstawiająca do kolejki parę o kluczu key i wartości value
	// [O(log size())] (dopuszczamy możliwość występowania w kolejce wielu
	// par o tym samym kluczu)
	void insert(const K& key, const V& value){
		auto const it = values.emplace(value, key);
		auto elem = &it->second;
		try{
			iterators.emplace(elem, it);
		}catch(...){
			values.erase(it);
			throw;
		}
	}
	

	// Metody zwracające odpowiednio najmniejszą i największą wartość przechowywaną
	// w kolejce [O(1)]; w przypadku wywołania którejś z tych metod na pustej
	// strukturze powinien zostać zgłoszony wyjątek PriorityQueueEmptyException
	const V& minValue() const{
		if(values.empty())
			throw PriorityQueueEmptyException();
		return values.begin()->first;
	}	
	const V& maxValue() const{
		if(values.empty())
			throw PriorityQueueEmptyException();
		return values.rbegin()->first;
	}

	// Metody zwracające klucz o przypisanej odpowiednio najmniejszej lub
	// największej wartości [O(1)]; w przypadku wywołania którejś z tych metod
	// na pustej strukturze powinien zostać zgłoszony wyjątek
	// PriorityQueueEmptyException
	const K& minKey() const{
		if(values.empty())
			throw PriorityQueueEmptyException();
		return values.begin()->second;
	}
	const K& maxKey() const{
		if(values.empty())
			throw PriorityQueueEmptyException();
		return values.rbegin()->second;
	}
	private:
	bool deleteElem(const K& key){
		auto const it = iterators.find(&key);
		if(it == iterators.end()){
			return false;
		}
		values.erase(it->second);
		iterators.erase(it);
		return true;
	}
	public:
	// Metody usuwające z kolejki jedną parę o odpowiednio najmniejszej lub
	// największej wartości [O(log size())]
	void deleteMin(){
		if(values.empty())
			return;
		deleteElem(values.begin()->second);
	}
	void deleteMax(){
		if(values.empty())
			return;
		deleteElem(values.rbegin()->second);
	}
	
	// Metoda zmieniająca dotychczasową wartość przypisaną kluczowi key na nową
	// wartość value [O(log size())]; w przypadku gdy w kolejce nie ma pary
	// o kluczu key, powinien zostać zgłoszony wyjątek
	// PriorityQueueNotFoundException(); w przypadku kiedy w kolejce jest kilka par
	// o kluczu key, zmienia wartość w dowolnie wybranej parze o podanym kluczu
	void changeValue(const K& key, const V& value){
		if(!deleteElem(key))
			throw PriorityQueueNotFoundException();
		this->insert(key, value);
	}
	
	// Metoda scalająca zawartość kolejki z podaną kolejką queue; ta operacja usuwa
	// wszystkie elementy z kolejki queue i wstawia je do kolejki *this
	// [O(queue.size() * log (queue.size() + size()))]
	void merge(PriorityQueue<K, V>& queue){
		for(auto elem : queue.values){
			this->insert(elem.second, elem.first);
			queue.deleteElem(elem.second);
		}
	}

	// Metoda zamieniającą zawartość kolejki z podaną kolejką queue (tak jak
	// większość kontenerów w bibliotece standardowej) [O(1)]
	void swap(PriorityQueue<K, V>& queue){
		this->values.swap(queue.values);
		this->iterators.swap(queue.iterators);
	}
	template <typename S, typename T>
	friend bool operator==(const PriorityQueue<S,T> &lhs, const PriorityQueue<S,T> &rhs);
	template <typename S, typename T>
	friend bool operator<(const PriorityQueue<S,T> &lhs, const PriorityQueue<S,T> &rhs);
};
template <typename K, typename V>
bool operator==(const PriorityQueue<K,V> &lhs, const PriorityQueue<K,V> &rhs){
	return lhs.values == rhs.values;
}
template <typename K, typename V>
bool operator<(const PriorityQueue<K,V> &lhs, const PriorityQueue<K,V> &rhs){
	return lhs.values < rhs.values;
}
