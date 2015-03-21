/*
    Jonathan Dummer

    image helper functions

    MIT license
*/

#include "image_helper.h"
#include <stdlib.h>
#include <math.h>

/*	Upscaling the image uses simple bilinear interpolation	*/
int
up_scale_image
(
    const unsigned char* const orig,
    int width, int height, int channels,
    unsigned char* resampled,
    int resampled_width, int resampled_height
)
{
    float dx, dy;
    int x, y, c;

    /* error(s) check	*/
    if ( 	(width < 1) || (height < 1) ||
            (resampled_width < 2) || (resampled_height < 2) ||
            (channels < 1) ||
            (NULL == orig) || (NULL == resampled) )
    {
        /*	signify badness	*/
        return 0;
    }
    /*
    	for each given pixel in the new map, find the exact location
    	from the original map which would contribute to this guy
    */
    dx = (width - 1.0f) / (resampled_width - 1.0f);
    dy = (height - 1.0f) / (resampled_height - 1.0f);
    for ( y = 0; y < resampled_height; ++y )
    {
        /* find the base y index and fractional offset from that	*/
        float sampley = y * dy;
        int inty = (int)sampley;
        /*	if( inty < 0 ) { inty = 0; } else	*/
        if ( inty > height - 2 ) { inty = height - 2; }
        sampley -= inty;
        for ( x = 0; x < resampled_width; ++x )
        {
            float samplex = x * dx;
            int intx = (int)samplex;
            int base_index;
            /* find the base x index and fractional offset from that	*/
            /*	if( intx < 0 ) { intx = 0; } else	*/
            if ( intx > width - 2 ) { intx = width - 2; }
            samplex -= intx;
            /*	base index into the original image	*/
            base_index = (inty * width + intx) * channels;
            for ( c = 0; c < channels; ++c )
            {
                /*	do the sampling	*/
                float value = 0.5f;
                value += orig[base_index]
                         * (1.0f - samplex) * (1.0f - sampley);
                value += orig[base_index + channels]
                         * (samplex) * (1.0f - sampley);
                value += orig[base_index + width * channels]
                         * (1.0f - samplex) * (sampley);
                value += orig[base_index + width * channels + channels]
                         * (samplex) * (sampley);
                /*	move to the next channel	*/
                ++base_index;
                /*	save the new value	*/
                resampled[y * resampled_width * channels + x * channels + c] =
                    (unsigned char)(value);
            }
        }
    }
    /*	done	*/
    return 1;
}
