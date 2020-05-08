#pragma once

#include <iostream>
#include <string>
#include <istream>
#include <unordered_map>
#include <sstream>
#include "NFA.h"

class RegularGrammar
{
private:

    struct StateTransition
    {
        char        character;
        std::string nextState;
    };

public:

    RegularGrammar(char, std::string);

    NFA ToDFA();

    friend std::istream& operator>>(std::istream&, RegularGrammar&);

private:

    void ToLambdaFree();

private:

    char                                                                           m_lambda;
    std::string                                                                    m_startSymbol;
    std::unordered_map<std::string, std::pair<bool, std::vector<StateTransition>>> m_grammar;
};