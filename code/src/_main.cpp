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
  string img = "";
  double factor;
  int choice;
  bool quit = false;

  do {
    cout << endl;

    cout << "Choose a solution" << endl;
    cout << "  1 - cut and enlarge" << endl;
    cout << "  2 - transform with power law" << endl;
    cout << "  3 - transform with equalized histogram" << endl;
    cout << "  any - quit" << endl << endl;

    cin >> choice;
    cout << endl;

    switch (choice) {
      case 1:
        cout << "Type in a factor: ";
        cin >> factor;
        cout << endl;
        img = cutAndEnlarge(imgDir + colorGirlImgName, factor);
        if (img == "") {
          break;
        }

        cout << "Type in an odd factor: ";
        cin >> factor;
        cout << endl;
        edgeSmooth(img, factor);
        img = "";
        break;

      case 2:
        cout << "Choose a picture to transform" << endl;
        cout << "  1 - car" << endl;
        cout << "  2 - crowd" << endl;
        cout << "  3 - university" << endl << endl;
        while (img == "") {
          cin >> choice;
          cout << endl;
          switch (choice) {
            case 1:
              img = carImgName;
              break;
            case 2:
              img = crowdImgName;
              break;
            case 3:
              img = universityImgName;
              break;
            default:
              cout << "Choose 1 of the pictures (1, 2, or 3)" << endl << endl;
          }
        }

        cout << "Type in a factor: ";
        cin >> factor;
        cout << endl;

        powerLawTransform(imgDir + img, factor);
        img = "";
        break;

      case 3:
        cout << "Choose a picture to transform" << endl;
        cout << "  1 - car" << endl;
        cout << "  2 - crowd" << endl;
        cout << "  3 - university" << endl << endl;
        while (img == "") {
          cin >> choice;
          cout << endl;
          switch (choice) {
            case 1:
              img = carImgName;
              break;
            case 2:
              img = crowdImgName;
              break;
            case 3:
              img = universityImgName;
              break;
            default:
              cout << "Choose 1 of the pictures (1, 2, or 3)" << endl << endl;
          }
        }

        histogramTransform(imgDir + img);
        img = "";
        break;

      default:
        cout << "Bye!" << endl;
        quit = true;
    }

  } while (!quit);

  return 0;
}
