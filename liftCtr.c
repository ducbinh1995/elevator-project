#include "commonfunction.h"

pid_t *list_pid, lift_sensor_process_id, lift_body_process_id;
int current_floor = 1, request_destination = 1;
int lift_action = STAND;

void up_request(int sigNo){
	switch (sigNo - SIGRTMIN){
		case F1_UP:
		case F1_CALL:
			request_destination = 1;
			printf("Back to floor %d\n", request_destination);
			send_signal(lift_body_process_id, SIGRTMIN + LIFT_DOWN);
			break;
		case F2_UP:
		case F2_CALL:
			request_destination = 2;
			printf("Request to floor %d\n", request_destination);
			send_signal(lift_body_process_id, SIGRTMIN + LIFT_UP);
			break;
		case F3_UP:
		case F3_CALL:
			request_destination = 3;
			printf("Request to floor %d\n", request_destination);
			send_signal(lift_body_process_id, SIGRTMIN + LIFT_UP);
			break;
		case F4_UP:
		case F4_CALL:
			request_destination = 4;
			printf("Request to floor %d\n", request_destination);
			send_signal(lift_body_process_id, SIGRTMIN + LIFT_UP);
			break;
		case F5_UP:
		case F5_CALL:
			request_destination = 5;
			printf("Request to floor %d\n", request_destination);
			send_signal(lift_body_process_id, SIGRTMIN + LIFT_UP);
			break;
		default:
			break;
	}
}

// xu ly ngat khi thang may den 1 tang nao do
void sensor_change(int sigNo){
	send_signal(list_pid[LIFT_MNG], sigNo);
	int floor;
	switch (sigNo - SIGRTMIN){
		case F1_ARRIVAL:
			if (request_destination == 1)
			{
				send_signal(lift_body_process_id, SIGRTMIN + LIFT_STOP);
				send_signal(list_pid[LIFT_MNG], SIGRTMIN + FINISHED);
				printf("Lift finish move\n");
			}
			break;
		case F2_ARRIVAL:
		case F3_ARRIVAL:
		case F4_ARRIVAL:
		case F5_ARRIVAL:
			floor = sigNo - SIGRTMIN - F1_ARRIVAL + 1;
			if (request_destination == floor)
			{
				send_signal(lift_body_process_id, SIGRTMIN + LIFT_STOP);
				send_signal(list_pid[LIFT_MNG], SIGRTMIN + FINISHED);
				sleep(SLEEP_TIME);
				printf("Chuyen hang o tang %d\n", request_destination);
				// dua thang may ve tang 1
				request_destination = 1;
				send_signal(lift_body_process_id, SIGRTMIN + LIFT_DOWN);
			}
			break;
		default:
			break;
	}
}

// chay tien trinh lift sensor check vi tri thang may
void sensor_process_run(){
	// lay id cua tien trinh lift sensor
	int control_process_id = getppid(); 
	int prev_position = F1_HEIGHT;

	while (1){
		usleep(CLOCK);

		if (prev_position != list_pid[LIFT_POSITION]) // vi tri thang may co su thay doi
		{
			switch (list_pid[LIFT_POSITION]){
				// gui tin hieu thang may den cac tang toi lift sensor
				case F1_HEIGHT:
					send_signal(control_process_id, SIGRTMIN + F1_ARRIVAL);
					break;
				case F2_HEIGHT:
					send_signal(control_process_id, SIGRTMIN + F2_ARRIVAL);
					break;
				case F3_HEIGHT:
					send_signal(control_process_id, SIGRTMIN + F3_ARRIVAL);
					break;
				case F4_HEIGHT:
					send_signal(control_process_id, SIGRTMIN + F4_ARRIVAL);
					break;
				case F5_HEIGHT:
					send_signal(control_process_id, SIGRTMIN + F5_ARRIVAL);
					break;
				default:
					break;
			}
			prev_position = list_pid[LIFT_POSITION]; // luu lai vi tri thang may
		}
	}
}

void init_action(int sigNo){
	switch (sigNo - SIGRTMIN){
		case LIFT_UP: 
			lift_action = UP;
			break;
		case LIFT_DOWN: 
			lift_action = DOWN;
			break;
		case LIFT_STOP: 
			lift_action = STAND;
			break;
		default:
			break;
	}
}

void body_process_run(){
	int control_process_id = getppid();
	signal(SIGRTMIN + LIFT_UP, init_action);
	signal(SIGRTMIN + LIFT_DOWN, init_action);
	signal(SIGRTMIN + LIFT_STOP, init_action);

	//khoi tao vi tri ban dau thang may
	list_pid[LIFT_POSITION] = F1_HEIGHT;

	pid_t prev_position = F1_HEIGHT;

	while(1){
		usleep(CLOCK);
		switch(lift_action){
			case UP:
				if (list_pid[LIFT_POSITION] < F5_HEIGHT)
				{
					list_pid[LIFT_POSITION] += SPEED;
				}
				else{
					lift_action = STAND;
				}
				break;
			case DOWN:
				if (list_pid[LIFT_POSITION] > F1_HEIGHT)
				{
					list_pid[LIFT_POSITION] -= SPEED;
				}
				else{
					lift_action = STAND;
				}
				break;
			default:
				break;
		}
		// thong bao do cao hien tai cua lift
		if (prev_position != list_pid[LIFT_POSITION])
		{
			printf("Current lift height: %.1f metters\n", list_pid[LIFT_POSITION] / 10.0);
			prev_position = list_pid[LIFT_POSITION];
		}
	}
}

void quit(){
	send_signal(lift_body_process_id, SIGKILL);
	send_signal(lift_sensor_process_id, SIGKILL);
	exit(1);
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
	signal(SIGRTMIN+F1_ARRIVAL, sensor_change);
	signal(SIGRTMIN+F2_ARRIVAL, sensor_change);
	signal(SIGRTMIN+F3_ARRIVAL, sensor_change);
	signal(SIGRTMIN+F4_ARRIVAL, sensor_change);
	signal(SIGRTMIN+F5_ARRIVAL, sensor_change);
}

int main(int argc, char const *argv[])
{
	init_signal();
	list_pid = create_pid(LIFT_CTR);
	//tao tien trinh con cho lift sensor
	if ((lift_sensor_process_id = fork()) == 0)
	{
		sensor_process_run();
	}
	//tao tien trinh con cho lift body
	else{
		if ((lift_body_process_id = fork()) == 0)
		{
			body_process_run();
		}
		else{
			signal(SIGINT, quit);
			while (1){
				sleep(5);
			}
		}
	}
	return 0;
}

