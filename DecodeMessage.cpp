// DecodeMessage.cpp

#include "DecodeMessage.h"
#include <iostream>

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}



std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    //First Getting the pixels:
    int numPixels= edgePixels.size();
    int Pixels[numPixels];
    int counter=0;

    for (const auto& pixel : edgePixels) {
        Pixels[counter]= int(image.get_data(pixel.first,pixel.second));
        counter+=1;
    }

    
    //We will store the binary String here:
    std::string binaryString;
    //If the last digit of a binary number is 1, the number is odd; if it's 0, the number is even.
    for (int i = 0; i < numPixels; ++i) {
        binaryString+=std::to_string(Pixels[i]% 2);
    }

    
    //Making the size divisible by 7, by adding leading zeros
    if (binaryString.size() % 7 !=0){
        int numOfZeros= 7-(binaryString.size() % 7);
        for (int i =0; i < numOfZeros;++i){
            binaryString="0"+binaryString;
        }
    }

    int size=binaryString.size();
    //Making our decimal string 
    int decimal[size/7];
    int count=0;
    for (int i=0;i<size;i+=7){
        //Filling it with an integer for each 7-bit value
        int convertedValue = std::stoi(binaryString.substr(i, 7), nullptr, 2);
        
        // Clipping
        if (convertedValue >= 127) {
            convertedValue = 126;
        } else if (convertedValue <=32) {
            convertedValue += 33;
        }
        decimal[count] = convertedValue;
        count++;
    };

    //Lastly : turning decimal to ASCII
    std::string ascii;
    for (int i =0;i<size/7;i++){
        ascii+= char(decimal[i]);
    };

    return(ascii);
    }

