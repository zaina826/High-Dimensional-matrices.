
#include "EncodeMessage.h"
#include <cmath>
#include <iostream>
#include "ImageMatrix.h"

// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {
    
}

bool EncodeMessage::isPrime(int n){
    if (n <= 1) {
        return false;
    }

    for (int i = 2; i <= n / 2; i++) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}

int EncodeMessage::Fibonacci(int n){
    if (n == 0 ){
        return(0);
    }else if( n == 1){
        return(1);
    }
    else{
    return Fibonacci(n - 1) + Fibonacci(n - 2);
    };
}
std::string EncodeMessage::shifting(std::string message){
    int n = message.size()/2;
    if(message.size()==0){
        return(message);
    }
    else {
        if (message.size()>=n){
        return(message.substr(message.length() - n) + message.substr(0, message.length() - n));
    }
    else{return(message);}
    }
}


// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {
    
    int i=0;
    std::string newMessage;
    for (char character : message) {
        if (isPrime(i)){
            int decimalValue = int(character);
            decimalValue+=Fibonacci(i);
             if (decimalValue >= 127) {
            decimalValue = 126;
        } else if (decimalValue <=32) {
            decimalValue += 33;
        }
            newMessage+=char(decimalValue);

        }else{
        newMessage+=character;}

        i+=1;
    }

    //Right Circular Shifting
    newMessage=shifting(newMessage);
    std::string binary;

    for (char c : newMessage) {
    for (int i = 0; i <= 6; ++i) {
        if (c & (1 << i)) {
            binary += '1';
        } else {
            binary += '0';
        }
    }
}

    //In case we have more bits to encode than edge pixels (positions) where they should be encoded we need to truncate from the back:

    if (positions.size()<binary.size()){
        //Truncating:
        binary=binary.erase(positions.size());
    }


    int binary_counter=0;
    ImageMatrix img_copy(img);

    for (const auto& position : positions) {
        int row = position.first;  
        int column = position.second; 
        char lsb = binary[binary_counter];

        int decimal=img_copy.get_data(row,column);
        int finalVal=decimal;


        if (decimal%2==0){
            //If it's even and lsb should be changed to 1, that means we should add 1 to the decimal.

            if (lsb=='1'){
                finalVal=(decimal + 1<255)?decimal + 1: 255;
                if (finalVal<0){finalVal=0;};
                img_copy.set_data(row,column,finalVal);
            }
        }
        else{
            if (lsb=='0'){
                finalVal=(decimal - 1> 0)?decimal - 1:0;
                if (finalVal>2550){finalVal=255;};

                img_copy.set_data(row,column,finalVal);
            }
        };
                ++binary_counter;
    }
    
    return img_copy;
}
