
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _ROWS 36
#define _COLS 100
#define _MAX_STR 10000


void PlusToSpace( char *str )
{
    while( *str ) {
        if( *str == '+' )
            *str = ' ';
        str++;
    }
}

int main()
{
    char str[ _MAX_STR ];
    int i;
    int ch;
    FILE *stream;

    if( !( stream = fopen( "index.html", "r" ) ) ) {
        strcpy( (char *)str, "NULL" );
    }
    else {
        ch = fgetc( stream );
        for( i=0; (i < _MAX_STR ) && ( feof( stream ) == 0 ); i++ ) {
            str[i] = (char)ch;
            ch = fgetc( stream );
        }
    }

    fclose( stream );

    printf( "Content-type: text/html\n" );
    printf( "\n" );

    printf( "<html>\n<head>\n<title>Edit your page</title>\n<meta charset=\"UTF-8\">\n<meta http-equiv=\"cache-control\" content=\"no-cache\"/>\n</head>\n<body>\n" );

    printf( "<form method=post action=write.cgi>\n" );
    printf( "<textarea name=src rows=%d cols=%d>%s</textarea><br>\n", _ROWS, _COLS, str );
    printf( "<input type=submit value=submit>\n" );
    printf( "<input type=reset value=clear><br>\n</form>\n" );

    printf( "</body></html>\n" );

    return 0;
}

