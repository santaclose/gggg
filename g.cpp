#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <algorithm>
using namespace std;

struct rule {
    string left;
    string right;
};


int main ()
{
    ifstream ifs ("test.txt", ifstream::in);
    string fileContents((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());


    // get rule count
    int ruleCount = 0;

    for (int i = 0; i < fileContents.length(); i++)
    {
      if (fileContents.at(i) == '-' || fileContents.at(i) == '|')
      {
        ruleCount++;
      }
    }

    rule rules[ruleCount];
    vector<string> nonTerminals;

    // fill rules

    int currentRule = 0;
    int aux = 0;
    for (int i = 0; i < fileContents.length(); i++)
    {
      if (fileContents.at(i) == '-')
      {
        string leftSide = fileContents.substr(aux, i - aux);
        rules[currentRule].left = leftSide;
        if (find(nonTerminals.begin(), nonTerminals.end(), leftSide) == nonTerminals.end()) // if vector does not contain
        {
          nonTerminals.push_back(leftSide);
        }
        aux = i + 2;
      }
      else if (fileContents.at(i) == '|')
      {
        rules[currentRule].right = fileContents.substr(aux, i - aux);
        rules[currentRule + 1].left = rules[currentRule].left;
        currentRule++;
        aux = i + 1;
      }
      else if (fileContents.at(i) == '\n')
      {
        rules[currentRule].right = fileContents.substr(aux, i - aux);
        currentRule++;
        aux = i + 1;
      }
    }

    //for (auto i : nonTerminals) // access by value, the type of i is int
    //    cout << i << endl;

    vector<string> currentStrings;
    currentStrings.push_back(nonTerminals.front());

    for (int i = 0; i < ruleCount; i++)
    {
      if (rules[i].left.compare(axiom) == 0)
      {
        cout << rules[i].right << endl;
      }
    }

    return 0;
}
