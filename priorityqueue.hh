#include <map>
#include <cstdlib>
#include <utility>
#include <cassert>
#include <iostream>

using namespace std;

template <typename K>
struct classcomp {
  bool operator() (const K * lhs, const K * rhs) const
  {return *lhs < *rhs;}
};


template <typename K, typename V> 
class PriorityQueue {
	
	std::multimap <V, K> values;
	std::map <K*, typename std::multimap <V, K>::iterator , classcomp<K> > iterators;
	
	public:
	// Konstruktor bezparametrowy tworzący pustą kolejkę [O(1)]
	PriorityQueue(){}

	// Konstruktor kopiujący [O(queue.size())]
	PriorityQueue(const PriorityQueue<K, V>& queue){
		for (auto it = queue.values.begin(); it != queue.values.end(); it++)
			this->insert(it->second, it->first);
	}

	// Konstruktor przenoszący [O(1)]
	PriorityQueue(PriorityQueue<K, V>&& queue) = default;

	// Operator przypisania [O(queue.size()) dla użycia P = Q, a O(1) dla użycia
	// P = move(Q)]
	PriorityQueue<K, V>& operator=(PriorityQueue<K, V> queue);

	// Metoda zwracająca true wtedy i tylko wtedy, gdy kolejka jest pusta [O(1)]
	bool empty() const{
		return values.empty();
	}

	// Metoda zwracająca liczbę par (klucz, wartość) przechowywanych w kolejce
	// [O(1)]
	std::size_t size() const{ //TODO: poprawic na size_type
		return values.size();
	}

	// Metoda wstawiająca do kolejki parę o kluczu key i wartości value
	// [O(log size())] (dopuszczamy możliwość występowania w kolejce wielu
	// par o tym samym kluczu)
	void insert(const K& key, const V& value){
		auto it = values.emplace(value, key);
		auto elem = &it->second;
		iterators.emplace(elem, it);
	}
	

	// Metody zwracające odpowiednio najmniejszą i największą wartość przechowywaną
	// w kolejce [O(1)]; w przypadku wywołania którejś z tych metod na pustej
	// strukturze powinien zostać zgłoszony wyjątek PriorityQueueEmptyException
	const V& minValue() const{
		return values.begin()->first;
	}	
	const V& maxValue() const{
		return values.rbegin()->first;
	}

	// Metody zwracające klucz o przypisanej odpowiednio najmniejszej lub
	// największej wartości [O(1)]; w przypadku wywołania którejś z tych metod
	// na pustej strukturze powinien zostać zgłoszony wyjątek
	// PriorityQueueEmptyException
	const K& minKey() const{
		return values.begin()->second;
	}
	const K& maxKey() const{
		return values.rbegin()->second;
	}
	private:
	void deleteElem(const K& key){
		K nowe = key;
		K * wskaznik = &nowe;
		auto it = iterators.find(wskaznik);
		values.erase(it->second);
		iterators.erase(it);
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
		deleteElem(key);
		this->insert(key, value);
	}
	
	// Metoda scalająca zawartość kolejki z podaną kolejką queue; ta operacja usuwa
	// wszystkie elementy z kolejki queue i wstawia je do kolejki *this
	// [O(queue.size() * log (queue.size() + size()))]
	void merge(PriorityQueue<K, V>& queue){
		for(auto elem : queue.values){
			this->insert(elem.second->first, elem.first);
			queue.delete_elem(elem.second);
		}
	}

	// Metoda zamieniającą zawartość kolejki z podaną kolejką queue (tak jak
	// większość kontenerów w bibliotece standardowej) [O(1)]
	void swap(PriorityQueue<K, V>& queue){
		
	}
};
