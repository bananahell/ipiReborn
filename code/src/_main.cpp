/**
 * @brief Program to run the solutions for the image processing class's
 * assignments I had at Universidade de Brasilia, Brasil.
 *
 * The first solution messes up an image by making it edgy, cutting pixels then
 * enlarging them, so that I can try to smooth it with an average kernel filter.
 *
 * @author Pedro Nogueira
 * @since 2022-05-25
 */
#include <iostream>

#include "Globals.h"
#include "PixelOperations.h"

using namespace std;

int main() {
  string imgDir = "../img/";
  string colorGirlImgName = "colorGirl.jpg";
  string carImgName = "car.png";
  string crowdImgName = "crowd.png";
  string universityImgName = "university.png";
  string imgNameEdge2;
  string imgNameEdge4;
  string imgNameEdge8;
  string chosenImg;
  int choice;

  cout << endl;

  cout << "Choose a solution" << endl;
  cout << " 1 - cut and enlarge" << endl;
  cout << " 2 - equalize" << endl << endl;

  cin >> choice;
  cout << endl;

  switch (choice) {
    case 1:
      imgNameEdge2 = cutAndEnlarge(imgDir + colorGirlImgName, 2);
      imgNameEdge4 = cutAndEnlarge(imgDir + colorGirlImgName, 4);
      imgNameEdge8 = cutAndEnlarge(imgDir + colorGirlImgName, 8);

      if (imgNameEdge2 == "" || imgNameEdge4 == "" || imgNameEdge8 == "") {
        cout << ERROR << "Can't continue without all enlarged pictures."
             << endl;
        return 1;
      }

      edgeSmooth(imgNameEdge2, 3);
      edgeSmooth(imgNameEdge4, 5);
      edgeSmooth(imgNameEdge8, 7);
      break;

    case 2:

      cout << "Choose a picture to transform" << endl;
      cout << " 1 - car" << endl;
      cout << " 2 - crowd" << endl;
      cout << " 3 - university" << endl << endl;

      cin >> choice;
      cout << endl;

      switch (choice) {
        case 1:
          chosenImg = carImgName;
          break;
        case 2:
          chosenImg = crowdImgName;
          break;
        case 3:
          chosenImg = universityImgName;
          break;
        default:
          cout << "Inside default" << endl << endl;
          exit(1);
      }

      powerLawTransform(imgDir + chosenImg, 2);
      histogramTransform(imgDir + chosenImg);
      break;

    default:
      cout << "Inside default" << endl << endl;
      exit(1);
  }

  cout << endl;

  return 0;
}
