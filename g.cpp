#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
using namespace std;

struct rule {
    string left;
    string right;
};


int main ()
{
    ifstream ifs ("test.txt", ifstream::in);
    string fileContents((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());

    int ruleCount = 0;

    for (int i = 0; i < fileContents.length(); i++)
    {
      if (fileContents.at(i) == '>' || fileContents.at(i) == '|')
      {
        ruleCount++;
      }
    }

    rule rules[ruleCount];

    int currentRule = 0;
    int auxLeft = 0;
    for (int i = 0; i < fileContents.length(); i++)
    {
      if (fileContents.at(i) == '-')
      {
        rules[currentRule].left = fileContents.substr(auxLeft, i);
        auxLeft = i + 2;
      }
      else if (fileContents.at(i) == '|')
      {
        rules[currentRule].right = fileContents.substr(auxLeft, i);
        rules[currentRule + 1].left = rules[currentRule].left;
        currentRule++;
        auxLeft = i + 1;
      }
      else if (fileContents.at(i) == '\n')
      {
        rules[currentRule].right = fileContents.substr(auxLeft, i);
        currentRule++;
        auxLeft = i + 1;
      }
    }

    for (int i = 0; i < ruleCount; i++)
    {
      cout << rules[i].left << ", " << rules[i].right << endl;
    }

    //cout << asdf << endl << "wwwwwwwwwwwwwwwwwwwwww" << endl;
    //char c = ifs.get();

    //while (ifs.good())
    //{
    //  std::cout << c;
    //  c = ifs.get();
    //}

    //ifs.close();
/*
    rule papa;
    papa.left = "S";
    papa.right = "dffasfsdsdfa";


    cout << papa.right << endl;

    size_t f = papa.right.find("fa");
    papa.right.replace(f, std::string("fa").length(), "w");


    cout << papa.right << endl;*/

    /*string str = "asdf";
    str.append("waawawa");

    ofstream myfile;
    myfile.open ("example.txt");
    myfile << "Writing this to a file.\n";
    myfile.close();

    cout << str << '\n';*/
    return 0;
}
