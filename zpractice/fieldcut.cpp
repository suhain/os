#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <iostream>
#include <string>
#define STDIN 0
#define STDOUT 1

void print(int fd, std::string const& str) {
    const char *buffer = str.c_str();
    size_t size = str.size();
    size_t begin = 0;
    while (begin < size) {
        int w = write(fd, buffer + begin, size - begin);
        if (w < 0)
            exit(EXIT_FAILURE);
        begin += w;
    }
}

int main(int argc, char** argv)
{
    std::vector<char*> d;
    std::vector<int> p;
    int idx;
    for (idx = 1; idx < argc; idx++)
    {
        if (strcmp(argv[idx], ";") == 0)
        {
            break;
        }
        d.push_back(argv[idx]);
    }
    idx++;
    for (;idx < argc; idx++)
    {
        p.push_back(atoi(argv[idx]));
    }
    const size_t buffer_size = 4096;
    char* buffer = (char*) malloc(buffer_size);
    std::vector<std::string> s;
    std::vector<size_t> s_len;
    size_t tail = 0;
    while(true)
    {
        bool ok = true;
        int r = read(STDIN, buffer + tail, buffer_size - tail);
        if (r == 0) 
        {
            puts("end of input ...");
            break;
        }
        if (r < 0) 
        {
            puts("error ...");
            return 1;
        }
        tail = r;
        while(true)
        {
            char* word;
            size_t word_len = tail;
            size_t d_len = 0;
            size_t cnt = 0;
            for (size_t i = 0; i != d.size(); i++)
            {
                char* start = strstr(buffer, d[i]);
                if (start == NULL) continue;
                else cnt++;
                if (word_len > start - buffer)
                {
                    d_len = strlen(d[i]);
                    word_len = start - buffer;
                    word = start;
                }
            }
            if (cnt == 0 || word_len == tail) 
            {
                ok = false;
            }
            char* first_word = (char*) malloc(word_len);
            memcpy(first_word, buffer, word_len);
            s.push_back(std::string(first_word, first_word + word_len));
            s_len.push_back(word_len);
            size_t len = word_len + d_len;
            memmove(buffer, buffer + len, tail - len);
            tail -= len;
            if (!ok) break;
        }
    }
    for (size_t i = 0; i < s.size(); i += d.size())
    {
        for (size_t k = 0; k < p.size(); k++)
        {
            size_t index = i + p[k] - 1;
            if (index < s.size())
            {
                print(STDOUT, s[index]);
                print(STDOUT, std::string("\n"));
                //std::cout << s[index] << std::endl;
            }
        }
    }
    free(buffer);
    return 0;
}
