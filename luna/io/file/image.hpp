#pragma once
#include <vector>

namespace luna {
/** Template structure containing information and data about an image.
 */
template <typename Type>
struct ImageInfo {
  std::vector<Type> pixels;  ///< The raw pixel data of the image.
  int width;                 ///< The width of the image.
  int height;                ///< The height of the image.
  int channels;  ///< The amount of channels ( i.e. RGBA=4 vs RGB=3 ) of each
                 ///< pixel.

  auto texInfo() -> ImageInfo;
};

/** Function to load the image at the specified path.
 * @param path Path to image to load.
 * @return A Structure containing information about this image.
 */
auto loadRGBA32F(const char* path) -> ImageInfo<float>;

/** Function to load the image at the specified path.
 * @param path Path to image to load.
 * @return A Structure containing information about this image.
 */
auto loadRGB32F(const char* path) -> ImageInfo<float>;

/** Function to load the image at the specified path.
 * @param path Path to image to load.
 * @return A Structure containing information about this image.
 */
auto loadRG32F(const char* path) -> ImageInfo<float>;

/** Function to load the image at the specified path.
 * @param path Path to image to load.
 * @return A Structure containing information about this image.
 */
auto loadR32F(const char* path) -> ImageInfo<float>;

/** Function to load the image at the specified path.
 * @param path Path to image to load.
 * @return A Structure containing information about this image.
 */
auto loadRGBA8(const char* path) -> ImageInfo<unsigned char>;

/** Function to load the image at the specified path.
 * @param path Path to image to load.
 * @return A Structure containing information about this image.
 */
auto loadRGB8(const char* path) -> ImageInfo<unsigned char>;

/** Function to load the image at the specified path.
 * @param path Path to image to load.
 * @return A Structure containing information about this image.
 */
auto loadRG8(const char* path) -> ImageInfo<unsigned char>;

/** Function to load the image at the specified path.
 * @param path Path to image to load.
 * @return A Structure containing information about this image.
 */
auto loadR8(const char* path) -> ImageInfo<unsigned char>;

/** Function to load the image at the specified path.
 * @param path Path to image to load.
 * @return A Structure containing information about this image.
 */
auto loadRGBA32F(unsigned num_bytes, const unsigned char* bytes)
    -> ImageInfo<float>;

/** Function to load the image at the specified path.
 * @param path Path to image to load.
 * @return A Structure containing information about this image.
 */
auto loadRGB32F(unsigned num_bytes, const unsigned char* bytes)
    -> ImageInfo<float>;

/** Function to load the image at the specified path.
 * @param path Path to image to load.
 * @return A Structure containing information about this image.
 */
auto loadRG32F(unsigned num_bytes, const unsigned char* bytes)
    -> ImageInfo<float>;

/** Function to load the image at the specified path.
 * @param path Path to image to load.
 * @return A Structure containing information about this image.
 */
auto loadR32F(unsigned num_bytes, const unsigned char* bytes)
    -> ImageInfo<float>;

/** Function to load the image at the specified path.
 * @param path Path to image to load.
 * @return A Structure containing information about this image.
 */
auto loadRGBA8(unsigned num_bytes, const unsigned char* bytes)
    -> ImageInfo<unsigned char>;

/** Function to load the image at the specified path.
 * @param path Path to image to load.
 * @return A Structure containing information about this image.
 */
auto loadRGB8(unsigned num_bytes, const unsigned char* bytes)
    -> ImageInfo<unsigned char>;

/** Function to load the image at the specified path.
 * @param path Path to image to load.
 * @return A Structure containing information about this image.
 */
auto loadRG8(unsigned num_bytes, const unsigned char* bytes)
    -> ImageInfo<unsigned char>;

/** Function to load the image at the specified path.
 * @param path Path to image to load.
 * @return A Structure containing information about this image.
 */
auto loadR8(unsigned num_bytes, const unsigned char* bytes)
    -> ImageInfo<unsigned char>;

/** Function to save an image to the filesystem.
 * @param info The structure containing information about the image to save.
 * @param path The path to the desired output image.
 */
auto savePNG(const ImageInfo<float>& info, const char* path) -> void;

/** Function to save an image to the filesystem.
 * @param info The structure containing information about the image to save.
 * @param path The path to the desired output image.
 */
auto savePNG(const ImageInfo<unsigned char>& info, const char* path) -> void;


}  // namespace luna