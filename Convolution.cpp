#include <iostream>

#include "Convolution.h"
#include "ImageMatrix.h"
// Default constructor 
Convolution::Convolution() {
    //I'm assuming these should be default values:
    kernel_matrix= nullptr;
    height = 0;
    width = 0;
    stride = 0;
    padding = false;
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad){
    //We will dynamically allocate a kernel according to the parameters given;
    height = kh;
    width = kw;
    kernel_matrix=new double*[height];
    for (int i = 0; i < height; ++i) {
        kernel_matrix[i] = new double[width];
        for (int j = 0; j < width; ++j) {
            kernel_matrix[i][j] = customKernel[i][j];

        }
    }
    stride= stride_val;
    padding =pad;
}



// Destructor
Convolution::~Convolution() {
    //Here we will delete allocated memory for the kernel;
    if (kernel_matrix!=nullptr){
        for (int i = 0; i < height; ++i){
            delete[] kernel_matrix[i];
        }
        delete[] kernel_matrix;
    }
}

// Copy constructor
Convolution::Convolution(const Convolution &other){
    height = other.height;
    width = other.width;
    stride=other.stride;
    padding=other.padding;
    kernel_matrix=new double*[height];
    for (int i = 0; i < height; ++i) {
        kernel_matrix[i] = new double[width];
        for (int j = 0; j < width; ++j) {
            kernel_matrix[i][j] = other.kernel_matrix[i][j];
        }
    }
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    if (this == &other) {
        return *this; 
    }

    if (kernel_matrix != nullptr) {
        for (int i = 0; i < height; ++i) {
            delete[] kernel_matrix[i];
        }
        delete[] kernel_matrix;
    }

    height= other.height;
    width=other.width;
    stride=other.stride;
    padding=other.padding;
    
    kernel_matrix = new double*[height];
    for (int i = 0; i < height; ++i) {
        kernel_matrix[i] = new double[width];
        for (int j = 0; j < width; ++j) {
            kernel_matrix[i][j] = other.kernel_matrix[i][j];
        }
    }
    return *this;
}


// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {
    int new_height=input_image.get_height()+2;
    int new_width=input_image.get_width()+2;
    ImageMatrix preconvo(new_height,new_width);
    if (padding){
        for (int i = 1; i < new_height-1; ++i) {
            for (int j = 1; j < new_width-1; ++j) {
            preconvo.set_data(i,j,input_image.get_data(i-1,j-1));
        }
    }}
    //If it is ready for convolotion, (no padding) then:

    else{preconvo=input_image;}
    
    //Now we'll start with the convolotion process:
    int padding_int = (padding) ? 1 : 0;
    int result_height=((input_image.get_height()-height+2*padding_int)/stride)+1;
    int result_width=((input_image.get_width()-width+2*padding_int)/stride)+1;

    //Creating the resulting matrix:
    ImageMatrix result_matrix(result_height,result_width);
    for (int i = 0; i < result_height; i++) {
            for (int j = 0; j < result_width; j++) {
                double cell=0;
                int image_i, image_j,kernel_i,kernel_j;
                image_i=i*stride;
                image_j=j*stride;
                for (int kernel_i = 0; kernel_i < height; ++kernel_i) {
                    for (int kernel_j = 0; kernel_j < width; ++kernel_j){
                        cell+= kernel_matrix[kernel_i][kernel_j]*preconvo.get_data(image_i,image_j);
                        image_j+=1;
                    };
                    image_j=j*stride;
                   image_i+=1;
                };
                result_matrix.set_data(i,j,cell);
        }
    }
    return (result_matrix);

}
