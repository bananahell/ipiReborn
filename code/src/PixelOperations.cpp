#include "PixelOperations.h"

#include <opencv2/opencv.hpp>
#include <sstream>
#include <vector>

#include "Globals.h"

using namespace std;
using namespace cv;

string cutAndEnlarge(const string& imgNameOrig, int factor) {
  // Cut the image.
  string cutImgName = cutImg(imgNameOrig, factor);

  // Enlarge image if it could cut.
  if (cutImgName == "") {
    cout << ERROR << "Won't enlarge image if can't cut!" << endl;
    return "";
  }

  return enlargeBits(cutImgName, factor);
}

string cutImg(string imgNameOrig, int factor) {
  Mat imgOrig;
  stringstream stringStreamAux;
  string factorString;
  string imgNameCut;
  string preCutImgStr = "Image before cutting";
  string postCutImgStr = "Image after cutting";
  bool writeSuccessful;

  // Read and check image.
  imgOrig = imread(imgNameOrig, IMREAD_COLOR);
  if (!imgOrig.data) {
    cout << ERROR << "Image " << imgNameOrig << " could not be read!" << endl;
    return "";
  }

  // "Cutting" an image means selecting the first pixel in each imaginary square
  // kernel made with a side equal to the factor, so it must be able to divide
  // both height and width of the image.
  // Example: a 400x496 image (colorGirl.jpg) can use factors 2, 4, 8 or 16.
  if (imgOrig.rows % factor != 0 || imgOrig.cols % factor != 0) {
    cout << ERROR
         << "Image cutting factor should be divisible by both the image's "
            "width ("
         << imgOrig.cols << ") and height (" << imgOrig.rows << ")!" << endl;
    imgOrig.release();
    return "";
  }

  // Show original image.
  namedWindow(preCutImgStr, WINDOW_AUTOSIZE);
  imshow(preCutImgStr, imgOrig);
  cout << INFO << preCutImgStr << endl;

  // Create an all black image with the right dimensions (divided by factor).
  Mat imgCut(imgOrig.rows / factor, imgOrig.cols / factor, CV_8UC3,
             Scalar(0, 0, 0));

  // Cut image by getting each pixel in a factor jump.
  for (int j = 0; j < imgOrig.rows; j += factor) {
    for (int i = 0; i < imgOrig.cols; i += factor) {
      imgCut.at<Vec3b>(j / factor, i / factor) = imgOrig.at<Vec3b>(j, i);
    }
  }

  // Show cut image.
  namedWindow(postCutImgStr, WINDOW_AUTOSIZE);
  imshow(postCutImgStr, imgCut);
  cout << INFO << postCutImgStr << endl;

  // Create sstream to put the factor number into the file name.
  stringStreamAux << factor;
  factorString = stringStreamAux.str();

  // Create file with name and directory.
  // Example (file name = name.jpg, factor = 2): "../img/cut/name_cut_2.jpg".
  imgNameCut = imgNameOrig;
  imgNameCut.insert(imgNameCut.find_last_of('/'), "/cut");
  imgNameCut.insert(imgNameCut.find_last_of('.'), "_cut_");
  imgNameCut.insert(imgNameCut.find_last_of('.'), factorString);

  // Attempt to write the new image file with the new name.
  writeSuccessful = imwrite(imgNameCut, imgCut);
  if (writeSuccessful == false) {
    cout << ERROR << "Could not save file " << imgNameCut << "!" << endl;
    destroyAllWindows();
    imgOrig.release();
    imgCut.release();
    return "";
  } else {
    cout << SAVED << postCutImgStr << " -> " << imgNameCut << endl;
  }

  cout << endl;
  waitKey(0);
  destroyAllWindows();
  imgOrig.release();
  imgCut.release();
  return imgNameCut;
}

