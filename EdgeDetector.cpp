// EdgeDetector.cpp
#include <iostream>

#include "EdgeDetector.h"
#include <cmath>


// Default constructor
EdgeDetector::EdgeDetector() {
    height=width=3;
    Gx = new double*[height];
    Gx[0]=new double[width]{-1, 0,1};
    Gx[1]=new double[width]{-2, 0,2};
    Gx[2]=new double[width]{-1, 0,1};

    Gy = new double*[height];
    Gy[0]=new double[width]{-1, -2,-1};
    Gy[1]=new double[width]{0, 0,0};
    Gy[2]=new double[width]{1, 2,1};
}

//Destructor
EdgeDetector::~EdgeDetector() {
       for (int i = 0; i < height; ++i) {
            delete[] Gx[i];
            delete[] Gy[i];
    }
    delete[] Gx;
    delete[] Gy;
}

//Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {
    Convolution Gx_kernel(Gx,height,width,1,true);
    Convolution Gy_kernel(Gy,height,width,1,true);

    //Convolution
    ImageMatrix Ix= Gx_kernel.convolve(input_image);
    ImageMatrix Iy= Gy_kernel.convolve(input_image);


    //Getting the magnitude, and putting it in a matrix (while also getting the average)
    int input_image_height=input_image.get_height();
    int input_image_width=input_image.get_width();
    ImageMatrix MagnitudeMatrix(input_image_height,input_image_width);
    double summation=0;
    for (int i = 0; i < input_image_height; ++i) {
        for (int j = 0; j < input_image_width; ++j) {
            double Gradient=std::sqrt((Ix.get_data(i,j)*Ix.get_data(i,j))+(Iy.get_data(i,j)*Iy.get_data(i,j)));
            MagnitudeMatrix.set_data(i,j,Gradient);
            summation=summation+Gradient;
        }
    };
    //Getting the threshold
    double average= summation/(input_image_width*input_image_height);

    //Making the vector:
    std::vector<std::pair<int, int>> EdgePixels;

   
   //Looping over the graient matrix to find the ones over the threshold, those will be placed in the vector:
    for (int i = 0; i < input_image_height; ++i) {
        for (int j = 0; j < input_image_width; ++j) {
            if (MagnitudeMatrix.get_data(i,j)>average){
                //Pushing the pair to edge Pixels and returning them:
                EdgePixels.push_back(std::make_pair(i,j));
            };
        }
    };

    return EdgePixels;


}

