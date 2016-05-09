
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef NULL
#undef NULL
#endif
#define NULL '\0'

#ifdef TRUE
#undef TRUE
#endif
#define TRUE 1

#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0

#ifdef bool
#undef bool
#endif
#define bool unsigned char

/* IE6 and IE7 only */
#define _NAMEFIRST '\\'
#define _NAMEEND '\"'

/* to support firefox and IE8 later */
#define _NAMEFIRST_V2 "filename=\""
#define _NAMEEND_V2 '\"'


/* return a pointer of contents */
char *GetContent( int *n )
{
    char *ContentBuffer;
    int i;

    *n = atoi( getenv( "CONTENT_LENGTH" ) );
    ContentBuffer = (char *)malloc( *n+1 );
    for ( i=0; i<*n; i++ )
        ContentBuffer[ i ] = getchar();

    ContentBuffer[ i ] = 0;

    return( ContentBuffer );
}

int main()
{
    char *ContentBuffer;
    unsigned char *ptr;
    char filename[ 256 ];
    char *name_start;
    char *name_end;
    char *name_temp;
    int i, n, len;
    bool bOldIE = FALSE;
    FILE *fp;
    
    ContentBuffer = GetContent( &n );

    printf( "Content-type: text/html\n" );
    printf( "\n" );

    for( i=0; i<n; i++ ) {
        if( ContentBuffer[ i ] == 0x0D && i+4<n ) {
            if( ContentBuffer[ i+1 ] == 0x0A ) {
                if( ContentBuffer[ i+2 ] == 0x0D ) {
                    if( ContentBuffer[ i+3 ] == 0x0A ) {
                        i += 4;
                        ptr = ContentBuffer + i;
                        n -= i;
                        break;
                    }
                }
            }
        }
    }

    if( !( name_temp = strrchr( ContentBuffer, _NAMEFIRST ) ) ) {
	if ( !( name_temp = strstr( ContentBuffer, _NAMEFIRST_V2 ) ) ) {
            printf( "No filename(1)\n" );
            free( ContentBuffer );
            exit( 1 );
	}
	name_start = strchr( name_temp, _NAMEEND );
	name_start++;
    }
    else
    {
	name_start = name_temp;
	bOldIE = TRUE;
    }
    
    if( !( name_end = strchr( name_start, _NAMEEND ) ) ) {
        printf( "No filename(2)\n" );
        free( ContentBuffer );
        exit( 1 );
    }

    memset( (char *)filename, NULL, 256 );
    
    if ( bOldIE )
    {
        len = name_end - name_start - 1;
        strncpy( (char *)filename, ++name_start, len );
    }
    else
    {
	len = name_end - name_start;
	strncpy( (char *)filename, name_start, len );
    }
    
//    printf( "filename(%s), len(%d)\n", filename, len );

    if( !(fp = fopen( filename, "w" ) ) ) {
        printf( "Error : opening filename[%s]", filename );
        free( ContentBuffer );
        exit( 1 );
    }

    for( i=0; i<n; i++ ) {
        fprintf( fp, "%c", ptr[ i ] );

        if( i+5 < n )
            if( ptr[ i+1 ] == 0x0D && ptr[ i+2 ] == 0x0A && ptr[ i+3 ] == 0x2D && ptr[ i+4 ] == 0x2D && ptr[ i+5 ] == 0x2D )
                break;
    }

    printf( "Upload Success" );

    fclose( fp );
    free( ContentBuffer );

    return 0;
}

