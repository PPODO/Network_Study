#include "hash.h"
#include <iostream>

CLinearHashLL::CLinearHashLL(const size_t BucketSize) {
	new(&m_Buckets) std::vector<LPDATA>(BucketSize, nullptr);
}

CLinearHashLL::~CLinearHashLL() {
	for (auto& It : m_Buckets) {
		auto CurrentSlot = It;
		while (CurrentSlot != nullptr) {
			auto DeletePtr = CurrentSlot;
			CurrentSlot = CurrentSlot->m_Next;
			delete DeletePtr;
		}
	}
}

void CLinearHashLL::Insert(const char* const Key, int UniqueKey) {
	auto HashResult = GetHash(Key);

	if (HashResult < m_Buckets.size()) {
		if (!m_Buckets[HashResult]) {
			m_Buckets[HashResult] = new DATA(Key, UniqueKey);
		}
		else {
			auto CurrentNode = m_Buckets[HashResult];
			while (CurrentNode->m_Next != nullptr) {
				CurrentNode = CurrentNode->m_Next;
			}
			CurrentNode->m_Next = new DATA(Key, UniqueKey);
		}
	}
}

void CLinearHashLL::Delete(const char* const Key) {
	auto HashResult = GetHash(Key);

	if (HashResult < m_Buckets.size()) {
		auto CurrentNode = m_Buckets[HashResult];
		while (CurrentNode != nullptr) {
			if (strcmp(CurrentNode->m_ID, Key) == 0) {
				if (CurrentNode == m_Buckets[HashResult]) {
					m_Buckets[HashResult] = CurrentNode->m_Next;
				}
				CurrentNode = CurrentNode->m_Next;
				std::cout << "delete!\n";
				return;
			}
			CurrentNode = CurrentNode->m_Next;
		}
		std::cout << "nonexistent user!\n";
	}
}

int CLinearHashLL::Search(const char* const Key) {
	auto hashResult = GetHash(Key);
	
	if (hashResult < m_Buckets.size()) {
		auto CurrentNode = m_Buckets[hashResult];
		while (CurrentNode != nullptr) {
			if (strcmp(CurrentNode->m_ID, Key) == 0) {
				return CurrentNode->m_UniqueID;
			}
		}
	}
	return -1;
}

int CLinearHashLL::GetHash(const char* const Key) {
	size_t Length = strlen(Key);
	int Sum = 0;
	for (size_t i = 0; i < Length; i++) {
		Sum += Key[i];
	}
	return Sum % m_Buckets.size();
}
