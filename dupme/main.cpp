#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#define STDIN 0
#define STDOUT 1

void _write(char* buffer, int begin, int end)
{
    for (int k = 0; k < 2; ++k)
    {
        while(begin < end)
        {
            int written = write(STDOUT, buffer + begin, end - begin);

            if (written < 0)
            {
                _exit(1);
            }

            begin += written;
        }
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        return 1;
    }

    int k = atoi(argv[1]);

    if (k < 0) 
    {
        return 1;
    }

    char* buffer = static_cast<char*>(malloc(k));

    if (buffer == NULL)
    {
        return 2;
    }

    bool eof_found = false;
    bool ok = true;
    int len = 0;

    while(true)
    {

        int r = read(STDIN, buffer + len, k - len);

        if (r > 0)
        {
            len += r;
        }

        if (r == 0)
        {
            eof_found = true;
            if (len > 0 && ok)
            {
                buffer[len] = '\n';
                r = len + 1;
                len = 0;
            }
        }

        if (r < 0)
        {
            return 2;
        }

        int begin = 0;
        int end;

        for (int i = 0; i < r; i++)
        {
            if (buffer[i] == '\n')
            {
                if (ok)
                {
                    begin = i + 1;
                    ok = true;
                }
                end = i + 1;
                _write(buffer, begin, end);
                begin = i + 1;
            }
        }

        if (eof_found) break;

        len += r - begin;
        memmove(buffer, buffer + begin, len);
        
        if (len == k)
        {
            ok = false;
            len = 0;
        }
    }

    free(buffer);

    return 0;
}
