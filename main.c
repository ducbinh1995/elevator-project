#include "commonfunction.h"

int main(int argc, char const *argv[])
{
	pid_t pid;
	if ((pid = fork()) == 0)
	{
		execl("./liftMng", "./liftMng", NULL);
	}
	else if ((pid = fork()) == 0)
	{
		execl("./liftCtr", "./liftCtr", NULL);
	}
	else if ((pid = fork()) == 0)
	{
		execl("./opePanelX", "./opePanelX", "5", NULL);
	}
	else if ((pid = fork()) == 0)
	{
		execl("./opePanelX", "./opePanelX", "4", NULL);
	}
	else if ((pid = fork()) == 0)
	{
		execl("./opePanelX", "./opePanelX", "3", NULL);
	}
	else if ((pid = fork()) == 0)
	{
		execl("./opePanelX", "./opePanelX", "2", NULL);
	}
	else if ((pid = fork()) == 0)
	{
		execl("./opePanel1", "./opePanel1", NULL);
	}
	return 0;
}