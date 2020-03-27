#include <iostream>
#include "hash.h"

int main() {
	CLinearHash hash(10);

	hash.Insert("PPODO", 101);
	hash.Insert("HANNAM", 55);
	hash.Insert("EVER", 22);

	auto Result1 = hash.Search("PPODO");
	std::cout << Result1 << std::endl;

	auto Result2 = hash.Search("EVET");
	std::cout << Result2 << std::endl;

	hash.Delete("HANNAM");

	return 0;
}