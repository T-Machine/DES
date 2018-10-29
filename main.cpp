#include <string.h>
#include <iostream>
#include"des.h"
using namespace std;

void strToBit(string str, bool bit[64]) {
    int count = 0, index = 0;
    while (1) {
        if (count == 64)
            break;
        if (str[index] == ' ') {
            index ++;
            continue;
        }
        bit[count] = bool(str[index] - '0');
        count ++;
        index ++;
    }
}

int main() {
    string keyStr;
    bool   keyBit[64];
    string inputStr;
    string encryptStr;
    string decryptStr;

    keyStr = "11110010 10010110 01001000 11000100 00111000 00110000 00111000 11111111";
    strToBit(keyStr, keyBit);

    cout << "please input plain text: ";
    cin >> inputStr;

    encryptStr = DES::DES_encrypt(inputStr, keyBit);
    decryptStr = DES::DES_decrypt(encryptStr, keyBit);

    cout << "input:   " << inputStr << endl;
    cout << "encrypt: ";
    for(int i = 0; i < encryptStr.length(); i ++) {
        int ch = (int)encryptStr[i];
        if(ch < 0) ch += 256;
        printf("%X", ch);
    }
    cout << endl;
    cout << "decrypt: " << decryptStr << endl;

    return 0;
}