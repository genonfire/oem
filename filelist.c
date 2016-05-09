
#include <stdio.h>

int main()
{
    printf( "Content-type: text/html\n" );
    printf( "\n" );

    system( "ls -lX --ignore=*.c --ignore=*.cgi --ignore=cgi-bin --ignore=wap --ignore=oem > ls.txt" );

    printf( "<html>\n<head>\n<title>redirection..</title>\n" );
    printf( "<meta http-equiv=REFRESH content=0;URL=ls.txt>\n" );
    printf( "</head>\n<body>\ngetting file list..\n</body>\n</html>\n" );

    return 0;
}

