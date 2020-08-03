#include "withlibjpeg.h"

#include "stddef.h"
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

// #include <cderror.h>
// #include <cdjpeg.h>		/* Common decls for cjpeg/djpeg applications */
// #include <jversion.h>
//unsigned char *imgBuffer[1];

// https://gist.github.com/royshil/fa98604b01787172b270
// https://www.ridgesolutions.ie/index.php/2019/12/10/libjpeg-example-encode-jpeg-to-memory-buffer-instead-of-file/

// extern void my_init_destination (j_compress_ptr cinfo){

//     cinfo->dest->next_output_byte = &imgBuffer[0];
//     cinfo->dest->free_in_buffer = 307200;
// }

// cinfo->dest->init_destination = &my_init_destination;

// void loadRGB()
// {
//     FILE *fp = fopen("RGB", "rb");
//     fread(input_buffer, 234000, 1, fp);
//     fclose(fp);
// }
// void saveRGB()
// {
//     __u_char pixel = 255;

//     __u_char pixel0 = 0;
//     FILE *fp = fopen("RGB", "wb");
//     for (int i = 0; i < IMAGE_HEIGHT; i++)
//     {
//         for (int j = 0; j < IMAGE_WIDTH; j++)
//         {
//             fwrite(&pixel, 1, 1, fp);
//             fwrite(&pixel0, 1, 1, fp);
//             fwrite(&pixel, 1, 1, fp);
//         }
//     }
//     fclose(fp);
// }

void saveJpeg(char*buff, int imgBufferSize)
{
    __u_char pixel = 255;
    FILE *fp = fopen("hell.jpeg", "wb");
    int i = 0;
    fwrite(buff, imgBufferSize, 1, fp);
    fclose(fp);
}

void jpegCompress( char* inputBuffer, char** outImgBuffer, unsigned long * outImgSize, int inputWidth, int inputHeight)
{    
    //300kb = 300 * 1024
    //img size는 왠만하면 300 안넘음
    outImgBuffer[0] = (char*)calloc(300 * 1024, 1);
    *outImgSize = 300 * 1024;
    
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    //((&cinfo)->dest)->init_destination = &my_init_destination;

    cinfo.err = jpeg_std_error(&jerr);
    //jerr.addon_message_table = cdjpeg_message_table;
    // jerr.first_addon_message = JMSG_FIRSTADDONCODE;
    // jerr.last_addon_message = JMSG_LASTADDONCODE;

    /* Initialize JPEG parameters.
   * Much of this may be overridden later.
   * In particular, we don't yet know the input file's color space,
   * but we need to provide some value for jpeg_set_defaults() to work.
   */
    jpeg_create_compress(&cinfo);
    jpeg_mem_dest(&cinfo, (unsigned char**)outImgBuffer, outImgSize);

    /* Step 3: set parameters for compression */
    cinfo.image_width = inputWidth; /* image width and height, in pixels */
    cinfo.image_height = inputHeight;
    cinfo.input_components = 3;     /* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB; /* arbitrary guess */
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 70, TRUE /* limit to baseline-JPEG values */);

    /* Step 4: Start compressor */

    /* TRUE ensures that we will write a complete interchange-JPEG file.
   * Pass TRUE unless you are very sure of what you're doing.
   */
    jpeg_start_compress(&cinfo, TRUE);

    /* Step 5: while (scan lines remain to be written) */
    /*           jpeg_write_scanlines(...); */

    /* Here we use the library's state variable cinfo.next_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   * To keep things simple, we pass one scanline per call; you can pass
   * more if you wish, though.
   */
    unsigned char *row_pointer[1];

    int row_stride = inputWidth * 3; /* JSAMPLEs per row in image_buffer */
    while (cinfo.next_scanline < cinfo.image_height)
    {
        /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
        row_pointer[0] = (unsigned char *)&inputBuffer[cinfo.next_scanline * row_stride];
        (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
  
    // saveJpeg(outImgBuffer[0], *outImgSize);

}