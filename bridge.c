#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define MEM_MEM 24
#define DEBUG if (0)

#define BREAKER(...)					\
	do {						\
		printf ("Line: %d\n", __LINE__);	\
		getchar ();				\
	} while (0)

#define SEMTOBUF(SEM, OP, FLG)		 		\
	do {						\
		Sems[Sem_ind].sem_num = SEM;		\
		Sems[Sem_ind].sem_op  = OP;		\
		Sems[Sem_ind].sem_flg = FLG;		\
		Sem_ind++;				\
	} while(0) 

#define SEMOP(...)					\
	do {						\
		if (semop (Sem_id, Sems, Sem_ind) == -1)\
		{					\
			perror ("semop");		\
			return -1;			\
		}					\
		Sem_ind = 0;				\
	} while (0)

enum semaphores {
	LOCK = 0,
	N = 1,
	S = 2,
	CNT = 3,
	SEM_NUM = 4
};

enum sharedvar {
	NWAIT = 0,
	SWAIT = 1,
	COUNT = 2,
	DIR = 3,
	NPASS = 4,
	SPASS = 5
};

int Shm_id = 0;
int Sem_id = 0;

int* Buffer = 0;

struct sembuf Sems[SEM_NUM] = {};
short Sem_ind = 0;

int north_car ();
int south_car ();

int main (int argc, char* argv[])
{
	Shm_id = shmget (26, MEM_MEM, IPC_CREAT | 0666);	
	if (Shm_id == -1)
	{
		perror ("shmget");
		exit (-1);
	}

	Buffer = (int*) shmat (Shm_id, NULL, 0);
	if (Buffer == (int*) (void*) -1)
	{
		perror ("shmat");
		exit (-1);
	}

	Sem_id = semget (26, SEM_NUM, IPC_CREAT | 0666);
	if (Sem_id == -1)
	{
		perror ("semget");
		exit (-1);
	}
	
	if (argc != 2)
	{
		errno = EINVAL;
		perror ("main");
	}

	SEMTOBUF(LOCK, 0, IPC_NOWAIT);
	SEMTOBUF(LOCK, 1, 0);
	semop (Sem_id, Sems, Sem_ind);
	Sem_ind = 0;

	if (!strcmp (argv[1], "north")) north_car ();
	else if (!strcmp (argv[1], "south")) south_car ();
	else {
		errno = EINVAL;
		perror ("main");
	}

	return 0;
}



int north_car ()
{
	SEMTOBUF(LOCK, -1, 0); // Ждем, пока значение семафора не будет 1 и уменьшаем, чтоб процессы,
	SEMOP();	       // которые появятся позже ждали

	if (Buffer[SWAIT] || (Buffer[COUNT] && Buffer[DIR] != 1))
	{		// Если есть машины, ждущие въезда на мост с юга или
			//на мосту кто-то есть и едет с юга
		Buffer[NWAIT]++;  // увеличиваем счетчик ждущих въезда с севера
		printf ("waiting\n");
		SEMTOBUF(LOCK, 1, 0); // увеличиваем семафор на 1
		SEMTOBUF(N, -1, 0); // ждем, пока движение с севера будет разблокировано
		SEMOP();
		Buffer[NWAIT]--; //уменьшаем счетчик ждущих въезда с юга		
	}	
	
	if (Buffer[COUNT] == 2)
	{
		SEMTOBUF(CNT, -1, 0);
		SEMOP();
	}

	Buffer[COUNT]++; // увеличиваем количество машин на мосту
	Buffer[DIR] = 1; // говорим, что движение на мосту с севера
	
	if (Buffer[COUNT] == 2)
	{
		SEMTOBUF(CNT, 1, 0);
		SEMOP();
	}

	if (Buffer[NWAIT] && Buffer[SWAIT] == 0) //если есть ждущие с севера и
	{		//нет ждуих с юга
		SEMTOBUF(N, 1, 0); //разрешаем движение с севера
		SEMOP();
	}
	else
	{
		SEMTOBUF(LOCK, 1, 0); // увеличиваем семафор на 1
		SEMOP();
	}

	//проехать мост
	printf ("I'm driving through from north\n");
	fflush (stdout);

	SEMTOBUF(LOCK, -1, 0); // Ждем, пока значение семафора не будет 1 и уменьшаем, чтоб процессы,
	SEMOP();	       // которые появятся позже ждали

	Buffer[COUNT]--;	// уменьшаем количество машин на мосту
	if (Buffer[COUNT] == 1)
	{
		SEMTOBUF(CNT, 1, 0);
		SEMOP();
	}

	if (Buffer[SWAIT] && Buffer[COUNT] == 0) // если есть ждущие с юга и на мосту нет машин
	{
		SEMTOBUF(S, 1, 0); // разрешаем движение с юга
		SEMOP();
	}
	else
	{
		SEMTOBUF(LOCK, 1, 0); // увеличиваем семафор на 1
		SEMOP();
	}
	//Продолжить движение
	printf ("Continue\n");
	fflush (stdout);
}

int south_car ()
{
	SEMTOBUF(LOCK, -1, 0);
	SEMOP();

	if (Buffer[NWAIT] || (Buffer[COUNT] && Buffer[DIR] != 2))
	{
		Buffer[SWAIT]++;
		printf ("waiting\n");
		SEMTOBUF(LOCK, 1, 0);
		SEMTOBUF(S, -1, 0);
		SEMOP();
		Buffer[SWAIT]--;		
	}

	if (Buffer[COUNT] == 2)
	{
		SEMTOBUF(CNT, -1, 0);
		SEMOP();
	}	

	Buffer[COUNT]++;
	Buffer[DIR] = 2;

	if (Buffer[COUNT] == 2)
	{
		SEMTOBUF(CNT, 1, 0);
		SEMOP();
	}

	if (Buffer[SWAIT] && Buffer[NWAIT] == 0)
	{
		SEMTOBUF(S, 1, 0);
		SEMOP();
	}
	else
	{
		SEMTOBUF(LOCK, 1, 0);
		SEMOP();
	}

	//проехать мост
	printf ("I'm driving through from south\n");
	fflush (stdout);

	SEMTOBUF(LOCK, -1, 0);
	SEMOP();

	Buffer[COUNT]--;
	if (Buffer[COUNT] == 1)
	{
		SEMTOBUF(CNT, 1, 0);
		SEMOP();
	}

	if (Buffer[NWAIT] && Buffer[COUNT] == 0)
	{
		SEMTOBUF(N, 1, 0);
		SEMOP();
	}
	else
	{
		SEMTOBUF(LOCK, 1, 0);
		SEMOP();
	}
	//Продолжить движение
	printf ("Continue\n");
	fflush (stdout);
}
