#pragma once
#include <vector>

static constexpr size_t id_buffer_size = 32;

typedef struct _DATA {
public:
	char m_ID[id_buffer_size];
	int m_UniqueID;
	_DATA* m_Next;

public:
	_DATA() : m_ID(), m_UniqueID(), m_Next() {};
	_DATA(const char* const ID, int UniqueID) : m_UniqueID(UniqueID), m_Next(nullptr) { strcpy_s(m_ID, id_buffer_size, ID); };

} DATA, *LPDATA;

class CLinearHashLL {
public:
	CLinearHashLL(const size_t BucketSize);
	~CLinearHashLL();

public:
	void Insert(const char* const Key, int UniqueKey);
	void Delete(const char* const Key);
	int Search(const char* const Key);
	int GetHash(const char* const Key);

private:
	std::vector<LPDATA> m_Buckets;

};