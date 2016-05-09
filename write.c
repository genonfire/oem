
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef NULL
#undef NULL
#endif
#define NULL '\0'

#define _ERRSTR "an error occurs while trying to save your source, you gotta try again.."


char x2c( char *x )
{
    char digit;

    digit = ( x[0] >= 'A' ? ( ( x[0] & 0xdf ) - 'A' )+10 : ( x[0] - '0' ) );
    digit *= 16;
    digit += ( x[1] >= 'A' ? ( ( x[1] & 0xdf ) - 'A' )+10 : ( x[1] - '0' ) );

    return( digit );
}

void DecodeENCODEDURL( char *encodedURL )
{
    register int x,y;

    for( x=0, y=0; encodedURL[y]; ++x, ++y ) {
        if( ( encodedURL[x] = encodedURL[y] ) == '%' ) {
            encodedURL[x] = x2c( &encodedURL[y+1] );
            y+=2;
        }
    }

    encodedURL[x] = NULL;
}

void PlusToSpace( char *str )
{
    while( *str ) {
        if( *str == '+' )
            *str = ' ';
        str++;
    }
}

void SpaceToPlus( char *str )
{
    while( *str ) {
        if( *str == ' ' )
            *str = '+';
        str++;
    }
}

void DecodeURL( char *buf )
{
    char* a = buf;

    PlusToSpace( buf );
    buf = a;

    DecodeENCODEDURL( buf );
}

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

    DecodeURL( ContentBuffer );

    return( ContentBuffer + 4 );
}

/* Convert CR(0x0D), LF(0x0A) to <br> */
void Convert_String( char *to, const char *from, int len )
{
    int i =0, j =0;

    while( from[ i ] && j < len ) {
        if( from[ i ] == 0x0D ) {
            if( from[ ++i ] == 0x0A ) {
#if 0
                to[ j ] = '<';
                to[++j] = 'b';
                to[++j] = 'r';
                to[++j] = '>';
#else
                to[ j ] = '\n';
#endif
            }
            else
                to[ j ] = ' ';
        } else
            to[ j ] = from[ i ];

        i++;
        j++;
    }

    to[ j ] = NULL;
}


int main()
{
    char *ContentBuffer;
    char *srcBuffer;
    int n;
    FILE *fp;

    ContentBuffer = GetContent( &n );
    srcBuffer = (char *)malloc(n+1);
    memset(srcBuffer, NULL, n+1);

//    Convert_String( srcBuffer, ContentBuffer );

    printf( "Content-type: text/html\n" );
    printf( "\n" );

    printf( "%s", "Upload success" );

    if( !( fp = fopen( "index.html", "w" ) ) ) {
        printf( "%s", _ERRSTR );
    }
    else {
        Convert_String( srcBuffer, ContentBuffer, n );
        fprintf( fp, "%s", srcBuffer );
        fclose( fp );
    }

    free( ContentBuffer );
    free( srcBuffer );

    return 0;
}

