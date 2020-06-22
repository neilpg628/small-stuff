#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main( int argc, char* argv[] ){

  if( argc < 2 ){
    printf( "Usage: $ %s <bmp_filename>\n", argv[0] );
    exit(EXIT_FAILURE);
  }

  unsigned int image_width, image_height, bits_per_pixel, row_padding, data_size, data_offset;
  unsigned char *img_data = NULL;

  img_data = bmp_open( argv[1], &image_width, &image_height, &bits_per_pixel, &row_padding, &data_size, &data_offset );
  
  if( img_data == NULL ){
    printf( "Error: bmp_info function returned NULL. Cannot report file information.\n" );
    return 0;
  }

  printf( "bmp_info for file %s:\n", argv[1] );
  printf( "  width         = %d\n", image_width );
  printf( "  height        = %d\n", image_height );
  printf( "  bpp           = %d\n", bits_per_pixel );
  printf( "  padding       = %d\n", row_padding );
  printf( "  data_offset   = %d\n", data_offset );
    
  unsigned int middle_row = image_width/2;
  unsigned int middle_col = image_height/2;
  unsigned int num_colors = bits_per_pixel/8;
  unsigned char *pixel_data = img_data + data_offset;
    
  printf( "  The middle pixel has ( row col )=( %d %d ) and\n", middle_row, middle_col  );
  printf( "  color=( " );
  for( int color=0; color<bits_per_pixel/8; color++ )
    printf( "%d ", pixel_data[ middle_row*(image_width*num_colors+row_padding) + middle_col*num_colors + color ] );
  printf( ").\n" );
  
  bmp_close( &img_data );
  
  return 0;
}

