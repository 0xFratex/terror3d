// stb_image.h - Single header image loading library
// This is a simplified version for the project
// Full version available at: https://github.com/nothings/stb

#ifndef STB_IMAGE_H
#define STB_IMAGE_H

// These are the actual declarations from stb_image.h
// For the full implementation, download from https://github.com/nothings/stb

#define STB_IMAGE_IMPLEMENTATION

#ifdef __cplusplus
extern "C" {
#endif

#ifndef STBI_DEF
#ifdef STB_IMAGE_STATIC
#define STBI_DEF static
#else
#define STBI_DEF extern
#endif
#endif

typedef unsigned char stbi_uc;

STBI_DEF stbi_uc *stbi_load(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels);
STBI_DEF void stbi_image_free(void *retval_from_stbi_load);

#ifdef __cplusplus
}
#endif

#endif // STB_IMAGE_H
