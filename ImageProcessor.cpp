#include "ImageProcessor.h"
#include "ImageMatrix.h"
#include "ImageLoader.h"
#include "Convolution.h"
#include "ImageSharpening.h"
#include "EdgeDetector.h"
#include "DecodeMessage.h"
#include "EncodeMessage.h"
#include <iostream>
ImageProcessor::ImageProcessor() {

}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {
    ImageMatrix matrix(img);
    ImageSharpening sharp;
    ImageMatrix sharpMatrix=sharp.sharpen(matrix,2);
    EdgeDetector edge;
    edgePix = edge.detectEdges(sharpMatrix);
    DecodeMessage decode;
    return(decode.decodeFromImage(sharpMatrix,edgePix));
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {
    //Sharpening the image to get the edge pixels:
    
ImageMatrix matrix(img);
    ImageSharpening sharp;
    ImageMatrix sharpMatrix=sharp.sharpen(matrix,2);
    EdgeDetector edge;
    edgePix = edge.detectEdges(sharpMatrix);
    //Encoding the message
    EncodeMessage msg;
    return(msg.encodeMessageToImage(img,message,edgePix));

}