string enlargeBits(string imgNameOrig, int factor) {
  Mat imgOrig;
  stringstream stringStreamAux;
  string factorString;
  string imgNameLarge;
  string origImgString = "Image before enlarging";
  string largeImgString = "Image after enlarging";
  size_t strIndex;
  bool writeSuccessful;

  // Read and check image.
  imgOrig = imread(imgNameOrig, IMREAD_COLOR);
  if (!imgOrig.data) {
    cout << ERROR << "Image " << imgNameOrig << " could not be read!" << endl;
    return "";
  }

  // Show original image.
  namedWindow(origImgString, WINDOW_AUTOSIZE);
  imshow(origImgString, imgOrig);
  cout << INFO << origImgString << endl;

  // Create an all black image with the right dimensions (multiplied by factor).
  Mat imgLarge(imgOrig.rows * factor, imgOrig.cols * factor, CV_8UC3,
               Scalar(0, 0, 0));

  // "Enlarging" an image is just getting each pixel and repeating it as a
  // square with a side size equal to the factor.
  for (int j = 0; j < imgOrig.rows; j++) {
    for (int i = 0; i < imgOrig.cols; i++) {
      for (int emptyRows = 0; emptyRows < factor; emptyRows++) {
        for (int emptyCols = 0; emptyCols < factor; emptyCols++) {
          imgLarge.at<Vec3b>((j * factor) + emptyRows,
                             (i * factor) + emptyCols) =
              imgOrig.at<Vec3b>(j, i);
        }
      }
    }
  }

  // Show enlarged image.
  namedWindow(largeImgString, WINDOW_AUTOSIZE);
  imshow(largeImgString, imgLarge);
  cout << INFO << largeImgString << endl;

  // Create sstream to put the factor number into the file name.
  stringStreamAux << factor;
  factorString = stringStreamAux.str();

  // Create file with name and directory.
  // Example (file name = nam.jpg, factor = 2): "../img/large/nam_large_2.jpg".
  // Since the enlarging is often used after cutting, this code can replace
  // "/cut" by "/large" or "_cut_" by "_large_" if it's found in the name.
  imgNameLarge = imgNameOrig;
  strIndex = imgNameLarge.find("/cut");
  if (strIndex != string::npos) {
    imgNameLarge.replace(strIndex, 4, "/large");
  } else {
    imgNameLarge.insert(imgNameLarge.find_last_of('/'), "/large");
  }
  strIndex = imgNameLarge.find("_cut_");
  if (strIndex != string::npos) {
    int auxIndex = imgNameLarge.find_last_of('.');
    imgNameLarge.replace(strIndex, auxIndex - strIndex, "_large_");
  } else {
    imgNameLarge.insert(imgNameLarge.find_last_of('.'), "_large_");
  }
  imgNameLarge.insert(imgNameLarge.find_last_of('.'), factorString);

  // Attempt to write the new image file with the new name.
  writeSuccessful = imwrite(imgNameLarge, imgLarge);
  if (writeSuccessful == false) {
    cout << ERROR << "Could not save file " << imgNameLarge << "!" << endl;
    destroyAllWindows();
    imgOrig.release();
    imgLarge.release();
    return "";
  }
  cout << SAVED << largeImgString << " -> " << imgNameLarge << endl;

  cout << endl;
  waitKey(0);
  destroyAllWindows();
  imgOrig.release();
  imgLarge.release();
  return imgNameLarge;
}

