#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main()
{
    string str = "";
    cout << "Put String In" << endl;
    cin >> str;

    stringstream ss(str);
    string token;

    while(getline(ss, token, ','))
    {
        cout << token << endl;
    }

    return 0;
}
