#pragma once
#include <iostream>
#include <sstream>
#include <string>

template <class T>
struct TNode {
	T value;
	TNode<T>* pNext;
};

template <class T>
class TList {
protected:
	TNode<T>* pFirst, * pCurr, * pPrev, * pLast, * pStop;
	int length;

	virtual void Print(std::ostream& os) const;
public:
	TList();
	virtual ~TList();

	bool IsEmpty();
	bool IsNotEmpty();

	virtual void InsFirst(T value);
	void InsLast(T value);
	void InsCurr(T value);

	virtual void DelFirst();
	void DelCurr();

	void Reset();
	void GoNext();
	bool IsEnd();
	bool IsNotEnd();

	TNode<T>* GetCurrNode();

	std::string ToStr() const;

	friend std::ostream& operator<<(std::ostream& os, TList<T>& lst) {
		lst.Print(os);
		return os;
	}
};

template <class T>
void TList<T>::Print(std::ostream& os) const {
	os << "[ ";
	TNode<T>* t = pFirst;
	while (t != pStop) {
		os << t->value << " ";
		t = t->pNext;
	}
	os << "]";
}

template<class T>
TList<T>::TList() {
	pStop = nullptr;
	pFirst = pLast = pPrev = pCurr = pStop;
	length = 0;
}

template<class T>
TList<T>::~TList() {
	while (pFirst != pStop) {
		TNode<T>* newNode = pFirst;
		pFirst = pFirst->pNext;
		delete newNode;
	}
}

template<class T>
bool TList<T>::IsEmpty() {
	return pFirst == pStop;
}

template <class T>
bool TList<T>::IsNotEmpty() {
	return pFirst != pStop;
}

template<class T>
void TList<T>::InsFirst(T value) {
	TNode<T>* newNode = new TNode<T>;
	newNode->value = value;
	newNode->pNext = pFirst;
	pFirst = newNode;
	length++;
	if (length == 1) {
		pLast = pFirst;
	}
}

template<class T>
void TList<T>::InsLast(T value) {
	if (length > 0) {
		TNode<T>* newNode = new TNode<T>();
		newNode->value = value;
		newNode->pNext = pStop;

		pLast->pNext = newNode;
		pLast = newNode;
		length++;
	}
	else {
		InsFirst(value);
	}
}

template<class T>
void TList<T>::InsCurr(T value) {
	if (pCurr == pFirst) {
		InsFirst(value);
		pPrev = pFirst;
	}
	else if (pCurr == pLast) {
		InsLast(value);
		pPrev = pLast;
	}
	else {
		TNode<T>* newNode = new TNode<T>();
		newNode->value = value;

		newNode->pNext = pCurr;
		pPrev->pNext = newNode;
		pCurr = newNode;
		length++;
	}
}

template<class T>
void TList<T>::DelFirst() {
	if (IsEmpty()) {
		throw "Can't delete first element: it's a barrier";
	}
	TNode<T>* newNode = pFirst;
	pFirst = pFirst->pNext;
	delete newNode;
	length--;
}

template<class T>
void TList<T>::DelCurr() {
	if (IsEmpty()) {
		throw "Can't delete current element: it's a barrier";
	}
	if (pCurr == pFirst) {
		DelFirst();
		pPrev = pStop;
		pCurr = pFirst;
	}
	else {
		TNode<T>* newNode = pCurr;
		pCurr = newNode->pNext;
		pPrev->pNext = pCurr;
		delete newNode;
		length--;
	}
}

template<class T>
void TList<T>::Reset() {
	pPrev = pStop;
	pCurr = pFirst;
}

template<class T>
void TList<T>::GoNext() {
	pPrev = pCurr;
	pCurr = pCurr->pNext;
}

template<class T>
bool TList<T>::IsEnd() {
	return pCurr == pStop;
}

template<class T>
TNode<T>* TList<T>::GetCurrNode() {
	if (pCurr == pStop) {
		throw "Can't get current element: it's a barrier";
	}
	return pCurr;
}

template <class T>
bool TList<T>::IsNotEnd() {
	return pCurr != pStop;
}

template <class T>
std::string TList<T>::ToStr() const {
	std::stringstream ss;
	Print(ss);
	return ss.str();
}
