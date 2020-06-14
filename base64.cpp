#include "base64.h"
#include <vector>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <iostream>

namespace base64 {

    char base64Set[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                        'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

    //Function that converts decimal value to binary array
    std::vector<int> decToBin(int dec, int bits) {
        int x = 0;
        std::vector<int> binBuffer(bits);
        std::vector<int> bins(bits);

        for(x = 0; dec > 0; x++) {
            binBuffer[x] = dec%2;
            dec /= 2;
        }

        int dif = bits - x;
        int y = 0;
        for(x = x -1; x>=0; x--, y++) bins[y] = binBuffer[x];

        if(dif != 0) {

            for(int x = bits - 1; x >= dif; x--) bins[x] = bins[x - dif];

            for(int x = 0; x < dif; x++) bins[x] = 0;

        }

        return bins;
    }

    //Function that converts binary value to decimal
    int binToDec(int *bins, int bits) { 
        int dec = 0;
        int exponent = bits - 1;

        for(int i = 0; i < sizeof(bins) && exponent >= 0; i++, exponent--) if(bins[i] == 1) dec += pow(2, exponent);

        return dec;
    }

    //Function that encodes decimal array with base64
    std::string encode(std::vector<unsigned char> decArr) {

        std::string encodedMes;
        
        //Put binary values into one array
        int bins[decArr.size()*8];
        for(int i = 0, y = 0; i < decArr.size(); i++) {

            std::vector<int> tmp = decToBin(decArr[i], 8);
            for(int x : tmp) bins[y] = x, y++;
            
        }

        int remnant = (sizeof(bins) / sizeof(*bins))%6;

        //Check if binary array fits into 6 bits and if needed do padding
        if(remnant != 0) {
            int base64Bins[(sizeof(bins) / sizeof(*bins) - remnant) / 6 + 1][6];

            for(int i = 0; i < sizeof(base64Bins) / sizeof(*base64Bins); i++) for(int ii = 0; ii < 6; ii++) base64Bins[i][ii] = 0;

            for(int i = 0, y = 0; i < sizeof(base64Bins) / sizeof(*base64Bins); i++) {

                for(int ii = 0; ii < 6; ii++, y++){ 

                    if(y < sizeof(bins) / sizeof(*bins)) base64Bins[i][ii] = bins[y]; 
                    else base64Bins[i][ii] = 0;
                }
                encodedMes += base64Set[binToDec(base64Bins[i], 6)];
            }

            if(6 - remnant == 2) encodedMes += "=";
            else if(6- remnant == 4) encodedMes += "==";
        }

        else {
            int base64Bins[(sizeof(bins) / sizeof(*bins) - remnant) / 6][6];

            for(int i = 0; i < sizeof(base64Bins) / sizeof(*base64Bins); i++) for(int ii = 0; ii < 6; ii++) base64Bins[i][ii] = 0;

            for(int i = 0, y = 0; i < sizeof(base64Bins) / sizeof(*base64Bins); i++) {
                for(int ii = 0; ii < 6; ii++, y++) base64Bins[i][ii] = bins[y];
                encodedMes += base64Set[binToDec(base64Bins[i], 6)];
            }
        }

        return encodedMes;
    }

    //Function that decodes the message into decimal values
    std::vector<unsigned char> decode(std::string encodedMessage) {
        
        //Put message into char array
        char chMes[encodedMessage.length()];
        strcpy(chMes, encodedMessage.c_str());


        int binZerosFromEnd = 0;
        int sizeReducer;

        //Check for any paddings
        if(chMes[encodedMessage.length()-1] == '=' && chMes[encodedMessage.length()-2] == '=') binZerosFromEnd = 4, sizeReducer = 2;
        else if(chMes[encodedMessage.length()-1] == '=' && chMes[encodedMessage.length()-2] != '=') binZerosFromEnd = 2, sizeReducer = 1;
        else binZerosFromEnd = 0, sizeReducer = 0;


        std::vector<int> decs(encodedMessage.length() - sizeReducer);
        int lastIndex;

        for(lastIndex = 0; chMes[lastIndex] != '=' && lastIndex < encodedMessage.size() - sizeReducer; lastIndex++) {
            int dec;
            for(dec = 0; base64Set[dec] != chMes[lastIndex]; dec++);
            decs[lastIndex] = dec;
        }
        
        int bins[decs.size()*6 - binZerosFromEnd];
        for(int i = 0; i < sizeof(bins) / sizeof(*bins); i++) bins[i] = 0;

        for(int i = 0, y = 0; i < decs.size(); i++) {
            std::vector<int> bin = decToBin(decs[i], 6);
            for(int ii = 0; ii < 6 && y < sizeof(bins) / sizeof(*bins); ii++) bins[y] = bin[ii], y++;
        }

        std::vector<unsigned char> bitDecs(sizeof(bins) / sizeof(*bins) / 8);
        for(int i = 0, y = 0; i < sizeof(bins) / sizeof(*bins) / 8; i++) {
            int tmp[8];
            for(int ii = 0; ii < 8; ii++, y++) tmp[ii] = bins[y];

            bitDecs[i] = binToDec(tmp, 8);
        }

        return bitDecs;

    }
}