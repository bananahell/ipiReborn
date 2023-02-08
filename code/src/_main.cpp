#include <iostream>

#include "PixelOperations.h"

using namespace std;

int main() {
  std::string imgDir = "../img/";
  std::string colorGirlImgName = "colorGirl.jpg";

  cout << endl;

  enlargeBits(imgDir + colorGirlImgName, 2);

  cout << endl;

  return 0;
}
