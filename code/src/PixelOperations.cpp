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

  // Show original image.
  namedWindow(preCutImgStr, WINDOW_AUTOSIZE);
  imshow(preCutImgStr, imgOrig);
  cout << INFO << preCutImgStr << endl;

  // "Cutting" an image means selecting the first pixel in each imaginary square
  // kernel made with a side equal to the factor, so it must be able to divide
  // both height and width of the image.
  // Example: a 400x496 image (colorGirl.jpg) can use factors 2, 4, 8 or 16.
  if (imgOrig.rows % factor != 0 || imgOrig.cols % factor != 0) {
    cout << ERROR
         << "Image cutting factor should be divisible by both the image's "
            "width and height!"
         << endl;
    return "";
  }

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
    return "";
  } else {
    cout << SAVED << postCutImgStr << " -> " << imgNameCut << endl;
  }

  cout << endl;
  waitKey(0);
  destroyAllWindows();
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
    return "";
  }
  cout << SAVED << largeImgString << " -> " << imgNameLarge << endl;

  cout << endl;
  waitKey(0);
  destroyAllWindows();
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
    return "";
  }
  cout << SAVED << smoothImgString << " -> " << imgNameSmooth << endl;

  cout << endl;
  waitKey(0);
  destroyAllWindows();
  return "";
}

string powerLawTransform(string imgNameOrig, int factor) {
  Mat img = imread(imgNameOrig, IMREAD_GRAYSCALE);
  stringstream stringStreamAux;
  string factorString;
  string imgNameTrans;
  string origImgString = "Image before power law transforming";
  string transImgString = "Image after power law transforming";
  bool writeSuccessful;
  double corrector = pow(255, factor - 1);

  if (!img.data) {
    cout << ERROR << "Image " << imgNameOrig << " could not be read!" << endl;
    return "";
  }

  // Show original image.
  namedWindow(origImgString, WINDOW_AUTOSIZE);
  imshow(origImgString, img);
  cout << INFO << origImgString << endl;

  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      img.at<uchar>(i, j) = (int)(pow(img.at<uchar>(i, j), factor) / corrector);
    }
  }

  // Show smooth image.
  namedWindow(transImgString, WINDOW_AUTOSIZE);
  imshow(transImgString, img);
  cout << INFO << transImgString << endl;

  // Create sstream to put the factor number into the file name.
  stringStreamAux << factor;
  factorString = stringStreamAux.str();

  imgNameTrans = imgNameOrig;
  imgNameTrans.insert(imgNameTrans.find_last_of('/'), "/powerLawTrans");
  imgNameTrans.insert(imgNameTrans.find_last_of('.'), "_powerLawTrans_");
  imgNameTrans.insert(imgNameTrans.find_last_of('.'), factorString);

  writeSuccessful = imwrite(imgNameTrans, img);
  if (writeSuccessful == false) {
    cout << ERROR << "Could not save file " << imgNameTrans << "!" << endl;
    return "";
  } else {
    cout << SAVED << transImgString << " -> " << imgNameTrans << endl;
  }

  cout << endl;
  waitKey(0);
  destroyAllWindows();
  return imgNameTrans;
}

string histogramTransform(string imgNameOrig) {
  Mat imgOrig = imread(imgNameOrig, IMREAD_GRAYSCALE);
  string imgNameTrans;
  string origImgString = "Image before histogram transforming";
  string transImgString = "Image after histogram transforming";
  bool writeSuccessful;
  vector<double> pixelsPercent;
  int pixelsAmmount;

  if (!imgOrig.data) {
    cout << ERROR << "Image " << imgNameOrig << " could not be read!" << endl;
    return "";
  }

  pixelsAmmount = imgOrig.rows * imgOrig.cols;

  // Show original image.
  namedWindow(origImgString, WINDOW_AUTOSIZE);
  imshow(origImgString, imgOrig);
  cout << INFO << origImgString << endl;

  for (int i = 0; i < 256; i++) {
    pixelsPercent.push_back(0);
  }

  for (int i = 0; i < imgOrig.rows; i++) {
    for (int j = 0; j < imgOrig.cols; j++) {
      ++pixelsPercent.at(imgOrig.at<uchar>(i, j));
    }
  }

  for (int i = 0; i < 256; i++) {
    pixelsPercent.at(i) = (pixelsPercent.at(i) * 255) / pixelsAmmount;
    if (i != 0) {
      pixelsPercent.at(i) += pixelsPercent.at(i - 1);
    }
  }

  for (int i = 0; i < imgOrig.rows; i++) {
    for (int j = 0; j < imgOrig.cols; j++) {
      imgOrig.at<uchar>(i, j) = (int)pixelsPercent.at(imgOrig.at<uchar>(i, j));
    }
  }

  // Show original image.
  namedWindow(transImgString, WINDOW_AUTOSIZE);
  imshow(transImgString, imgOrig);
  cout << INFO << transImgString << endl;

  imgNameTrans = imgNameOrig;
  imgNameTrans.insert(imgNameTrans.find_last_of('/'), "/histogramTrans");
  imgNameTrans.insert(imgNameTrans.find_last_of('.'), "_histogramTrans");

  writeSuccessful = imwrite(imgNameTrans, imgOrig);
  if (writeSuccessful == false) {
    cout << ERROR << "Could not save file " << imgNameTrans << "!" << endl;
    return "";
  } else {
    cout << SAVED << transImgString << " -> " << imgNameTrans << endl;
  }

  cout << endl;
  waitKey(0);
  destroyAllWindows();
  return imgNameTrans;
}
