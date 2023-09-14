/**
 * @brief Each pixel alterating function I come up with to the project I store
 * here in this file.
 *
 * @author Pedro Nogueira
 */
#ifndef PIXELOPERATIONS
#define PIXELOPERATIONS

#include <string>
#include <vector>

/**
 * @brief Does cutImg and enlargeBits with the same factor in order.
 * @param imgNameOrig Directory and name of the image file.
 * @param factor Number by which to cut intercalating pixels and enlarge image,
 * must divide width and height.
 * @return Name of the cut and enlarged image, empty string if failure.
 */
std::string cutAndEnlarge(const std::string&, int);

/**
 * @brief Removes intercalating pixels of images by factor.
 * @param imgNameOrig Directory and name of the image file.
 * @param factor Number by which to cut intercalating pixels of image, must
 * divide width and height.
 * @return Name of the cut image, empty string if failure.
 */
std::string cutImg(std::string, int);

/**
 * @brief Copies each bit by factor given.
 * @param imgNameOrig Directory and name of the image file.
 * @param factor Number by which to enlarge image, without restrictions.
 * @return Name of the enlarged image, empty string if failure.
 */
std::string enlargeBits(std::string, int);

/**
 * @brief Runs square kernel through each pixel to take average of pixels around
 * it and replace it, smoothing edgy parts of image.
 * @param imgNameOrig Directory and name of the image file.
 * @param factor Size of side of square kernel. Bigger squares make smoother but
 * blurrier images.
 * @return Name of the smoothed image, empty string if failure.
 */
std::string edgeSmooth(std::string, int);

/**
 * @brief Transforms the pixels in an image raising the pixels' values to the
 * factor equalized.
 * @param imgNameOrig Directory and name of the image file.
 * @param factor Exponent to raise the pixels to the power of.
 * @return Name of the transformed image, empty string if failure.
 */
std::string powerLawTransform(std::string, double);

/**
 * @brief Tranforms the pixels in an image equalizing the pixels by their
 * percentage of occurrences in the image.
 * @param imgNameOrig Directory and name of the image file.
 * @return Name of the transformed image, empty string if failure.
 */
std::string histogramTransform(std::string);

/**
 * @brief Shows the histogram of the pixel values in a vector.
 * @param chanPercent The percentages of each pixel value occurrence.
 * @param highestFrequency The highest percentage in the vector.
 * @param histImgString The name of the histogram image to be shown.
 */
void showHistogram(std::vector<double>, double, std::string);

/**
 * @brief Shows the histogram of the pixel values in a vector.
 * @param chanAmmount The ammount of each pixel value occurrence.
 * @param highestFrequency The highest ammount in the vector.
 * @param histImgString The name of the histogram image to be shown.
 */
void showHistogram(std::vector<int>, double, std::string);

#endif  // PIXELOPERATIONS
