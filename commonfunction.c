#include "commonfunction.h"

#define SHIMSIZE 10

// tao ra cac pid trong cac process trong chuong trinh
pid_t* create_pid(int i){
	int shmid;
	pid_t *shm;
	// get memory ID
	if ((shmid = shmget(SHARE_KEY, SHIMSIZE*sizeof(pid_t), IPC_CREAT)) < 0)
	{
		perror("shmget");
		exit(1);
	}
	// attach memory
	if ((shm = (pid_t *)shmat(shmid, 0, 0)) == (int *)-1){
		perror("shmat");
		exit(1);
	}

	shm[i] = getpid();
	return shm;
}

void release_shm(){
	int shmid = shmget(SHARE_KEY, SHIMSIZE*sizeof(pid_t), IPC_CREAT);
	// Delete shared memory
	if (shmctl(shmid, IPC_RMID, NULL) == -1)
	{
		perror("shmctl");
		exit(1);
	}
}

// tao signal dua tren so hieu
int send_signal(pid_t pid,int sigNo){
	if (pid>0){ 
		return (kill(pid, sigNo) == 0);
	}
	else{ 
		return 0;
	}
}