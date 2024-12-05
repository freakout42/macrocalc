#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUF_LEN 1
#define LINE     12

long int filesize( FILE *fp )
  {
    long int save_pos, size_of_file;

    save_pos = ftell( fp );
    fseek( fp, 0L, SEEK_END );
    size_of_file = ftell( fp );
    fseek( fp, save_pos, SEEK_SET );
    return( size_of_file );
  }


void Upper_chars(char *buffer)
{
 unsigned int c;

 for (c=0; c <= strlen(buffer)-1; c++) *(buffer+c)=toupper( *(buffer+c) );
}

int main( argc, argv )
int argc;
char *argv[];
{
    FILE *source,*dest;
    unsigned char buffer[BUF_LEN], Dummy[20];
    int c, l;

    if( (source=fopen( argv[1], "rb" )) == NULL )
    {
      printf("ERROR : I can't find source file   %s\n",argv[1]);
      exit(20L);
    }

    strcpy(Dummy,argv[2]);
    strcat(Dummy,".h");               /* add suffix .h to target name */

    if( (dest=fopen( Dummy, "wb+" )) == NULL )
    {
      printf("ERROR : I can't open destination file   %s\n",Dummy);
      exit(20L);
    }

    strcpy(Dummy,argv[3]);
    Upper_chars(Dummy);    /* lower to upper chars */
    strcat(Dummy,"_LEN");  /* add the suffix _LEN to the struct name */
                           /* for the #define stantment              */


    /* It writes the header information */
    fprintf( dest, "\n#define %s %ld\n\n", Dummy, filesize(source) );
    fprintf( dest, " static unsigned char %s[]=\n {\n  ", argv[3] );

    if( ferror( dest ) )
    {
     printf( "ERROR writing on target file:  %s\n",argv[2] );
     exit(20L);
    }

    c=-1; l=0;
    while( fread( buffer, 1, 1, source ) == 1 )
    {
     if (c != -1) fprintf(dest,",");
     if (++c >= LINE) { fprintf(dest,"\n  "); c=0; l++; }
     fprintf(dest,"0x%02x",*buffer);
    }

    fprintf(dest,"\n};\n\n");
    printf( "done writing %d bytes\n", l*LINE+c+1 );

    fclose(source);
    fclose(dest);

}
