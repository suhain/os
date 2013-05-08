#include <stdio.h>
#include <fcntl.h>

int main()
{
	pid_t pid = fork();
	if (pid)
	{
		sleep(100);
	}
	else 
	{
		int slave, master;
	}
}
