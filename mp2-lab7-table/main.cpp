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
			out << "Пуста.\n\n";
		}
		out.close();
	}
	else {
		if (!(*pTable).IsEmpty()) {
			cout << (*pTable) << "\n";
		}
		else {
			cout << "Пуста.\n\n";
		}
	}
}

int main() {
	setlocale(LC_ALL, "russian");
	Randomex::Init();

	while (true) {
		cout << "Доступные типы таблиц:\n";
		cout << "1  -  Просматриваемая (TScanTable);\n";
		cout << "2  -  Отсортированная (TSortTable);\n";
		cout << "21 -  Отсортированная (TSortTable) через TScanTable" << " + QuickSort; \n";
		cout << "22 -  Отсортированная (TSortTable) через TScanTable" << " + SelectionSort; \n";
		cout << "3  -  Хэш-таблица на массиве (TArrayHashTable);\n";
		cout << "4  -  Хэш-таблица на списке (TListHashTable);\n";
		cout << "5  -  Таблица на дереве (TTreeTable);\n";
		cout << "6  -  Таблица на сбалансированном дереве " << "(TBalTreeTable).\n\n";

		int tableType;
		cout << "Тип таблицы: ";
		cin >> tableType;

		int tableSize = -1;

		if (tableType != 5 && tableType != 6) {
			cout << "Размер таблицы: ";
			cin >> tableSize;
		}

		int entriesCount;
		cout << "Число случайных записей (ключ-значение): ";
		cin >> entriesCount;

		int keyMax;
		cout << "Возможны ключи от 0 до ";
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

			cout << "\nВременно создана таблица: \n";
			cout << (*temp1) << "\n\n";

			T_SORTING_MODE = TSortingMode::QUICK_SORT;
			pTable = new TSortTable(*temp1);
			delete temp1;
			break;
		}
		case 22: {
			TScanTable* temp2 = new TScanTable(tableSize);
			fill_table(temp2, entriesCount, keyMax);

			cout << "\nВременно создана таблица: \n";
			cout << (*temp2) << "\n\n";

			T_SORTING_MODE = TSortingMode::SELECTION_SORT;
			pTable = new TSortTable(*temp2);
			delete temp2;
			break;
		}
		case 3: {
			int step;
			cout << "Шаг хэширования: ";
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

		cout << "Таблица сгенерирована.\n";
		cout << "Эффективность генерации: " << pTable->GetEff() << "\n\n";
		show_table(pTable);

		pTable->ClearEff();

		while (true) {
			TKey key;
			cout << "Ключ (или -1 для смены таблицы): ";
			cin >> key;
			cout << '\n';
			if (key == -1) {
				break;
			}

			cout << "Поиск (1), добавление (2) или удаление (3): ";
			int op;
			cin >> op;

			if (op == 1) {
				bool result = pTable->Find(key);
				if (result) {
					cout << "Удалось найти элемент " << key << ". " << "Эффективность: " << pTable->GetEff() << "\n\n";
				}
				else {
					cout << "Не найден элемент " << key << ". " << "Эффективность: " << pTable->GetEff() << "\n\n";
				}
				pTable->ClearEff();
			}
			else if (op == 2) {
				bool result = false;

				try {
					result = pTable->Insert(TRecord(key, "value" + to_string(key)));

					if (result) {
						cout << "Добавление успешно завершено. " << "Эффективность: " << pTable->GetEff() << "\n\n";
						cout << "Таблица теперь:\n";
						show_table(pTable);
					}
					else {
						cout << "Элемент с таким ключом уже есть в " << "таблице. Эффективность: " << pTable->GetEff() << "\n\n";
					}
				}
				catch (...) {
					cout << "Не удалось добавить элемент: " << "таблица переполнена.\n\n";
				}

				pTable->ClearEff();
			}
			else if (op == 3) {
				bool result = pTable->Delete(key);

				if (result) {
					cout << "Элемент успешно найден и удалён. " << "Эффективность: " << pTable->GetEff() << "\n\n";
					cout << "Таблица теперь:\n";
					show_table(pTable);
				}
				else {
					cout << "Элемент с таким ключом не найден. " << "Эффективность: " << pTable->GetEff() << '\n';
				}

				pTable->ClearEff();
			}
		}
	}
}