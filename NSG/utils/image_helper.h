/*
    Jonathan Dummer

    Image helper functions

    MIT license
*/

#ifndef HEADER_IMAGE_HELPER
#define HEADER_IMAGE_HELPER

#ifdef __cplusplus
extern "C" {
#endif

/**
	This function upscales an image.
	Not to be used to create MIPmaps,
	but to make it square,
	or to make it a power-of-two sized.
**/
int
up_scale_image
(
    const unsigned char* const orig,
    int width, int height, int channels,
    unsigned char* resampled,
    int resampled_width, int resampled_height
);

#ifdef __cplusplus
}
#endif

#endif /* HEADER_IMAGE_HELPER	*/
