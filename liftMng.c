#include "commonfunction.h"

#define MAX_QUEUE 5

pid_t *list_pid;
int lift_is_using = FALSE;
int fifoFd;
int current_floor = 1, current_request;

// request
void up_request(int sigNo){
	if (lift_is_using)
	{
		// thang may dang chay thi dua tin hieu vao hang doi
		write(fifoFd, &sigNo, sizeof(int));
	}
	else{
		lift_is_using = send_signal(list_pid[LIFT_CTR], sigNo);
		current_request = sigNo;
	}
}

// thong bao vi tri lift
void lift_positon(int sigNo){
	pid_t pid;
	if((pid = fork()) == 0){
		send_signal(list_pid[OPE_PANEL1],sigNo);
		send_signal(list_pid[OPE_PANEL2],sigNo);
		send_signal(list_pid[OPE_PANEL3],sigNo);
		send_signal(list_pid[OPE_PANEL4],sigNo);
		send_signal(list_pid[OPE_PANEL5],sigNo);
		exit(1);
	}
}

// xu ly ngat tu liftCtrl
void lift_arrival(int sigNo){
	lift_positon(sigNo);
	int sigNumber; // id request tiep theo
	switch (sigNo - SIGRTMIN){
		case F1_ARRIVAL:
			if (read(fifoFd, &sigNumber, sizeof(int)) > 0)
			{
				sleep(SLEEP_TIME);
				lift_is_using = send_signal(list_pid[LIFT_CTR], sigNumber);
				current_request = sigNumber;
			}
			else{
				lift_is_using = FALSE;
				printf("Lift stop!\n");
			}
			break;
		case F2_ARRIVAL:
		case F3_ARRIVAL:
		case F4_ARRIVAL:
		case F5_ARRIVAL:
			break;
		default:
			lift_is_using = TRUE;
			break;
	}
}

void send_finish_notification(int sigNo){
	switch (current_request - SIGRTMIN){
		case F2_UP:
			send_signal(list_pid[OPE_PANEL1], SIGRTMIN + FINISHED);
			send_signal(list_pid[OPE_PANEL2], SIGRTMIN + USING);
			break;
		case F3_UP:
			send_signal(list_pid[OPE_PANEL1], SIGRTMIN + FINISHED);
			send_signal(list_pid[OPE_PANEL3], SIGRTMIN + USING);
			break;
		case F4_UP:
			send_signal(list_pid[OPE_PANEL1], SIGRTMIN + FINISHED);
			send_signal(list_pid[OPE_PANEL4], SIGRTMIN + USING);
			break;
		case F5_UP:
			send_signal(list_pid[OPE_PANEL1], SIGRTMIN + FINISHED);
			send_signal(list_pid[OPE_PANEL5], SIGRTMIN + USING);
			break;
		case F2_CALL:
			send_signal(list_pid[OPE_PANEL2], SIGRTMIN + FINISHED);
			send_signal(list_pid[OPE_PANEL2], SIGRTMIN + USING);
			break;
		case F3_CALL:
			send_signal(list_pid[OPE_PANEL3], SIGRTMIN + FINISHED);
			send_signal(list_pid[OPE_PANEL3], SIGRTMIN + USING);
			break;
		case F4_CALL:
			send_signal(list_pid[OPE_PANEL4], SIGRTMIN + FINISHED);
			send_signal(list_pid[OPE_PANEL4], SIGRTMIN + USING);
			break;
		case F5_CALL:
			send_signal(list_pid[OPE_PANEL5], SIGRTMIN + FINISHED);
			send_signal(list_pid[OPE_PANEL5], SIGRTMIN + USING);
			break;
		default:
			send_signal(list_pid[OPE_PANEL1], SIGRTMIN + USING);
			break;
	}
}

void init_signal(){
	signal(SIGRTMIN+F2_CALL, up_request);	
	signal(SIGRTMIN+F3_CALL, up_request);	
	signal(SIGRTMIN+F4_CALL, up_request);	
	signal(SIGRTMIN+F5_CALL, up_request);

	signal(SIGRTMIN+F2_UP, up_request);	
	signal(SIGRTMIN+F3_UP, up_request);	
	signal(SIGRTMIN+F4_UP, up_request);	
	signal(SIGRTMIN+F5_UP, up_request);

	signal(SIGRTMIN+F1_ARRIVAL, lift_arrival);
	signal(SIGRTMIN+F2_ARRIVAL, lift_arrival);
	signal(SIGRTMIN+F3_ARRIVAL, lift_arrival);
	signal(SIGRTMIN+F4_ARRIVAL, lift_arrival);
	signal(SIGRTMIN+F5_ARRIVAL, lift_arrival);

	signal(SIGRTMIN+FINISHED, send_finish_notification);

	signal(SIGCHLD, SIG_IGN);
	signal(SIGRTMIN+USING, SIG_IGN);
}

int main(int argc, char const *argv[])
{
	init_signal();
	list_pid = create_pid(LIFT_MNG);
	setpgid(list_pid[LIFT_MNG], 0);

	//Make FIFO file
	remove(MNG_FIFO_FILE_PATH);
	if (mkfifo(MNG_FIFO_FILE_PATH,0666) == -1){
	    perror("mkfifo");
	    exit(1);
	}
	
	if ((fifoFd = open(MNG_FIFO_FILE_PATH,O_RDWR|O_NONBLOCK)) == -1){
	    perror("fifofile open");
	    exit(1);
	} 
			
	while(1){
		sleep(1);					
	}

	close(fifoFd);
	unlink(MNG_FIFO_FILE_PATH);	
	return 0;
}