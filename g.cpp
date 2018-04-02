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

struct grammar
{
  rule* rules;
  int ruleCount;
  vector<string> nonTerminals;
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

vector<int> getMatchingIndexes(string str, string toFind)
{
  vector<int> positions;

  for (int i = 0; i < str.length(); i++)
  {
    if (stringMatchesString(str, toFind, i))
    {
      positions.push_back(i);
    }
  }
  return positions;
}

vector<string> findAndReplace(string str, string toFind, string replaceWith)
{
  vector<int> positions = getMatchingIndexes(str, toFind);
  vector<string> result;

  for (auto i : positions)
  {
    string temp = str;
    result.push_back(temp.replace(i, toFind.length(), replaceWith));
  }

  return result;
}

void breadth(rule* rules, int ruleCount, vector<string> currentStrings, vector<string> nonTerminals, string outputFile)
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

  if (newStrings.size() > 0)
  {
    fstream output;
    output.open (outputFile, fstream::out | fstream::app);
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
    breadth(rules, ruleCount, newStrings, nonTerminals, outputFile);
  }
}

void randomGen(rule* rules, int ruleCount, vector<string> nonTerminals, int stringNumber, string outputFile)
{
  ofstream output;
  output.open (outputFile);
  for (int i = 0; i < stringNumber; i++)
  {
    string str = nonTerminals.front();
    while (stringContainsNonTerminal(str, nonTerminals))
    {
      rule currentRule = rules[rand() % ruleCount];
      if (stringContainsSubstr(str, currentRule.left))
      {
        vector<int> positions = getMatchingIndexes(str, currentRule.left);
        str.replace(positions[rand() % positions.size()], currentRule.left.length(), currentRule.right);
      }
    }
    output << str << endl;
    //cout << "generated string: " << str << endl;
  }
  output.close();
}

static grammar getGrammar(string filePath)
{
  ifstream ifs (filePath, ifstream::in);
  string fileContents((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());

  grammar grmr;

  int rc = 0;

  for (int i = 0; i < fileContents.length(); i++)
  {
    if (fileContents.at(i) == '-' || fileContents.at(i) == '|')
    {
      rc++;
    }
  }

  grmr.ruleCount = rc;

  grmr.rules = new rule[rc];

  // vector<string> nonTerminals;

  // set up rules

  int currentRule = 0;
  int aux = 0;
  for (int i = 0; i < fileContents.length(); i++)
  {
    if (fileContents.at(i) == '-')
    {
      string leftSide = fileContents.substr(aux, i - aux);
      grmr.rules[currentRule].left = leftSide;
      if (find(grmr.nonTerminals.begin(), grmr.nonTerminals.end(), leftSide) == grmr.nonTerminals.end()) // if vector does not contain
      {
        grmr.nonTerminals.push_back(leftSide);
      }
      aux = i + 2;
    }
    else if (fileContents.at(i) == '|')
    {
      grmr.rules[currentRule].right = fileContents.substr(aux, i - aux);
      grmr.rules[currentRule + 1].left = grmr.rules[currentRule].left;
      currentRule++;
      aux = i + 1;
    }
    else if (fileContents.at(i) == '\n')
    {
      grmr.rules[currentRule].right = fileContents.substr(aux, i - aux);
      currentRule++;
      aux = i + 1;
    }
  }

  return grmr;
}

int main (int argc, char** argv)
{
  // g r 10000 in.txt span.ls
  // g r 10000 in.txt
  // g b in.txt span.ls
  // g b in.txt

    if (argc < 3 || argc > 5)
    {
      cout << "did nothing" << endl;
      return -1;
    }

    if (argv[1][0] == 'r')
    {
      int count = atoi(argv[2]);
      string inputFile = argv[3];
      string outputFile = "span.ls";
      if (argc > 4)
        outputFile = argv[4];

      grammar grmr = getGrammar(inputFile);

      randomGen(grmr.rules, grmr.ruleCount, grmr.nonTerminals, count, outputFile);
    }
    else
    {
      string inputFile = argv[2];
      string outputFile = "span.ls";
      if (argc > 3)
        outputFile = argv[3];

      grammar grmr = getGrammar(inputFile);

      vector<string> currentStrings;
      currentStrings.push_back(grmr.nonTerminals.front());
      breadth(grmr.rules, grmr.ruleCount, currentStrings, grmr.nonTerminals, outputFile);
    }
    return 0;
}
