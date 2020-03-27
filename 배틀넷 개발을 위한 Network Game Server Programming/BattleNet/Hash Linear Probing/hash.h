#pragma once
#include <iostream>
#include <vector>

static constexpr size_t not_used = -1;

typedef struct _DATA {
public:
	char m_ID[32];
	int m_UniqueIndex;

public:
	_DATA() {};
	_DATA(const char* const ID, int UniqueKey) { strcpy_s(m_ID, 32, ID); m_UniqueIndex = UniqueKey; };

}DATA;

class CLinearHash {
public:
	CLinearHash(int BucketSize) {
		new (&m_Bucket) std::vector<DATA*>(BucketSize, nullptr);
	}
	~CLinearHash() {
		for (auto It : m_Bucket) {
			auto lpData = reinterpret_cast<DATA*>(It);
			delete lpData;
		}
	}

public:
	void Insert(const char* const Key, int UniqueIndex) {
		auto hashValue = HashFunc(Key);
		while (m_Bucket[hashValue] != nullptr) {
			hashValue++;
			if (hashValue >= m_Bucket.size()) { std::cout << "Insert Failure!\n";  break; }
		}

		DATA* lpData = new DATA(Key, UniqueIndex);
		m_Bucket[hashValue] = lpData;
	}
	
	void Delete(const char* const Key) {
		auto hashValue = HashFunc(Key);
		while (true) {
			auto lpData = reinterpret_cast<DATA*>(m_Bucket[hashValue]);
			if (lpData != nullptr && strcmp(lpData->m_ID, Key) == 0) {
				delete lpData;
				m_Bucket[hashValue] = nullptr;
				break;
			}
			hashValue++;
			if (hashValue >= m_Bucket.size()) { std::cout << "Failed!\n"; break; }
		}
	}

	int Search(const char* const Key) {
		auto hashValue = HashFunc(Key);
		while (true) {
			auto lpData = reinterpret_cast<DATA*>(m_Bucket[hashValue]);
			if (lpData != nullptr && strcmp(lpData->m_ID, Key) == 0) {
				return lpData->m_UniqueIndex;
			}
			hashValue++;
			if (hashValue >= m_Bucket.size()) { std::cout << "Doesn't Exist!\n"; break; }
		}
		return not_used;
	}

	int HashFunc(const char* const Key) {
		size_t strLength = strlen(Key);

		int sum = 0;
		for (size_t i = 0; i < strLength; i++) {
			sum += Key[i];
		}
		return sum % m_Bucket.size();
	}

private:
	std::vector<DATA*> m_Bucket;

};