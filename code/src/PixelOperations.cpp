#include "PixelOperations.h"

#include <opencv2/opencv.hpp>
#include <sstream>

using namespace std;
using namespace cv;

void enlargeBits(string imgNameOrig, int factor) {
  Mat imgOrig;
  stringstream stringStreamAux;
  string factorString;
  string imgNameSmall;
  string imgNameLarge;
  bool writeSuccessful;

  if (factor % 2 != 0) {
    cout << "Expansion factor should be even. Error." << endl;
    return;
  }

  imgOrig = imread(imgNameOrig, IMREAD_COLOR);

  if (!imgOrig.data) {
    cout << "Image " << imgNameOrig << " could not be read. Error." << endl;
    return;
  }

  namedWindow("Original image", WINDOW_AUTOSIZE);
  imshow("Original image", imgOrig);

  Mat imgSmall(imgOrig.rows / factor, imgOrig.cols / factor, CV_8UC3,
               Scalar(0, 0, 0));

  for (int j = 0; j < imgOrig.rows; j += factor) {
    for (int i = 0; i < imgOrig.cols; i += factor) {
      imgSmall.at<Vec3b>(j / factor, i / factor) = imgOrig.at<Vec3b>(j, i);
    }
  }

  namedWindow("Small image", WINDOW_AUTOSIZE);
  imshow("Small image", imgSmall);
  cout << "Showing small image:" << endl;
  waitKey(0);

  Mat imgLarge(imgSmall.rows * factor, imgSmall.cols * factor, CV_8UC3,
               Scalar(0, 0, 0));

  for (int j = 0; j < imgSmall.rows; j++) {
    for (int i = 0; i < imgSmall.cols; i++) {
      for (int emptyRows = 0; emptyRows < factor; emptyRows++) {
        for (int emptyCols = 0; emptyCols < factor; emptyCols++) {
          imgLarge.at<Vec3b>((j * factor) + emptyRows,
                             (i * factor) + emptyCols) =
              imgSmall.at<Vec3b>(j, i);
        }
      }
    }
  }

  namedWindow("Large image", WINDOW_AUTOSIZE);
  imshow("Large image", imgLarge);
  cout << "Showing large image:" << endl;
  waitKey(0);

  stringStreamAux << factor;
  factorString = stringStreamAux.str();

  imgNameSmall = imgNameOrig;
  imgNameSmall.insert(imgNameSmall.find_last_of('/'), "/small");
  imgNameSmall.insert(imgNameSmall.find_last_of('.'), "_small_");
  imgNameSmall.insert(imgNameSmall.find_last_of('.'), factorString);
  writeSuccessful = imwrite(imgNameSmall, imgSmall);
  if (writeSuccessful == false) {
    cout << "Could not save file " << imgNameSmall << endl;
  } else {
    cout << "Small image successfully saved in " << imgNameSmall << endl;
  }

  imgNameLarge = imgNameOrig;
  imgNameLarge.insert(imgNameLarge.find_last_of('/'), "/large");
  imgNameLarge.insert(imgNameLarge.find_last_of('.'), "_large_");
  imgNameLarge.insert(imgNameLarge.find_last_of('.'), factorString);
  writeSuccessful = imwrite(imgNameLarge, imgLarge);
  if (writeSuccessful == false) {
    cout << "Could not save file " << imgNameLarge << endl;
  } else {
    cout << "Large image successfully saved in " << imgNameLarge << endl;
  }

  destroyAllWindows();
}
