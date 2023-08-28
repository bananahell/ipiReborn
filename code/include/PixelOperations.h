/**
 * @brief Each pixel alterating function I come up with to the project I store
 * here in this file.
 *
 * @author Pedro Nogueira
 */
#ifndef PIXELOPERATIONS
#define PIXELOPERATIONS

#include <string>

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

std::string powerLawTransform(std::string, int);

#endif  // PIXELOPERATIONS