string edgeSmooth(string imgNameOrig, int factor) {
  Mat imgOrig;
  stringstream stringStreamAux;
  string factorString;
  string imgNameSmooth;
  string origImgString = "Image before smoothing";
  string smoothImgString = "Image after smoothing";
  size_t strIndex;
  bool writeSuccessful;
  int auxFactor;

  // "Smoothing" an image is just running a square kernel through each pixel of
  // the image and calculating the average of the pixel values inside the
  // square, and since the square needs to envelop each pixel as a middle pixel
  // and a length up, down, right and left all equal, the factor, or the size of
  // the square kernel side, must be an odd number.
  if (factor % 2 == 0) {
    cout << ERROR << "Average filter factor can't be even!" << endl;
    return "";
  }

  // Read and check image.
  imgOrig = imread(imgNameOrig, IMREAD_COLOR);
  if (!imgOrig.data) {
    cout << ERROR << "Image " << imgNameOrig << " could not be read!" << endl;
    return "";
  }

  // Show original image.
  namedWindow(origImgString, WINDOW_AUTOSIZE);
  imshow(origImgString, imgOrig);
  cout << INFO << origImgString << endl;

  // Create an all black image with the right dimensions (same as factor).
  Mat imgSmooth(imgOrig.rows, imgOrig.cols, CV_8UC3, Scalar(0, 0, 0));

  // Run through each pixel in the image.
  for (int row = 0; row < imgOrig.rows; row++) {
    for (int col = 0; col < imgOrig.cols; col++) {
      // Get the distance from the middle pixel to each border of the square
      // kernel made by factor and save it on auxFactor.
      // Example: a factor of 5 has 2 pixels to the left, right, up and down of
      // the middle pixel to the border of the square kernel, so dividing 5 by 2
      // gets this distance 2.
      auxFactor = factor / 2;
      // If the pixel is close to one of the square's sides, decrease the
      // auxFactor to the smallest distance to the square side so it doesn't
      // look for pixels outside of the image itself.
      if (row < auxFactor) {
        auxFactor = row;
      }
      if (col < auxFactor) {
        auxFactor = col;
      }
      if (imgOrig.rows - row - 1 < auxFactor) {
        auxFactor = imgOrig.rows - row - 1;
      }
      if (imgOrig.cols - col - 1 < auxFactor) {
        auxFactor = imgOrig.cols - col - 1;
      }
      // Get each of the RGB channels of the pixel.
      for (int chn = 0; chn < 3; chn++) {
        int pixelNum = 0;
        int pixelTotal = 0;
        // Assign the average of the pixels inside the kernel to the pixel in
        // the same position in the new smooth image.
        for (int i = row - auxFactor; i <= row + auxFactor; i++) {
          for (int j = col - auxFactor; j <= col + auxFactor; j++) {
            pixelNum++;
            pixelTotal += imgOrig.at<Vec3b>(i, j)[chn];
          }
        }
        imgSmooth.at<Vec3b>(row, col)[chn] = (int)(pixelTotal / pixelNum);
      }
    }
  }

  // Show smooth image.
  namedWindow(smoothImgString, WINDOW_AUTOSIZE);
  imshow(smoothImgString, imgSmooth);
  cout << INFO << smoothImgString << endl;

  // Create sstream to put the factor number into the file name.
  stringStreamAux << factor;
  factorString = stringStreamAux.str();

  // Create file with name and directory.
  // Example (file name = na.jpg, factor = 3): "../img/smooth/na_smooth_3.jpg".
  // Since the smoothing is often used after enlarging, this code can replace
  // "/large" by "/smooth" or "_large_" by "_smooth_" if it's found in the name.
  imgNameSmooth = imgNameOrig;
  strIndex = imgNameSmooth.find("/large");
  if (strIndex != string::npos) {
    imgNameSmooth.replace(strIndex, 6, "/smooth");
  } else {
    imgNameSmooth.insert(imgNameSmooth.find_last_of('/'), "/smooth");
  }
  strIndex = imgNameSmooth.find("_large_");
  if (strIndex != string::npos) {
    int auxIndex = imgNameSmooth.find_last_of('.');
    imgNameSmooth.replace(strIndex, auxIndex - strIndex, "_smooth_");
  } else {
    imgNameSmooth.insert(imgNameSmooth.find_last_of('.'), "_smooth_");
  }
  imgNameSmooth.insert(imgNameSmooth.find_last_of('.'), factorString);

  // Attempt to write the new image file with the new name.
  writeSuccessful = imwrite(imgNameSmooth, imgSmooth);
  if (writeSuccessful == false) {
    cout << ERROR << "Could not save file " << imgNameSmooth << "!" << endl;
    destroyAllWindows();
    imgOrig.release();
    imgSmooth.release();
    return "";
  }
  cout << SAVED << smoothImgString << " -> " << imgNameSmooth << endl;

  cout << endl;
  waitKey(0);
  destroyAllWindows();
  imgOrig.release();
  imgSmooth.release();
  return imgNameSmooth;
}

