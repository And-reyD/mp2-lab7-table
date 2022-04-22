#pragma once
#include "TTable.h"

class TArrayTable : public TTable {
protected:
	TRecord* arr;
	int size, curr;
public:
	TArrayTable(int _size = 10);
	~TArrayTable();

	bool IsFull();

	TKey GetCurrentKey() const override;
	TValue GetCurrentValue() const override;

	void Reset();
	void GoNext();
	bool IsEnd();

	int GetSize() const;
};

TArrayTable::TArrayTable(int _size) {
	size = _size;
	arr = new TRecord[size];
	curr = -1;
}

TArrayTable::~TArrayTable() {
	delete[] arr;
}

bool TArrayTable::IsFull() {
	return DataCount == size;
}

TKey TArrayTable::GetCurrentKey() const {
	return arr[curr].key;
}

TValue TArrayTable::GetCurrentValue() const {
	return arr[curr].val;
}

void TArrayTable::Reset() {
	curr = 0;
}

void TArrayTable::GoNext() {
	curr++;
}

bool TArrayTable::IsEnd() {
	return curr == DataCount;
}

int TArrayTable::GetSize() const {
	return size;
}
