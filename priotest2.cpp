#include <iostream>
#include <exception>
#include <cassert>

#include "priorityqueue.hh"

PriorityQueue<int, int> f(PriorityQueue<int, int> q)
{
    return q;
}

int main() {
    PriorityQueue<int, int> P;
    assert(P.empty());

    P.insert(1, 42);

    P.insert(2, 13);
    P.insert(2, 14);

    assert(P.size() == 3);
    assert(P.maxKey() == 1);
    assert(P.maxValue() == 42);
    assert(P.minKey() == 2);
	
    assert(P.minValue() == 13);
    PriorityQueue<int, int> Q(f(P));
   Q.deleteMax();
    Q.deleteMin();

    Q.deleteMin();
    assert(Q.empty());
    PriorityQueue<int, int> R(Q);

    R.insert(1, 100);
    R.insert(2, 100);
    R.insert(3, 300);
    
    PriorityQueue<int, int> S;
    S = R;
    
    try
    {
        S.changeValue(4, 400);
    }
    catch (const PriorityQueueNotFoundException& pqnfex)
    {
        std::cout << pqnfex.what() << std::endl;
    }
    catch (...)
    {
        assert(!"exception missing!");
    }

    S.changeValue(2, 200);

    try
    {
        while (true)
        {
            std::cout << S.minValue() << std::endl;
            S.deleteMin();
        }
        assert(!"S.minValue() on empty S did not throw!");
    }
    catch (const PriorityQueueEmptyException& pqeex)
    {
        std::cout << pqeex.what() << std::endl;
    }
    catch (...)
    {
        assert(!"exception missing!");
    }

    try
    {
        S.minKey();
        assert(!"S.minKey() on empty S did not throw!");
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
    catch (...)
    {
        assert(!"exception missing!");
    }

    PriorityQueue<int, int> T;
    T.insert(1, 1);
    T.insert(2, 4);
    S.insert(3, 9);
    S.insert(4, 16);
    S.merge(T);
    assert(S.size() == 4);
    assert(S.minValue() == 1);
    assert(S.maxValue() == 16);
    assert(T.empty());

    S = R;
    swap(R, T);
    assert(T == S);
   // assert(T != R);

    R = std::move(S);
  //  assert(T != S);
    assert(T == R);

    std::cout << "ALL OK!" << std::endl;

    return 0;
}
/*
UWAGA! Każda implementacja PriorityQueue<K, V> zostanie sprawdzona zestawem
testów z typami, które np. zgłaszają wyjątki ze swoich metod, operatorów itd.
(oczywiście poza destruktorem). Symulowane będą też niepowodzenia w alokacji
pamięci (bad_alloc). Przejście tych testów będzie podstawą do zaliczenia.
Warto zatem spróbować samemu zasymulować takie warunki. Wskazówka, jak to
zrobić, znajduje się w jednej z obowiązkowych czytanek.

Ustalenia techniczne
^^^^^^^^^^^^^^^^^^^^
Jako rozwiązanie należy dostarczyć plik priorityqueue.hh.
Plik ten należy umieścić w repozytorium w katalogu

grupaN/zadanie5/ab123456+cd123456

lub

grupaN/zadanie5/ab123456+cd123456+ef123456

gdzie N jest numerem grupy, a ab123456, cd123456, ef123456 są identyfikatorami
członków zespołu umieszczającego to rozwiązanie. Katalog z rozwiązaniem nie
powinien zawierać innych plików, ale może zawierać podkatalog private, gdzie
można umieszczać różne pliki, np. swoje testy. Pliki umieszczone w tym
podkatalogu nie będą oceniane.
*/
