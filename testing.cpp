#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <algorithm>
using namespace std;

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

void writeFinalStrings(vector<string> strings, vector<string> nonTerminals)
{
  for (auto i : strings)
  {
    if (!stringContainsNonTerminal(i, nonTerminals))
    {
      cout << i << endl;
    }
  }
}

int main()
{
  vector<string> newStrings;
  vector<string> nonTerminals;

  newStrings.push_back("aBc");
    newStrings.push_back("Bbba");
      newStrings.push_back("aaabC");
        newStrings.push_back("ababa");


  nonTerminals.push_back("B");
nonTerminals.push_back("C");

  writeFinalStrings(newStrings, nonTerminals);
}
