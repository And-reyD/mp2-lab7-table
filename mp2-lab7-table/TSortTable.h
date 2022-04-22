#pragma once
#include "TScanTable.h"

class TSortTable : public TScanTable {
public:
	TSortTable(int _size = 10) : TScanTable(_size) {};
	TSortTable(TScanTable& t) : TScanTable(t.GetSize()) {
		int i;
		for (i = 0, t.Reset(); i < DataCount && !t.IsEnd(); i++, t.GoNext()) {
			arr[i].key = t.GetCurrentKey();
			arr[i].val = t.GetCurrentValue();
		}
		Sort();
	};

	bool Find(TKey key);
	bool Insert(TRecord rec);
	bool Delete(TKey key);

	void Sort(int flag = 0);
	void QuickSort(int first, int last);
	void SelectionSort();
};

bool TSortTable::Find(TKey key) {
	int begin = 0, end = DataCount - 1, mid;
	while (begin <= end) {
		Eff++;
		mid = (begin + end) / 2;
		if (key < arr[mid].key) {
			end = mid - 1;
		}
		else if (key > arr[mid].key) {
			begin = end + 1;
		}
		else {
			curr = mid;
			return true;
		}
	}
	curr = begin;
	return false;
}

bool TSortTable::Insert(TRecord rec) {
	if (IsFull()) {
		return false;
	}
	if (Find(rec.key)) {
		return false;
	}

	for (int i = DataCount - 1; i >= curr; i--) {
		arr[i + 1] = arr[i];
		Eff++;
	}

	arr[curr] = rec;
	DataCount++;
	Eff++;
	return true;
}

bool TSortTable::Delete(TKey key) {
	if (!Find(key)) {
		return false;
	}

	for (int i = curr; i < DataCount - 1; i++) {
		arr[i] = arr[i + 1];
		Eff++;
	}

	DataCount--;
	Eff++;
	return true;
}

void TSortTable::Sort(int flag = 0) {
	if (flag) {
		SelectionSort();
	}
	else {
		QuickSort(0, DataCount - 1);
	}
}

void TSortTable::QuickSort(int first, int last) {
	TRecord el = arr[(first + last) / 2];
	int i = first, j = last;

	TRecord tmp;
	while (i < j) {
		while (arr[i] < el) {
			i++;
			Eff++;
		}
		while (arr[j] > el) {
			j--;
			Eff++;
		}

		if (i <= j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;

			i++;
			j--;
			Eff++;
		}
	}

	if (first < j) {
		QuickSort(first, j);
	}
	if (i < last) {
		QuickSort(i, last);
	}
}

void TSortTable::SelectionSort() {
	int pos;
	TRecord tmp;
	for (int i = 0; i < DataCount; i++) {
		pos = i;
		for (int j = i; j < DataCount; j++) {
			if (arr[j] < arr[pos]) {
				pos = j;
			}
			Eff++;
		}
		Eff;
		tmp = arr[pos];
		arr[pos] = arr[i];
		arr[i] = tmp;
	}
}