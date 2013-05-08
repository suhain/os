#include <unistd.h>

int main()
{
	setpgid(0, 0);
	sleep(100);
	pid_t pid = fork();
	if (pid)
	{
		sleep(10);
	}
	else 
	{
		setsid();
		sleep(100);
	}
}
