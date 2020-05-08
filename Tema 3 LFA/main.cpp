#include <fstream>

#include "NFA.h"
#include "RegularGrammar.h"

using namespace std;

int main()
{
    RegularGrammar grammar('*', "S");
    ifstream fin("data.txt");
    fin >> grammar;
    fin.close();

    NFA dfa = grammar.ToDFA();

    cout << dfa << endl;

    return 0;
}