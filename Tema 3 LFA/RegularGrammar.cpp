#include "RegularGrammar.h"

using namespace std;

RegularGrammar::RegularGrammar(char   lambda, 
                               string startSymbol) :
    m_lambda(lambda),
    m_startSymbol(startSymbol)
{
}

NFA RegularGrammar::ToDFA()
{
    NFA result(m_lambda);
    ToLambdaFree();

    unordered_map<string, int> indices;
    int index = 0;

    for (auto& keyValue : m_grammar)
        indices[keyValue.first] = index++;

    int n = index + 1;
    vector<pair<int, pair<int, char>>> links;

    for (auto& keyValue : m_grammar)
        for (auto& transition : m_grammar[keyValue.first].second)
        {
            int ix1 = indices[keyValue.first];
            int ix2 = n - 1;
            if (transition.nextState.size() >= 1)
                ix2 = indices[transition.nextState];
            char character = transition.character;

            links.push_back(make_pair(ix1, make_pair(ix2, character)));
        }

    int m = links.size();

    stringstream ss;
    ss << n << " " << m << endl;
    for (int i = 0; i < m; i++)
        ss << links[i].first << " " << links[i].second.first << " " << links[i].second.second << endl;
    ss << indices[m_startSymbol] << " " << 1 << endl;
    ss << n - 1;

    ss >> result;

    result.RemoveLambda();
    result.ToDFA();
    result.Minimize();

    return result;
}

istream& operator>>(istream& in, RegularGrammar& grammar)
{
    string state, transition;
    while (in >> state >> transition)
    {
        if (grammar.m_grammar.find(state) == grammar.m_grammar.end())
            grammar.m_grammar[state] = make_pair(false, vector<RegularGrammar::StateTransition>());

        RegularGrammar::StateTransition stateTransition;
        stateTransition.character = transition.at(0);

        if (transition.size() >= 2)
            stateTransition.nextState = transition.substr(1, transition.size() - 1);
        else
            stateTransition.nextState = string();

        grammar.m_grammar[state].second.push_back(stateTransition);
        grammar.m_grammar[state].first = grammar.m_grammar[state].first || (stateTransition.character == grammar.m_lambda);
    }

    return in;
}

void RegularGrammar::ToLambdaFree()
{
    unordered_map<string, pair<bool, vector<StateTransition>>> newGrammar;

    for (auto& keyVal : m_grammar)
    {
        for (auto& transition : keyVal.second.second)
        {
            if (newGrammar.find(keyVal.first) == newGrammar.end())
                newGrammar[keyVal.first] = make_pair(false, vector<StateTransition>());

            if (transition.character != m_lambda)
                newGrammar[keyVal.first].second.push_back(transition);
        }
    }

    for (auto& keyVal : newGrammar)
    {
        vector<char> toAdd;

        for (auto& transition : keyVal.second.second)
            if (m_grammar.find(transition.nextState) != m_grammar.end())
                if (m_grammar[transition.nextState].first)
                    toAdd.push_back(transition.character);

        for (auto& add : toAdd)
        {
            StateTransition transition;
            transition.character = add;
            transition.nextState = "";
            keyVal.second.second.push_back(transition);
        }
    }

    if (m_grammar[m_startSymbol].first)
    {
        string newStartSymbol = m_startSymbol + "1";
        newGrammar[newStartSymbol] = make_pair(true, vector<StateTransition>());
        for (auto& transition : newGrammar[m_startSymbol].second)
            newGrammar[newStartSymbol].second.push_back(transition);

        StateTransition stateTransition;

        stateTransition.character = m_lambda;
        stateTransition.nextState = string();

        newGrammar[newStartSymbol].second.push_back(stateTransition);
        m_startSymbol = newStartSymbol;
    }

    m_grammar = move(newGrammar);
}