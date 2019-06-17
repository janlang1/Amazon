#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <string>
#include <cmath>

using namespace std;

int hashPass(string tempPass);

int main()
{
    cout << "johnvn: " << hashPass("password") << endl;
    cout << "aturing: " << hashPass("graphics") << endl;
    cout << "adal: " << hashPass("piazza") << endl;
    cout << "mzuckerb: " << hashPass("abc123") << endl;
    cout << "ttrojan: " << hashPass("fighton") << endl;
    cout << "billg: " << hashPass("csci104") << endl;

    return 0;
}



int hashPass(string password)
{
    unsigned long long hashCode = 0;
    
    unsigned int encryptedPassword = 0;

    for(size_t i = 0; i < password.length(); ++i){
        hashCode *= 128;
        hashCode += (int)password[i];
    }
    //hashCode += (int)password[password.length() - 1]; //pn

    unsigned int encrypt[4];
    for(int i = 0; i < 4; i++){
        encrypt[i] = hashCode % 65521; 
        hashCode /= 65521;
    }
    //(45912 * w1 + 35511 * w2 + 65169 * w3 + 4625 * w4) % 65521
    encryptedPassword = (45912 * encrypt[3] + 35511 * encrypt[2] + 
                        65169 * encrypt[1] + 4625 * encrypt[0]) % 65521;
    
    return encryptedPassword;
}