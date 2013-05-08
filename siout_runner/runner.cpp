#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <deque>

void run_command(std::deque <char*> & v)
{
	char ** command = (char **) malloc((v.size() + 1) * (sizeof(char *)));
	for (int i = 0; i < v.size(); i++)
	{
		command[i] = &v[i][0];
	}
	command[v.size()] = NULL;
	int pid = fork();
	if (pid)
	{
		pid_t tpid;
		int status;
		do
		{
			tpid = wait(&status);
		}
		while (tpid != pid);
			free(command);
		exit(0);
	}
	else
	{
		if (execvp(&v[0][0], command) < 0)
		{
			exit(1);
		}
	}
}

void run(std::deque<std::deque<char*> > & v)
{
	if (v.size() < 1)
    {
        exit(1);
    }
    if (v.size() == 1)
    {
        run_command(v[0]);
    }
    else
    {
        int pipefd[2];
        pipe(pipefd);

        if (fork())
        {
            dup2(pipefd[0], 0);
            close(pipefd[0]);
            close(pipefd[1]);
            v.pop_front();
            run(v);
        }
        else
        {
            dup2(pipefd[1], 1);
            close(pipefd[0]);
            close(pipefd[1]);
            run_command(v[0]);
        }
    }
}

void _split(char* buf, size_t length, std::deque<std::deque<char*> > & s, char separator) 
{
	std::deque<char*> t;
	bool prev = false;
	for (size_t i = 0, beg = 0, end = 0; i != length; i++)
	{
		if (prev && buf[i] == separator)
		{
			s.push_back(t);
			t.clear();
			prev = false;
			beg = i + 1;
			continue;
		}
		prev = false;
		if (buf[i] == separator)
		{
			end = i;
			char* tmp = (char*) malloc(end - beg + 1);
		    memcpy(tmp, buf + beg, end - beg + 1);	
			t.push_back(tmp);
			beg = i + 1;
			prev = true;
		}
	}
}

size_t _read(int fd, char * buffer, size_t size)
{
	size_t current = 0;
	while (current < size)
    	{
			size_t result = read(fd, buffer + current, size);
	        if (result < 1)
	        {
	            return current;
	        }
	        current += result;
	        size -= current;
    	}
    	return current;
}

int main(int argc, char ** argv)
{
	int fds = open(argv[1], O_RDONLY);
	if (fds < 0)   exit(1);
	char buffer[1024];
	size_t len = _read(fds, buffer, 1024);
	std::deque<std::deque<char*> > command;
	_split(buffer, len, command, '\0');
	run(command);
    return 0;
}
