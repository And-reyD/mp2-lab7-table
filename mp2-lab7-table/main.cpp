#include <iostream>
#include <fstream>
#include <clocale>
#include <cmath>
#include "TSortTable.h"
#include "TArrayHashTable.h"
#include "TListHashTable.h"
#include "TTreeTable.h"
#include "TBalTreeTable.h"
#include "Randomex.h"

using namespace std;

TTable* pTable;

void fill_table(TTable* table, int entriesCount, int keyMax) {
	for (int i = 0; i < entriesCount; i++) {
		TKey randKey = Randomex::RandInt(0, keyMax);

		bool inserted = table->Insert(TRecord(randKey, "value" + to_string(randKey)));
		if (!inserted) {
			i--;
		}
	}
}

void show_table(TTable* table) {
	if (typeid(*table) == typeid(TTreeTable) || typeid(*table) == typeid(TBalTreeTable)) {
		std::ofstream out;
		out.open("..\\out.txt");
		if (!(*pTable).IsEmpty()) {
			((TTreeTable*)table)->Print(out);
			out << "\n";
		}
		else {
			out << "�����.\n\n";
		}
		out.close();
	}
	else {
		if (!(*pTable).IsEmpty()) {
			cout << (*pTable) << "\n";
		}
		else {
			cout << "�����.\n\n";
		}
	}
}

int main() {
	setlocale(LC_ALL, "russian");
	Randomex::Init();

	while (true) {
		cout << "��������� ���� ������:\n";
		cout << "1  -  ��������������� (TScanTable);\n";
		cout << "2  -  ��������������� (TSortTable);\n";
		cout << "21 -  ��������������� (TSortTable) ����� TScanTable" << " + QuickSort; \n";
		cout << "22 -  ��������������� (TSortTable) ����� TScanTable" << " + SelectionSort; \n";
		cout << "3  -  ���-������� �� ������� (TArrayHashTable);\n";
		cout << "4  -  ���-������� �� ������ (TListHashTable);\n";
		cout << "5  -  ������� �� ������ (TTreeTable);\n";
		cout << "6  -  ������� �� ���������������� ������ " << "(TBalTreeTable).\n\n";

		int tableType;
		cout << "��� �������: ";
		cin >> tableType;

		int tableSize = -1;

		if (tableType != 5 && tableType != 6) {
			cout << "������ �������: ";
			cin >> tableSize;
		}

		int entriesCount;
		cout << "����� ��������� ������� (����-��������): ";
		cin >> entriesCount;

		int keyMax;
		cout << "�������� ����� �� 0 �� ";
		cin >> keyMax;

		switch (tableType) {
		case 1: {
			pTable = new TScanTable(tableSize);
			fill_table(pTable, entriesCount, keyMax);
			break;
		}
		case 2: {
			pTable = new TSortTable(tableSize);
			fill_table(pTable, entriesCount, keyMax);
			break;
		}
		case 21: {
			TScanTable* temp1 = new TScanTable(tableSize);
			fill_table(temp1, entriesCount, keyMax);

			cout << "\n�������� ������� �������: \n";
			cout << (*temp1) << "\n\n";

			T_SORTING_MODE = TSortingMode::QUICK_SORT;
			pTable = new TSortTable(*temp1);
			delete temp1;
			break;
		}
		case 22: {
			TScanTable* temp2 = new TScanTable(tableSize);
			fill_table(temp2, entriesCount, keyMax);

			cout << "\n�������� ������� �������: \n";
			cout << (*temp2) << "\n\n";

			T_SORTING_MODE = TSortingMode::SELECTION_SORT;
			pTable = new TSortTable(*temp2);
			delete temp2;
			break;
		}
		case 3: {
			int step;
			cout << "��� �����������: ";
			cin >> step;

			pTable = new TArrayHashTable(tableSize, step);
			fill_table(pTable, entriesCount, keyMax);
			break;
		}
		case 4: {
			pTable = new TListHashTable(tableSize);
			fill_table(pTable, entriesCount, keyMax);
			break;
		}
		case 5: {
			pTable = new TTreeTable();
			fill_table(pTable, entriesCount, keyMax);
			break;
		}
		case 6: {
			pTable = new TBalTreeTable();
			fill_table(pTable, entriesCount, keyMax);
			break;
		}
		}

		cout << "������� �������������.\n";
		cout << "������������� ���������: " << pTable->GetEff() << "\n\n";
		show_table(pTable);

		pTable->ClearEff();

		while (true) {
			TKey key;
			cout << "���� (��� -1 ��� ����� �������): ";
			cin >> key;
			cout << '\n';
			if (key == -1) {
				break;
			}

			cout << "����� (1), ���������� (2) ��� �������� (3): ";
			int op;
			cin >> op;

			if (op == 1) {
				bool result = pTable->Find(key);
				if (result) {
					cout << "������� ����� ������� " << key << ". " << "�������������: " << pTable->GetEff() << "\n\n";
				}
				else {
					cout << "�� ������ ������� " << key << ". " << "�������������: " << pTable->GetEff() << "\n\n";
				}
				pTable->ClearEff();
			}
			else if (op == 2) {
				bool result = false;

				try {
					result = pTable->Insert(TRecord(key, "value" + to_string(key)));

					if (result) {
						cout << "���������� ������� ���������. " << "�������������: " << pTable->GetEff() << "\n\n";
						cout << "������� ������:\n";
						show_table(pTable);
					}
					else {
						cout << "������� � ����� ������ ��� ���� � " << "�������. �������������: " << pTable->GetEff() << "\n\n";
					}
				}
				catch (...) {
					cout << "�� ������� �������� �������: " << "������� �����������.\n\n";
				}

				pTable->ClearEff();
			}
			else if (op == 3) {
				bool result = pTable->Delete(key);

				if (result) {
					cout << "������� ������� ������ � �����. " << "�������������: " << pTable->GetEff() << "\n\n";
					cout << "������� ������:\n";
					show_table(pTable);
				}
				else {
					cout << "������� � ����� ������ �� ������. " << "�������������: " << pTable->GetEff() << '\n';
				}

				pTable->ClearEff();
			}
		}
	}
}