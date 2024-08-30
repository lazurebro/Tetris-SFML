#pragma once

#include <list>
#include <iterator>

class CodeKonami
{
	std::list<int> CODE;
	std::list<int> input;
	std::list<int>::iterator iter;
	int correct_size = 0;
public:
	CodeKonami( std::list<int>&& _CODE );

	bool addSymbol( int symbol ); // вернет true если сработает комбинация
	void clearInput();
};