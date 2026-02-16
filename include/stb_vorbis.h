// stb_vorbis - OGG Vorbis audio decoder
// Single header library from https://github.com/nothings/stb

#ifndef STB_VORBIS_H
#define STB_VORBIS_H

#ifdef __cplusplus
extern "C" {
#endif

// Decode an entire OGG Vorbis file
// Returns number of samples decoded, or negative on error
int stb_vorbis_decode_filename(const char *filename, int *channels, int *sample_rate, short **output);

// Decode from memory
int stb_vorbis_decode_memory(const unsigned char *mem, int len, int *channels, int *sample_rate, short **output);

#ifdef __cplusplus
}
#endif

#endif // STB_VORBIS_H

// Full implementation available at:
// https://raw.githubusercontent.com/nothings/stb/master/stb_vorbis.c
