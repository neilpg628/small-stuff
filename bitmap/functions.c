#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap_array.h"

unsigned char*
bmp_open( char* bmp_filename,        unsigned int *width, 
          unsigned int *height,      unsigned int *bits_per_pixel, 
          unsigned int *padding,     unsigned int *data_size, 
          unsigned int *data_offset                                  )
{
  unsigned char *img_data=NULL;

  FILE *bmpfile = fopen(bmp_filename, "rb");
	
  char b, m;
  fread(&b, 1, 1, bmpfile);
  fread(&m, 1, 1, bmpfile);

  if(b != 'B' || m != 'M') {
    printf("%s is not a BMP File!\n", bmp_filename);
    return NULL;
  }

  unsigned int filesize;
  fread(&filesize, 1, sizeof(unsigned int), bmpfile);
  data_size = &filesize;

  printf("File Size: %d\n", *data_size);

  rewind(bmpfile);

	
  img_data = (unsigned char*) malloc(filesize);
	
  if(fread(img_data, 1, filesize, bmpfile) != filesize) {
  	printf("Unable to read the requested %u of bytes!", filesize);
  	return NULL;
  }

  *width = *((unsigned int*)(img_data + 18));
  unsigned int iwidth = *width;
	
  *height = *((unsigned int*)(img_data + 22));
  unsigned int iheight = *height;
	
  printf("Image Width: %u\n", iwidth);
  printf("Image Height: %u\n", iheight);
	
  *bits_per_pixel = *((unsigned short int*)(img_data+28));
  printf("BPP: %d\n", *bits_per_pixel);
	
  *data_offset = *((unsigned int*)(img_data + 10));
  printf("Data Offset: %u\n", *data_offset);
  printf("Header Size: %u\n", *(img_data + 14));
	
  *padding = (4 - ((iwidth * (*bits_per_pixel) / 8) % 4)) % 4;
  printf("Padding: %d\n", *padding);
	
  return img_data;  
}

void 
bmp_close( unsigned char **img_data )
{

  if(img_data != NULL) {
  	free(*img_data);
  	*img_data = NULL;
  }

}

unsigned char***  
bmp_scale( unsigned char*** pixel_array, unsigned char* header_data, unsigned int header_size,
           unsigned int* width, unsigned int* height, unsigned int num_colors,
           float scale )
{
  unsigned char*** new_pixel_array = NULL;

  unsigned int new_width = *width * scale;
  unsigned int new_height = *height * scale;
	
  new_pixel_array = (unsigned char***) malloc( sizeof(unsigned char**) * new_height);
  if( new_pixel_array == NULL ){
    printf( "No space to allocate memory for image of height %d.\n", new_height );
    return NULL;
  }
	
  for( int row = 0; row < (*height) * scale; row++ ){
    new_pixel_array[row] = (unsigned char**) malloc( sizeof(unsigned char*) * new_width);
	if( new_pixel_array[row] == NULL ){
      printf( "No space to allocate memory for image row of width %d.\n", new_width );
      return NULL;
    }
    for( int col=0; col< (*width) * scale; col++ ){
      new_pixel_array[row][col] = (unsigned char*) malloc( sizeof(unsigned char) * num_colors );
	  if( new_pixel_array[row][col] == NULL ){
        printf( "No space to allocate memory for image pixel with %d colors.\n", num_colors );
        return NULL;
      }
	  for( int color = 0; color < num_colors; color++ ){
        new_pixel_array[row][col][color] = pixel_array[(int)(row/scale)][(int)(col/scale)][color];
      }
    }
  }

  *width = new_width;
  *height = new_height;

  unsigned int new_padding = ( 4 - (new_width * num_colors) % 4) % 4;
  unsigned int new_row_width = (new_width*num_colors)+new_padding;
  unsigned int overall_image_size = *(header_data + 14) + new_height*new_row_width;

  memcpy( header_data+2, &overall_image_size, sizeof(unsigned int) );
  memcpy( header_data+18, &new_width, sizeof(unsigned int) );
  memcpy( header_data+22, &new_height, sizeof(unsigned int) );
	

  return new_pixel_array;
}         

int 
bmp_collage( char* background_image_filename,     char* foreground_image_filename, 
             char* output_collage_image_filename, int row_offset,                  
             int col_offset,                      float scale )
{

  unsigned int fheader_size, fwidth, fheight, fpadding, fdata_offset, fnum_colors;
  unsigned char* fheader_data;
  unsigned char ***foreground_array = bmp_to_3D_array(foreground_image_filename, &fheader_data, &fheader_size, &fwidth, &fheight, &fnum_colors);
	
  if(foreground_array == NULL) {
    printf("Unable to read %s!", foreground_image_filename);
    return -1;
  }
	
  unsigned char ***scaled_image = bmp_scale(foreground_array, fheader_data, fheader_size, &fwidth, &fheight, fnum_colors, scale);

  if(scaled_image == NULL) {
    printf("Unable to properly scale %s!", foreground_image_filename);
    return -1;
  }
	
  unsigned int bheader_size, bwidth, bheight, bpadding, bdata_offset, bnum_colors;
  unsigned char* bheader_data;
  unsigned char ***background = bmp_to_3D_array(background_image_filename, &bheader_data, &bheader_size, &bwidth, &bheight, &bnum_colors);
	
  if(background == NULL) {
    printf("Unable to read %s!", background_image_filename);
    return -1;
  }
	
  if(fnum_colors != 4 || bnum_colors != 4) {
    printf("One of the images has an incorrect number of colors!\n");
    return -1;
  }
  
  if(col_offset + scale * fwidth > bwidth ||
	row_offset + scale * fheight > bheight) {
	printf("The scaled foreground image needs to fit in the background!\n");
	return -1;
  }
	
  for( int row = 0; row < bheight; row++ ){
    for( int col=0; col< bwidth; col++ ){
	  for( int color = 0; color < bnum_colors; color++ ){
        if(row >= row_offset &&
		  col >= col_offset &&
		  row < row_offset + fheight &&
		  col < col_offset + fwidth &&
		  scaled_image[row - row_offset][col - col_offset][0] > 0
		) background[row][col][color] = scaled_image[row - row_offset][col - col_offset][color];
      }
    }
  }
  
  bmp_from_3D_array(output_collage_image_filename, bheader_data, bheader_size, background, bwidth, bheight, bnum_colors);
  return 0;
}
