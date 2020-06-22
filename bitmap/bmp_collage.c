#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main( int argc, char* argv[] ){

  if( argc != 7 ){
    printf( "bmp_collage must be run with parameters that allow two images to be collaged.\n" );
    printf( "Usage: $ %s <background_bmp> <foreground_bmp> <bmp_result> row_offset col_offset scale\n", argv[0] );
    exit(EXIT_FAILURE);
  }
  
  int row_offset, col_offset;
  row_offset = atoi(argv[4]);
  col_offset = atoi(argv[5]);
  float scale;
  scale = atof(argv[6]);  
  
  int ret_code = bmp_collage( argv[1],    argv[2],    argv[3],
                              row_offset, col_offset, scale    );
                                 
  if( ret_code != 0 ){
    printf( "Error: bmp_collage was not successful.\n" );
    return ret_code;
  }

  return 0;
}

