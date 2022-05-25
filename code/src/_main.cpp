#include "PixelOperations.h"

int main() {
  std::string imgDir = "../img/";
  std::string colorGirlImgName = "colorGirl.jpg";

  enlargeBits(imgDir + colorGirlImgName, 2);

  return 0;
}
