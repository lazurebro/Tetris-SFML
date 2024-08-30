#include "CodeKonamiHandler.h"

CodeKonami::CodeKonami(std::list<int>&& _CODE)
{
	CODE = _CODE;
	iter = CODE.begin();
	correct_size = 0;
}

bool CodeKonami::addSymbol(int c_input)
{
    if (c_input == *iter)
    {
        input.push_back(c_input);
        iter++;
        correct_size++;
        if (correct_size == CODE.size())
        {
            clearInput();

            return true;
        }
    }
    else
    {
        clearInput();
    }

    return false;
}

void CodeKonami::clearInput()
{
    input.clear();
    iter = CODE.begin();
    correct_size = 0;
}