string powerLawTransform(string imgNameOrig, double factor) {
  Mat img;
  stringstream stringStreamAux;
  string factorString;
  string imgNameTrans;
  string origImgString = "Image before power law transforming";
  string transImgString = "Image after power law transforming";
  bool writeSuccessful;
  double corrector = pow(255, factor - 1);

  // Read and check image.
  img = imread(imgNameOrig, IMREAD_GRAYSCALE);
  if (!img.data) {
    cout << ERROR << "Image " << imgNameOrig << " could not be read!" << endl;
    return "";
  }

  // Show original image.
  namedWindow(origImgString, WINDOW_AUTOSIZE);
  imshow(origImgString, img);
  cout << INFO << origImgString << endl;

  // Power law transform the image. It gets the luminosity value of each pixel
  // and raises it to the power of the factor parameter scaled down to max at
  // the maximum luminosity value (512).
  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      img.at<uchar>(i, j) = (int)(pow(img.at<uchar>(i, j), factor) / corrector);
    }
  }

  // Show transformed image.
  namedWindow(transImgString, WINDOW_AUTOSIZE);
  imshow(transImgString, img);
  cout << INFO << transImgString << endl;

  // Create sstream to put the factor number into the file name.
  stringStreamAux << factor;
  factorString = stringStreamAux.str();
  if (factorString.find('.') != string::npos) {
    factorString.replace(factorString.find('.'), 1, ",");
  }

  // Create file with name and directory.
  // Example (file name = name.jpg, factor = 3):
  // "../img/powerLawTrans/name_powerLawTrans_3.jpg".
  imgNameTrans = imgNameOrig;
  imgNameTrans.insert(imgNameTrans.find_last_of('/'), "/powerLawTrans");
  imgNameTrans.insert(imgNameTrans.find_last_of('.'), "_powerLawTrans_");
  imgNameTrans.insert(imgNameTrans.find_last_of('.'), factorString);

  // Attempt to write the new image file with the new name.
  writeSuccessful = imwrite(imgNameTrans, img);
  if (writeSuccessful == false) {
    cout << ERROR << "Could not save file " << imgNameTrans << "!" << endl;
    destroyAllWindows();
    img.release();
    return "";
  }
  cout << SAVED << transImgString << " -> " << imgNameTrans << endl;

  cout << endl;
  waitKey(0);
  destroyAllWindows();
  img.release();
  return imgNameTrans;
}

