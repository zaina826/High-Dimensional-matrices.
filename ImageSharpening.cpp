#include "ImageSharpening.h"
#include "ImageMatrix.h"
#include <iostream>

// Default constructor
ImageSharpening::ImageSharpening() {
    kernel_height = 3;
    kernel_width = 3;
    blurring_kernel = new double*[kernel_height];
    for (int i = 0; i < kernel_height; ++i) {
        blurring_kernel[i] = new double[kernel_width];
        for (int j = 0; j < kernel_width; ++j) {
            blurring_kernel[i][j] = 1.0; 
        }
    }
}

ImageSharpening::~ImageSharpening() {
    for (int i = 0; i < kernel_height; ++i) {
        delete[] blurring_kernel[i];
    }
    delete[] blurring_kernel;
}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    //Turning the blurring kernel into the correct type so we can make a matrix out of it:
    const double** blurring_kernel_matrix = const_cast<const double**>(blurring_kernel);
    ImageMatrix blurring_matrix(blurring_kernel_matrix,kernel_height,kernel_width);
    
    //Now we multiply with 1/9
    blurring_matrix= blurring_matrix*(1.0/9);

    //Now we'll turn the matrix into a kernel (Convolution class):
    double** kernel_data=blurring_matrix.get_data();

    Convolution Kernel(kernel_data,kernel_height,kernel_width,1,true);

    //Convolution:
    ImageMatrix BlurredImg=Kernel.convolve(input_image);

    // //Sharpening:
    ImageMatrix SharpImg=input_image+(input_image-BlurredImg)*k;


    //Clipping:
    for (int i = 0; i < SharpImg.get_height(); ++i) {
        for (int j = 0; j < SharpImg.get_width(); ++j){
            if (SharpImg.get_data(i,j)<0){
                SharpImg.set_data(i,j,0);
            }
            if (SharpImg.get_data(i,j)>255){
                SharpImg.set_data(i,j,255);
            }
        }
    }
    
    return(SharpImg);
}
