#include <iostream>
#include "hash.h"

int main() {
	CLinearHashLL Hash(12);

	Hash.Insert("PPODO", 101);
	Hash.Insert("PPODO", 141);

	std::cout << Hash.Search("PPODO") << std::endl;

	Hash.Delete("PPODO");

	return 0;
}