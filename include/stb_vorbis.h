// stb_vorbis.h wrapper
// stb_vorbis is distributed as a single .c file that acts as both header and implementation
// Include the .c file here to get declarations

#ifndef STB_VORBIS_H_WRAPPER
#define STB_VORBIS_H_WRAPPER

// Define this to get only declarations (header mode)
// The implementation will be provided by stb_vorbis_impl.cpp
#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

#endif // STB_VORBIS_H_WRAPPER