string histogramTransform(string imgNameOrig) {
  Mat img;
  string imgNameTrans;
  string origImgString = "Image before histogram transforming";
  string transImgString = "Image after histogram transforming";
  string histOrigImgString = "Histogram before transforming";
  string histTransImgString = "Histogram after transforming";
  bool writeSuccessful;
  vector<int> chanAmmountOrig;
  vector<int> chanAmmountTrans;
  vector<double> chanPercentOrig;
  vector<double> chanPercentTrans;
  vector<double> transChannels;
  int pixelsAmmount;
  int normalizedChanAmmount;
  double highestFrequency = 0;

  // Read and check image.
  img = imread(imgNameOrig, IMREAD_GRAYSCALE);
  if (!img.data) {
    cout << ERROR << "Image " << imgNameOrig << " could not be read!" << endl;
    return "";
  }

  // Get total number of pixels
  pixelsAmmount = img.rows * img.cols;

  // Show original image.
  namedWindow(origImgString, WINDOW_AUTOSIZE);
  imshow(origImgString, img);
  cout << INFO << origImgString << endl;

  // Initialize each vector size 256 (luminosity values).
  for (int i = 0; i < 256; i++) {
    chanAmmountOrig.push_back(0);
    chanPercentOrig.push_back(0);
    chanAmmountTrans.push_back(0);
    transChannels.push_back(0);
    chanPercentTrans.push_back(0);
  }

  // Get number of luminosity occurrences in all pixels.
  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      ++chanAmmountOrig.at(img.at<uchar>(i, j));
    }
  }

  // Distributes the pixel luminosities by their percentage of occurrences.
  // The new pixel values add with the last pixel values to accumulate the
  // percentage values, distributing the pixel values throughout the image, but
  // keeping the relative luminosity of each pixel still in reference.
  for (int i = 0; i < 256; i++) {
    chanPercentOrig.at(i) = ((double)chanAmmountOrig.at(i)) / pixelsAmmount;
    transChannels.at(i) = chanPercentOrig.at(i) * 255;
    if (i != 0) {
      transChannels.at(i) += transChannels.at(i - 1);
    }
  }

  // Apply the new luminosity values calculated earlier to the new image.
  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      img.at<uchar>(i, j) = (int)transChannels.at(img.at<uchar>(i, j));
    }
  }

  // Show tranformed image.
  namedWindow(transImgString, WINDOW_AUTOSIZE);
  imshow(transImgString, img);
  cout << INFO << transImgString << endl;

  // Create file with name and directory.
  // Example (file name = name.jpg):
  // "../img/histogramTrans/name_histogramTrans.jpg".
  imgNameTrans = imgNameOrig;
  imgNameTrans.insert(imgNameTrans.find_last_of('/'), "/histogramTrans");
  imgNameTrans.insert(imgNameTrans.find_last_of('.'), "_histogramTrans");

  // Attempt to write the new image file with the new name.
  writeSuccessful = imwrite(imgNameTrans, img);
  if (writeSuccessful == false) {
    cout << ERROR << "Could not save file " << imgNameTrans << "!" << endl;
    destroyAllWindows();
    img.release();
    return "";
  }
  cout << SAVED << transImgString << " -> " << imgNameTrans << endl;

  // Initialize the original histogram image.
  Mat imgHistogramOrig(512, 512, CV_8UC3, Scalar(0, 0, 0));

  // Get highest frequency of the repeating pixel values.
  for (int i = 0; i < 256; i++) {
    if (chanPercentOrig.at(i) > highestFrequency) {
      highestFrequency = chanPercentOrig.at(i);
    }
  }

  // For each pixel value, iterate through image to draw "white bars" with the
  // height of their percentages.
  for (int i = 0; i < 256; i++) {
    normalizedChanAmmount = (chanPercentOrig.at(i) * 512) / highestFrequency;
    // The highest frequency divided by itself always returns 1, so drag it down
    // to 511 to not go out of bounds.
    if (normalizedChanAmmount > 511) {
      normalizedChanAmmount = 511;
    }
    // TODO nao sei por que tenho que fazer 6 vezes... se sao 256 valores, nao
    // devia so botar cada barra 2 vezes para chegar em 512?
    for (int j = 511; j >= 511 - normalizedChanAmmount; j--) {
      imgHistogramOrig.at<uchar>(j, i * 6) = 255;
      imgHistogramOrig.at<uchar>(j, (i * 6) + 1) = 255;
      imgHistogramOrig.at<uchar>(j, (i * 6) + 2) = 255;
      imgHistogramOrig.at<uchar>(j, (i * 6) + 3) = 255;
      imgHistogramOrig.at<uchar>(j, (i * 6) + 4) = 255;
      imgHistogramOrig.at<uchar>(j, (i * 6) + 5) = 255;
    }
  }

  // Show histogram of the original image.
  namedWindow(histOrigImgString, WINDOW_AUTOSIZE);
  imshow(histOrigImgString, imgHistogramOrig);
  cout << INFO << histOrigImgString << endl;

  // Get number of luminosity occurrences in all pixels, just like original.
  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      ++chanAmmountTrans.at(img.at<uchar>(i, j));
    }
  }

  // Distributes the pixel luminosities by their percentage of occurrences.
  for (int i = 0; i < 256; i++) {
    chanPercentTrans.at(i) = ((double)chanAmmountTrans.at(i)) / pixelsAmmount;
  }

  // Initialize the original histogram image.
  Mat imgHistogramTrans(512, 512, CV_8UC3, Scalar(0, 0, 0));

  // Get highest frequency of the repeating pixel values.
  highestFrequency = 0;
  for (int i = 0; i < 256; i++) {
    if (chanPercentTrans.at(i) > highestFrequency) {
      highestFrequency = chanPercentTrans.at(i);
    }
  }

  // For each pixel value, iterate through image to draw "white bars" with the
  // height of their percentages.
  for (int i = 0; i < 256; i++) {
    normalizedChanAmmount = (chanPercentTrans.at(i) * 512) / highestFrequency;
    // The highest frequency divided by itself always returns 1, so drag it down
    // to 511 to not go out of bounds.
    if (normalizedChanAmmount > 511) {
      normalizedChanAmmount = 511;
    }
    // TODO nao sei por que tenho que fazer 6 vezes... se sao 256 valores, nao
    // devia so botar cada barra 2 vezes para chegar em 512?
    for (int j = 511; j >= 511 - normalizedChanAmmount; j--) {
      imgHistogramTrans.at<uchar>(j, i * 6) = 255;
      imgHistogramTrans.at<uchar>(j, (i * 6) + 1) = 255;
      imgHistogramTrans.at<uchar>(j, (i * 6) + 2) = 255;
      imgHistogramTrans.at<uchar>(j, (i * 6) + 3) = 255;
      imgHistogramTrans.at<uchar>(j, (i * 6) + 4) = 255;
      imgHistogramTrans.at<uchar>(j, (i * 6) + 5) = 255;
    }
  }

  // Show histogram of transformed image.
  namedWindow(histTransImgString, WINDOW_AUTOSIZE);
  imshow(histTransImgString, imgHistogramTrans);
  cout << INFO << histTransImgString << endl;

  cout << endl;
  waitKey(0);
  destroyAllWindows();
  img.release();
  imgHistogramOrig.release();
  imgHistogramTrans.release();
  return imgNameTrans;
}
