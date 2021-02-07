//#include <bits/stdc++.h>
#include <string>
#include <iostream>

using namespace std;


/*
 * Complete the 'findSmallestDivisor' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. STRING s
 *  2. STRING t
 */

int findSmallestDivisor(string s, string t) {
    int slen = s.length();
    int tlen = t.length();
    if (slen % tlen != 0) {
        return 1;
    }

    for (int i = 1; i <= tlen; i++) {
        if (slen % i != 0) {
            continue;
        }
        string ttemp = t.substr(0, i);
        bool flag = false;
        for (int j = 0; j < tlen; j += i) {
            if (t.substr(j, i) != ttemp) {
                flag = true;
                break;
            }
        }
        if (flag) {
            continue;
        }
        for (int j = 0; j < slen; j += i) {
            if (s.substr(j, i) != ttemp) {
                flag = true;
                break;
            }
        }
        if (flag) {
            continue;
        }
        return i;
    }

    return 1;

}

int main()
{

    string s;
    cout << "Input S: ";
    cin >> s;

    string t;
    cout << "Input T: ";
    cin >> t;

    int result = findSmallestDivisor(s, t);

    cout << result << "\n";


    return 0;
}
