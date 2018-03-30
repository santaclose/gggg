#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <algorithm>
using namespace std;

struct rule
{
    string left;
    string right;
};

bool stringMatchesString(string a, string b, int position)
{
  for (int i = 0; i < b.length(); i++)
  {
    if (position + i >= a.length() || a.at(position + i) != b.at(i))
    {
      return false;
    }
  }
  return true;
}

bool stringContainsSubstr(string str, string substr)
{
  for (int i = 0; i < str.length() - substr.length() + 1; i++)
  {
    if (stringMatchesString(str, substr, i))
    {
      return true;
    }
  }
  return false;
}

bool stringContainsNonTerminal(string str, vector<string> nonTerminals)
{
  for (auto k : nonTerminals)
  {
    if (stringContainsSubstr(str, k))
    {
      return true;
    }
  }
  return false;
}

/*
void writeFinalStrings(vector<string> finalStrings)
{
  for (auto i : finalStrings)
  {
    cout << i << endl;
  }
}*/

vector<string> findAndReplace(string str, string toFind, string replaceWith)
{
  vector<int> positions;

  for (int i = 0; i < str.length(); i++)
  {
    if (stringMatchesString(str, toFind, i))
    {
      positions.push_back(i);
    }
  }

  vector<string> result;

  for (auto i : positions)
  {
    string temp = str;
    result.push_back(temp.replace(i, toFind.length(), replaceWith));
  }

  return result;
}

void iterate(rule* rules, int ruleCount, vector<string> currentStrings, vector<string> nonTerminals/*, ofstream &output*/)
{
  vector<string> newStrings;
  for (auto i : currentStrings)
  {
    for (int j = 0; j < ruleCount; j++)
    {
      if (stringContainsSubstr(i, rules[j].left)) // aplying rules
      {
        // returns all possible cases for the rule, just one replacement each time
        vector<string> ruleStrings = findAndReplace(i, rules[j].left, rules[j].right); // string, toFind, replaceWith
        for (auto i : ruleStrings) // insert only new strings
        {
          if (find(newStrings.begin(), newStrings.end(), i) == newStrings.end()) //newStrings does not contain i
          {
            newStrings.push_back(i);
          }
        }
        //newStrings.insert(newStrings.end(), ruleStrings.begin(), ruleStrings.end());
      }
    }
  }

  fstream output;
  output.open ("span.ls", fstream::out | fstream::app);
  cout << "iteration:" << endl;
  for (auto i : newStrings)
  {
    //cout << i << ", ";
    if (!stringContainsNonTerminal(i, nonTerminals))
    {
      cout << "found some string: \"" << i << "\"" << endl;
      output << i << endl; // write i to output
    }
  }
  output.close();
  cout << endl;

  if (newStrings.size() > 0)
  {
    iterate(rules, ruleCount, newStrings, nonTerminals/*, output*/);
  }
  else
  {
    //output.close();
  }

}


int main ()
{
    //ofstream ofs ("span.ls");
    ifstream ifs ("text.txt", ifstream::in);
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

    // set up rules

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
    currentStrings.push_back(nonTerminals.front()); // push axiom

    iterate(rules, ruleCount, currentStrings, nonTerminals/*, ofs*/);

    return 0;
}
