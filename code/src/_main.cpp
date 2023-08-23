#include <iostream>

#include "Globals.h"
#include "PixelOperations.h"

using namespace std;

int main() {
  string imgDir = "../img/";
  string colorGirlImgName = "colorGirl.jpg";
  string imgNameEdge2;
  string imgNameEdge4;
  string imgNameEdge8;

  cout << endl;

  imgNameEdge2 = cutAndEnlarge(imgDir + colorGirlImgName, 2);
  imgNameEdge4 = cutAndEnlarge(imgDir + colorGirlImgName, 4);
  imgNameEdge8 = cutAndEnlarge(imgDir + colorGirlImgName, 8);

  if (imgNameEdge2 == "" || imgNameEdge4 == "" || imgNameEdge8 == "") {
    cout << ERROR << "Can't continue without all enlarged pictures." << endl;
    return 1;
  }

  edgeSmooth(imgNameEdge2, 3);
  edgeSmooth(imgNameEdge4, 5);
  edgeSmooth(imgNameEdge8, 7);

  return 0;
}
