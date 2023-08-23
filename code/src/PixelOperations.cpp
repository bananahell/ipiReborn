#include "PixelOperations.h"

#include <opencv2/opencv.hpp>
#include <sstream>

#include "Globals.h"

using namespace std;
using namespace cv;

string cutAndEnlarge(const string& imgNameOrig, int factor) {
  string cutImgName = cutImg(imgNameOrig, factor);
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

  imgOrig = imread(imgNameOrig, IMREAD_COLOR);
  if (!imgOrig.data) {
    cout << ERROR << "Image " << imgNameOrig << " could not be read!" << endl;
    return "";
  }

  if (imgOrig.rows % factor != 0 || imgOrig.cols % factor != 0) {
    cout << ERROR
         << "Image cutting factor should be divisible by both the image's "
            "width and height!"
         << endl;
    return "";
  }

  namedWindow(preCutImgStr, WINDOW_AUTOSIZE);
  imshow(preCutImgStr, imgOrig);
  cout << INFO << preCutImgStr << endl;

  Mat imgCut(imgOrig.rows / factor, imgOrig.cols / factor, CV_8UC3,
             Scalar(0, 0, 0));

  for (int j = 0; j < imgOrig.rows; j += factor) {
    for (int i = 0; i < imgOrig.cols; i += factor) {
      imgCut.at<Vec3b>(j / factor, i / factor) = imgOrig.at<Vec3b>(j, i);
    }
  }

  namedWindow(postCutImgStr, WINDOW_AUTOSIZE);
  imshow(postCutImgStr, imgCut);
  cout << INFO << postCutImgStr << endl;

  stringStreamAux << factor;
  factorString = stringStreamAux.str();

  imgNameCut = imgNameOrig;
  imgNameCut.insert(imgNameCut.find_last_of('/'), "/cut");
  imgNameCut.insert(imgNameCut.find_last_of('.'), "_cut_");
  imgNameCut.insert(imgNameCut.find_last_of('.'), factorString);
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

  imgOrig = imread(imgNameOrig, IMREAD_COLOR);
  if (!imgOrig.data) {
    cout << ERROR << "Image " << imgNameOrig << " could not be read!" << endl;
    return "";
  }

  namedWindow(origImgString, WINDOW_AUTOSIZE);
  imshow(origImgString, imgOrig);
  cout << INFO << origImgString << endl;

  Mat imgLarge(imgOrig.rows * factor, imgOrig.cols * factor, CV_8UC3,
               Scalar(0, 0, 0));

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

  namedWindow(largeImgString, WINDOW_AUTOSIZE);
  imshow(largeImgString, imgLarge);
  cout << INFO << largeImgString << endl;

  stringStreamAux << factor;
  factorString = stringStreamAux.str();

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

  if (factor % 2 == 0) {
    cout << ERROR << "Average filter factor can't be even!" << endl;
    return "";
  }

  imgOrig = imread(imgNameOrig, IMREAD_COLOR);
  if (!imgOrig.data) {
    cout << ERROR << "Image " << imgNameOrig << " could not be read!" << endl;
    return "";
  }

  Mat imgSmooth(imgOrig.rows, imgOrig.cols, CV_8UC3, Scalar(0, 0, 0));

  namedWindow(origImgString, WINDOW_AUTOSIZE);
  imshow(origImgString, imgOrig);
  cout << INFO << origImgString << endl;

  for (int row = 0; row < imgOrig.rows; row++) {
    for (int col = 0; col < imgOrig.cols; col++) {
      auxFactor = factor / 2;
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
      for (int chn = 0; chn < 3; chn++) {
        int pixelNum = 0;
        int pixelTotal = 0;
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

  namedWindow(smoothImgString, WINDOW_AUTOSIZE);
  imshow(smoothImgString, imgSmooth);
  cout << INFO << smoothImgString << endl;

  stringStreamAux << factor;
  factorString = stringStreamAux.str();

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
