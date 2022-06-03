#pragma once
#include "TTable.h"
#include "TStack.h"

struct TTreeNode {
	TRecord rec;
	TTreeNode* pLeft, * pRight;
	
	int Bal;

	TTreeNode(
		TRecord _rec,
		TTreeNode* _pLeft = nullptr,
		TTreeNode* _pRight = nullptr);
};

TTreeNode::TTreeNode(TRecord _rec, TTreeNode* _pLeft, TTreeNode* _pRight) {
	rec = _rec;
	pLeft = _pLeft;
	pRight = _pRight;

	Bal = 0;
}

class TTreeTable : public TTable {
protected:
	TTreeNode* pRoot;
	TTreeNode* pPrev, * pCurr;
	TStack<TTreeNode*> st;

	int pos;
	void DelRecursively(TTreeNode* pDel);

	int level;
	void PrintRecursively(std::ostream& os, TTreeNode* tmp, bool printBal);

	bool IsBalanced(TTreeNode* node);
	int Height(TTreeNode* node);
public:
	TTreeTable();
	~TTreeTable();

	bool IsFull() const override;

	bool Find(TKey key) override;
	bool Insert(TRecord rec) override;
	bool Delete(TKey key) override;

	void Reset() override;
	void GoNext() override;
	bool IsEnd() override;

	bool IsBalanced();

	TKey GetCurrentKey() const override;
	TValue GetCurrentValue() const override;

	void Print(std::ostream& os = std::cout, bool printBal = false);
};

void TTreeTable::DelRecursively(TTreeNode* pDel) {
	if (pDel != nullptr) {
		DelRecursively(pDel->pRight);
		DelRecursively(pDel->pLeft);
		delete pDel;
	}
}

void TTreeTable::PrintRecursively(std::ostream& os, TTreeNode* pNode, bool printBal) {
	if (pNode != nullptr) {
		for (int i = 0; i < level; i++) {
			os << " ";
		}
		os << pNode->rec.key;
		if (printBal) {
			os << " (" << pNode->Bal << ")";
		}
		os << '\n';

		level++;
		PrintRecursively(os, pNode->pLeft, printBal);
		PrintRecursively(os, pNode->pRight, printBal);
		level--;
	}
}

bool TTreeTable::IsBalanced(TTreeNode* node) {
	int lh;
	int rh;

	if (node == nullptr) {
		return true;
	}

	lh = Height(node->pLeft);
	rh = Height(node->pRight);

	if (abs(lh - rh) <= 1 && IsBalanced(node->pLeft) && IsBalanced(node->pRight)) {
		return true;
	}
	return false;
}

int TTreeTable::Height(TTreeNode* node) {
	if (node == nullptr) {
		return 0;
	}
	return 1 + std::max(Height(node->pLeft), Height(node->pRight));
}

TTreeTable::TTreeTable() : TTable() {
	pRoot = pPrev = pCurr = nullptr;
	pos = 0;
}

TTreeTable::~TTreeTable() {
	DelRecursively(pRoot);
}

bool TTreeTable::IsFull() const {
	try {
		TTreeNode* newNode = new TTreeNode(TRecord());
		delete newNode;
		return false;
	}
	catch (...) {
		return true;
	}
}

bool TTreeTable::Find(TKey key) {
	pPrev = nullptr;
	pCurr = pRoot;

	while (pCurr != nullptr) {
		Eff++;
		if (pCurr->rec.key == key) {
			return true;
		}
		else if (key > pCurr->rec.key) {
			pPrev = pCurr;
			pCurr = pCurr->pRight;
		}
		else {
			pPrev = pCurr;
			pCurr = pCurr->pLeft;
		}
	}
	pCurr = pPrev;
	return false;
}

bool TTreeTable::Insert(TRecord rec) {
	if (IsFull()) {
		throw "Can't insert: no more space in table";
	}
	if (Find(rec.key)) {
		return false;
	}

	TTreeNode* newNode = new TTreeNode(rec);
	if (pRoot == nullptr) {
		pRoot = newNode;
	}
	else if (rec.key > pCurr->rec.key) {
		pCurr->pRight = newNode;
	}
	else {
		pCurr->pLeft = newNode;
	}

	Eff++;
	DataCount++;
	return true;
}

bool TTreeTable::Delete(TKey key) {
	if (!Find(key)) {
		return false;
	}
	TTreeNode* pDel = pCurr;
	if (pCurr->pRight == nullptr) {
		Eff++;
		if (pPrev == nullptr) {
			pRoot = pRoot->pLeft;
		}
		else {
			if (pCurr->rec.key > pPrev->rec.key) {
				pPrev->pRight = pCurr->pLeft;
			}
			else {
				pPrev->pLeft = pCurr->pLeft;
			}
		}
	}
	else if (pCurr->pLeft == nullptr) {
		Eff++;
		if (pPrev == nullptr) {
			pRoot = pRoot->pRight;
		}
		else {
			if (pCurr->rec.key > pPrev->rec.key) {
				pPrev->pRight = pCurr->pRight;
			}
			else {
				pPrev->pLeft = pCurr->pRight;
			}
		}
	}
	else {
		TTreeNode* tmp = pCurr->pLeft;

		pPrev = pCurr;
		while (tmp->pRight != nullptr) {
			Eff++;
			pPrev = tmp;
			tmp = tmp->pRight;
		}
		Eff++;

		pCurr->rec = tmp->rec;
		if (pPrev != pCurr) {
			pPrev->pRight = tmp->pLeft;
		}
		else {
			pPrev->pLeft = tmp->pLeft;
		}
		pDel = tmp;
	}

	delete pDel;
	DataCount--;
	Eff++;
	return true;
}

void TTreeTable::Reset() {
	st.clear();
	pCurr = pRoot;

	while (pCurr != nullptr) {
		st.push(pCurr);
		pCurr = pCurr->pLeft;
	}
	if (!IsEmpty()) {
		pCurr = st.top();
	}
	pos = 0;
}

void TTreeTable::GoNext() {
	TTreeNode* node = st.pop();
	if (pCurr->pRight != nullptr) {
		pCurr = pCurr->pRight;
		while (pCurr != nullptr) {
			st.push(pCurr);
			pCurr = pCurr->pLeft;
		}
		pCurr = st.top();
	}
	else {
		if (st.empty()) {
			pCurr = node->pRight;
		}
		else {
			pCurr = st.top();
		}
	}
	pos++;
}

bool TTreeTable::IsEnd() {
	return pos == DataCount;
}

TKey TTreeTable::GetCurrentKey() const {
	return pCurr->rec.key;
}

TValue TTreeTable::GetCurrentValue() const {
	return pCurr->rec.val;
}

void TTreeTable::Print(std::ostream& os, bool printBal) {
	PrintRecursively(os, pRoot, printBal);
	os << "IsBalanced: " << IsBalanced() << '\n';
}

bool TTreeTable::IsBalanced() {
	return IsBalanced(pRoot);
}