
#include <stdio.h>

int main()
{
    printf( "Content-type: text/html\n" );
    printf( "\n" );

    system( "cp index.html index-$(date '+%Y%m%d-%H%M').html" );

    printf( "<html>\n<head>\n<title>Backup</title>\n" );
    printf( "</head>\n<body>\nbackup done.\n</body>\n</html>\n" );

    return 0;
}

