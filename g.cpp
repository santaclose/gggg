#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct product {
    string left;
    string right;
} ;


int main ()
{
    product papa;
    papa.left = "S";
    papa.right = "dffasfsdsdfa";
    
    
    cout << papa.right << endl;
    
    size_t f = papa.right.find("fa");
    papa.right.replace(f, std::string("fa").length(), "w");
    
    
    cout << papa.right << endl;
    
    /*string str = "asdf";
    str.append("waawawa");
    
    ofstream myfile;
    myfile.open ("example.txt");
    myfile << "Writing this to a file.\n";
    myfile.close();
 
    cout << str << '\n';*/
    return 0;
}
