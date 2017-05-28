#ifndef _COMMONFUNCTION_H_
#define _COMMONFUNCTION_H_
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/fcntl.h>

pid_t* create_pid(int i);
void release_shm();
int send_signal(pid_t pid,int sigNo);
// process signal
enum 
{
	LIFT_MNG,
	LIFT_CTR,
	OPE_PANEL1,
	OPE_PANEL2,
	OPE_PANEL3,
	OPE_PANEL4,
	OPE_PANEL5,
	LIFT_POSITION
};

enum 
{
	FALSE = 0,
	TRUE
};

// LIFT DIRECTION
#define DOWN -1
#define STAND 0
#define UP 1

#define CLOCK 500000

// shared memory key
#define SHARE_KEY 2468

// signal lift den tang nao do
#define F1_ARRIVAL 1
#define F2_ARRIVAL 2
#define F3_ARRIVAL 3
#define F4_ARRIVAL 4
#define F5_ARRIVAL 5

// signal chieu thang may
#define LIFT_UP 6
#define LIFT_DOWN 7
#define LIFT_STOP 8

// signal lift status
#define USING 9
#define FINISHED 10

// signal call
#define F1_CALL 11
#define F2_CALL 12
#define F3_CALL 13
#define F4_CALL 14
#define F5_CALL 15

// signal call from tang 1
#define F1_UP 16
#define F2_UP 17
#define F3_UP 18
#define F4_UP 19
#define F5_UP 20

// time chuyen hang
#define SLEEP_TIME 3

// queue process file path
#define MNG_FIFO_FILE_PATH "processqueue"
#define OPE1_FIFO_FILE_PATH "opepanel1queue"

// Notification
#define CURRENT_FLOOR1 "Thang may dang o tang 1"
#define CURRENT_FLOOR2 "Thang may dang o tang 2"
#define CURRENT_FLOOR3 "Thang may dang o tang 3"
#define CURRENT_FLOOR4 "Thang may dang o tang 4"
#define CURRENT_FLOOR5 "Thang may dang o tang 5"

#define BUFF_SIZE  256

#define F1_HEIGHT 15
#define F2_HEIGHT 45
#define F3_HEIGHT 75
#define F4_HEIGHT 105
#define F5_HEIGHT 135

#define SPEED 5

#endif