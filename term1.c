#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define MAX 8

int process_num;
int RR_time;

typedef struct {
	int PID;
	int CPU_burst;
	int IO_burst;
	int Arrival;
	int Priority;
	int IO_timing;
} process;

process info[MAX];
process copy[MAX];

void Create_Process() {


	char choice;

	printf("How many processes do you want to make? (2~%d) : ", MAX);
	scanf("%d", &process_num);
	printf("How many time quantum do you want to set in RR? : ");
	scanf("%d", &RR_time);


	printf("Do you want random-creation? Y/N : ");
	do { scanf("%c", &choice); } while (choice == '\n' || choice == ' ');

	if (choice == 'Y' || choice == 'y') {
		srand(time(NULL));
		for (int i = 0; i < process_num; i++) {
			info[i].PID = i + 1;
			info[i].CPU_burst = 1 + rand() % 15;
			info[i].IO_burst = rand() % 11;
			info[i].Arrival = rand() % 26;
			info[i].Priority = rand() % 10 + 1;
			info[i].IO_timing = info[i].Arrival + rand() % 16; //IO_timing은 도착 시간보단 길게

		}

	}

	else if (choice == 'N' || choice == 'n') {

		for (int i = 0; i < process_num; i++) {
			printf("PID : ");
			scanf("%d", &info[i].PID);
			printf("CPU_Burst_Time : ");
			scanf("%d", &info[i].CPU_burst);
			printf("IO_Burst_Time : ");
			scanf("%d", &info[i].IO_burst);
			printf("Arrival_Time : ");
			scanf("%d", &info[i].Arrival);
			printf("Priority : ");
			scanf("%d", &info[i].Priority);
			printf("IO_timing : ");
			scanf("%d", &info[i].IO_timing);
			printf("\n");
			// 여기서 printf("\n"); 이 있으면 밑에 출력이 안되고 없으면 출력이 된다... 왜그러지?


		}




	}
	else {
		return;
	}



	for (int i = 0; i < process_num; i++) {
		printf("<Process %d>\n", info[i].PID);
		printf("CPU_Burst_Time : %d\n", info[i].CPU_burst);
		printf("IO_Burst_Time : %d\n", info[i].IO_burst);
		printf("Arrival_Time : %d\n", info[i].Arrival);
		printf("Priority : %d\n", info[i].Priority);
		printf("IO_timing : %d\n", info[i].IO_timing);
		printf("\n");

	}

}

void Copy() {
	for (int i = 0; i < process_num; i++) {
		copy[i].PID = info[i].PID;
		copy[i].CPU_burst = info[i].CPU_burst;
		copy[i].IO_burst = info[i].IO_burst;
		copy[i].Arrival = info[i].Arrival;
		copy[i].Priority = info[i].Priority;
		copy[i].IO_timing = info[i].IO_timing;
	}
}

void Gantt(int num, int p[], int p_t[]) {
	/*printf("%d\t%d\n", start, end);
	printf("| \t | \r\n | P % d | \r\n | \t |",index);*/
	printf("|");
	for (int i = 0; i < num; i++) {
		printf("\t|");
	}
	printf("\n|");
	for (int i = 0; i < num; i++) {
		if (p[i] == -1) printf(" IDLE  |");
		else printf("   P%d  |", p[i]);
		//printf(" index%d  |", p[i]);
	}
	printf("\n|");
	for (int i = 0; i < num; i++) {
		printf("\t|");
	}
	printf("\n0");
	for (int i = 0; i < num; i++) {
		printf("\t%d", p_t[i]);
	}
	printf("\n");
}



void FCFS(process copy[]) {
	int time = 0;
	int finished = 0;
	int print = 0;
	int process[100] = { 0 };
	int process_time[100] = { 0 };
	int finished_time[8] = { 0 }; //원래 arrival과 끝나는 시간 차이 = turnaround time
	//int p_waiting[8] = { 0 }; arrival과 끝나는 시간 차이인데 끝나는 시간이 IO_timing보다 뒤라면 IO 거쳤으니까 turnaround - IO_burst
	int IDLE_time = 0;
	//생각할 것
	Copy();
	while (finished < process_num) {
		int index = -1;

		for (int i = 0; i < process_num; i++) {
			if ((copy[i].Arrival > -1) && (time >= copy[i].IO_timing)) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //이거 IO_timing 같고 끝나는 시간 빠른 애들이 먼저 처리되도록 추가
			} //Arrival에 IO 반영
		}

		for (int i = 0; i < process_num; i++) {
			if (copy[i].Arrival > -1) {
				index = i;
				break;
			} // 이미 다 끝난 놈 빼고 제일 적은놈부터 시작
		}
		for (int i = 0; i < process_num; i++) {
			if ((copy[i].Arrival > -1) && (copy[index].Arrival > copy[i].Arrival)) {
				index = i;
			}

			//printf("P%d time: %d IO_t: %d IO_b : %d\n", index + 1, time, copy[i].IO_timing, copy[i].IO_burst);
			//printf("Arrival[%d] : %d index : %d finished : %d p[%d]cpburst : %d\n", i, copy[i].Arrival, index, finished, i ,copy[i].CPU_burst);
			//printf("IO_b : %d IO_t : %d \n", copy[i].IO_burst, copy[i].IO_timing);
			//printf("P%d time: %d IO_t: %d IO_b : %d\n", index + 1, time, copy[i].IO_timing, copy[i].IO_burst);


		}//제일 빠른 도착시간 인덱스 찾기
		if (time < copy[index].Arrival) {
			IDLE_time += copy[index].Arrival - time;
			//printf("%d idle %d\n",time,copy[index].Arrival); //체크용
			time = copy[index].Arrival;

			//printf("real minus 1 대입\n");
			if ((print != 0) && (process[print - 1] == -1)) {
				process_time[print - 1] = time;
				finished_time[index] = time;

			}
			else {
				process[print] = -1;
				process_time[print] = time;
				finished_time[index] = time;
				//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
				print += 1;
			}



			//자 여기 남은 시간 어케 쓸거야 ex 현재시간 15, 도착시간 20 남은 5초
		}



		/*
		if (time < copy[index].Arrival) {
			//printf("%d idle %d\n",time,copy[index].Arrival); //체크용
			IDLE_time+= copy[index].Arrival - time;
			time = copy[index].Arrival;

			//printf("real minus 1 대입\n");
			process[print] = -1;
			process_time[print] = time;
			//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
			print += 1;

			//자 여기 남은 시간 어케 쓸거야 ex 현재시간 15, 도착시간 20 남은 5초
		}*/




		//지금 시간이 도착시간보다 더 크면? 지금시간은 25 // 20 > 15                    15                  9                          20
		//근데 여기서 문제가 되는게 현재 시간보다 IO_timing이 더 작아서 문제군
		else {

			//            36        34				34			14						36								10
			if ((copy[index].IO_timing > time) && ((time + copy[index].CPU_burst) > (copy[index].IO_timing)) && (copy[index].IO_burst != 0)) {

				int temp = (copy[index].IO_timing) - time;
				//int temp = (time + copy[index].CPU_burst) - (copy[index].IO_timing);
				copy[index].Arrival = copy[index].IO_timing + copy[index].IO_burst;
				copy[index].CPU_burst -= temp;
				//copy[index].IO_burst = 0;
				time += temp;

				//printf("check\n");
				//printf("P%d  %d\n", index+1, time);

				process[print] = copy[index].PID;
				process_time[print] = time;
				//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
				print += 1;




			}
			else {
				if ((time >= copy[index].IO_timing) && (time < copy[index].IO_timing + copy[index].IO_burst)) {
					copy[index].Arrival = copy[index].IO_timing + copy[index].IO_burst;
					copy[index].IO_burst = 0;
				}
				else {
					time += copy[index].CPU_burst;
					copy[index].Arrival = -1;
					copy[index].CPU_burst = 0;
					finished += 1;
					//printf("here ");
					//printf("P%d  %d\n", index+1, time);
					process[print] = copy[index].PID;
					process_time[print] = time;
					finished_time[index] = time;
					//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
					print += 1;
				}
			}


		}




	}
	printf("<FCFS>\n");
	Gantt(print, process, process_time);
	printf("Average Turnaround Time : ( ");
	int sum = 0;
	printf("%d ", finished_time[0] - info[0].Arrival);
	sum += finished_time[0] - info[0].Arrival;
	for (int i = 1; i < process_num; i++) {
		printf("+ %d ", finished_time[i] - info[i].Arrival);
		sum += finished_time[i] - info[i].Arrival;
	}
	printf(") / %d = %d\n", process_num, sum / process_num);


	printf("Average Waiting Time : ( ");
	sum = 0;
	if (finished_time[0] > info[0].IO_timing) sum += finished_time[0] - info[0].Arrival - info[0].IO_burst - info[0].CPU_burst;
	else sum += finished_time[0] - info[0].Arrival - info[0].CPU_burst;
	printf("%d", sum);
	for (int i = 1; i < process_num; i++) {
		if (finished_time[i] > info[i].IO_timing) {
			sum += finished_time[i] - info[i].Arrival - info[i].IO_burst - info[i].CPU_burst;
			printf("+ %d ", finished_time[i] - info[i].Arrival - info[i].IO_burst - info[i].CPU_burst);

		}
		else {
			sum += finished_time[i] - info[i].Arrival - info[i].CPU_burst;
			printf("+ %d ", finished_time[i] - info[i].Arrival - info[i].CPU_burst);
		}
	}
	printf(") / %d = %d\n", process_num, sum / process_num);
	float Utilization = ((float)(time - IDLE_time)) / (float)time;
	printf("CPU Utilization : (%d/%d)*100 = %.1f%%\n", time - IDLE_time, time, (Utilization * 100));


}
//waiting time, turnaround time,


void SJF(process copy[]) {
	int time = 0;
	int finished = 0;
	int print = 0;
	int process[100] = { 0 };
	int process_time[100] = { 0 };
	int finished_time[8] = { 0 }; //원래 arrival과 끝나는 시간 차이 = turnaround time
	int IDLE_time = 0;
	//shortest job first
	Copy();
	while (finished < process_num) {
		int index = -1;
		for (int i = 0; i < process_num; i++) {
			if ((copy[i].Arrival > -1) && (time >= copy[i].IO_timing)) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //이거 IO_timing 같고 끝나는 시간 빠른 애들이 먼저 처리되도록 추가
			} //Arrival에 IO 반영
		}


		for (int i = 0; i < process_num; i++) {
			if (copy[i].Arrival > -1) {
				index = i;
				break;
			}
		}

		//여기도 Arrival 제일 적은거 넣을거 필요?

		for (int i = 0; i < process_num; i++) {

			if ((copy[i].Arrival > -1) && (copy[index].Arrival >= copy[i].Arrival)) {



				if (copy[index].Arrival == copy[i].Arrival) {

					if (copy[index].CPU_burst > copy[i].CPU_burst) {
						index = i; //아니면 그대로
					}
				}
				else {
					//copy[i]가 더 빨리 도착한다면
					if (copy[index].Arrival <= time) {
						//둘다 시간 안에 이미 도착
						if (copy[index].CPU_burst >= copy[i].CPU_burst) {
							index = i; //i가 더 작으면 인덱스로 지정
						}
					}

					else index = i;
				}

			}

			else if ((copy[i].Arrival > -1) && (copy[index].Arrival < copy[i].Arrival)) {
				if (time >= copy[i].Arrival) {
					if (copy[index].CPU_burst > copy[i].CPU_burst) {
						index = i;
					}
				}


			}





			// 이미 도착한 놈들 중에서 CPU_burst가 가장 작은 놈을 찾아서 index로 지정
			//printf("P%d time: %d IO_t: %d IO_b : %d\n", i+1, time, copy[i].IO_timing, copy[i].IO_burst);
			//printf("[p%d]Arrival : %d, index : %d\n", i+1, copy[i].Arrival,index);
			if ((copy[i].Arrival > -1) && (time >= (copy[i].IO_timing + copy[i].IO_burst))) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //이거 IO_timing 같고 끝나는 시간 빠른 애들이 먼저 처리되도록 추가
			}

		}

		if (time < copy[index].Arrival) {
			IDLE_time += copy[index].Arrival - time;
			//printf("%d idle %d\n",time,copy[index].Arrival); //체크용
			time = copy[index].Arrival;

			//printf("real minus 1 대입\n");
			if ((print != 0) && (process[print - 1] == -1)) {
				process_time[print - 1] = time;
				finished_time[index] = time;

			}
			else {
				process[print] = -1;
				process_time[print] = time;
				finished_time[index] = time;
				//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
				print += 1;
			}



			//자 여기 남은 시간 어케 쓸거야 ex 현재시간 15, 도착시간 20 남은 5초
		}

		/*
		if (time < copy[index].Arrival) {
			//printf("%d idle %d\n",time,copy[index].Arrival); //체크용
			IDLE_time += copy[index].Arrival - time;
			time = copy[index].Arrival;

			//printf("real minus 1 대입\n");
			process[print] = -1;
			process_time[print] = time;
			//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
			print += 1;

			//자 여기 남은 시간 어케 쓸거야 ex 현재시간 15, 도착시간 20 남은 5초
		}*/






		//근데 여기서 문제가 되는게 현재 시간보다 IO_timing이 더 작아서 문제군
		else {
			//            36        34				34			14						36								10
			if ((copy[index].IO_timing > time) && ((time + copy[index].CPU_burst) > (copy[index].IO_timing)) && (copy[index].IO_burst != 0)) {

				int temp = (copy[index].IO_timing) - time;
				//int temp = (time + copy[index].CPU_burst) - (copy[index].IO_timing);
				copy[index].Arrival = copy[index].IO_timing + copy[index].IO_burst;
				copy[index].CPU_burst -= temp;
				//copy[index].IO_burst = 0;
				time += temp;

				//printf("check\n");
				//printf("P%d  %d\n", index+1, time);

				process[print] = copy[index].PID;
				process_time[print] = time;
				finished_time[index] = time;
				//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
				print += 1;




			}
			else {
				if ((time >= copy[index].IO_timing) && (time < copy[index].IO_timing + copy[index].IO_burst)) {
					copy[index].Arrival = copy[index].IO_timing + copy[index].IO_burst;
					copy[index].IO_burst = 0;
				}
				else {
					time += copy[index].CPU_burst;
					copy[index].Arrival = -1;
					copy[index].CPU_burst = 0;
					finished += 1;
					//printf("here ");
					//printf("P%d  %d\n", index+1, time);
					process[print] = copy[index].PID;
					process_time[print] = time;
					finished_time[index] = time;
					//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
					print += 1;
				}
			}


		}




	}
	printf("<SJF>\n");
	Gantt(print, process, process_time);
	printf("Average Turnaround Time : ( ");
	int sum = 0;
	printf("%d ", finished_time[0] - info[0].Arrival);
	sum += finished_time[0] - info[0].Arrival;
	for (int i = 1; i < process_num; i++) {
		printf("+ %d ", finished_time[i] - info[i].Arrival);
		sum += finished_time[i] - info[i].Arrival;
	}
	printf(") / %d = %d\n", process_num, sum / process_num);


	printf("Average Waiting Time : ( ");
	sum = 0;
	if (finished_time[0] > info[0].IO_timing) sum += finished_time[0] - info[0].Arrival - info[0].IO_burst - info[0].CPU_burst;
	else sum += finished_time[0] - info[0].Arrival - info[0].CPU_burst;
	printf("%d", sum);
	for (int i = 1; i < process_num; i++) {
		if (finished_time[i] > info[i].IO_timing) {
			sum += finished_time[i] - info[i].Arrival - info[i].IO_burst - info[i].CPU_burst;
			printf("+ %d ", finished_time[i] - info[i].Arrival - info[i].IO_burst - info[i].CPU_burst);

		}
		else {
			sum += finished_time[i] - info[i].Arrival - info[i].CPU_burst;
			printf("+ %d ", finished_time[i] - info[i].Arrival - info[i].CPU_burst);
		}
	}
	printf(") / %d = %d\n", process_num, sum / process_num);
	float Utilization = ((float)(time - IDLE_time)) / (float)time;
	printf("CPU Utilization : (%d/%d)*100 = %.1f%%\n", time - IDLE_time, time, (Utilization * 100));
}



void P_SJF(process copy[]) {

	int time = 0;
	int finished = 0;
	int print = 0;
	int process[100] = { 0 };
	int process_time[100] = { 0 };
	int IO_issue = 0;
	int current_time = 0; //preemptive 될 때 burst 감소시킬 때 쓸 변수
	int min = 0;
	int next_index = 0;
	int before_index = 0;
	int before_time = 0;
	int finished_time[8] = { 0 }; //원래 arrival과 끝나는 시간 차이 = turnaround time
	int IDLE_time = 0;
	Copy();
	int index = -2; // preemption이라 밖에 놔두었음.

	while (finished < process_num) {

		int next = 0;

		for (int i = 0; i < process_num; i++) {
			if ((copy[i].Arrival > -1) && (time >= copy[i].IO_timing)) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //이거 IO_timing 같고 끝나는 시간 빠른 애들이 먼저 처리되도록 추가
			} //Arrival에 IO 반영
		}


		for (int i = 0; i < process_num; i++) {

			if (copy[i].Arrival > -1) {
				min = copy[i].Arrival;
				break;
			}

		}

		for (int i = 0; i < process_num; i++) {
			if (copy[i].Arrival > -1) {
				if (min >= copy[i].Arrival) {
					min = copy[i].Arrival;
					next = i;
					if (copy[i].Arrival > time) next = -2;
				}
			}
		}


		// 이미 도착한 놈 빼고 제일 적은놈부터 시작 index 찾기
		//if( finished == process_num) break;

		for (int i = 0; i < process_num; i++) {



			// 당장 도착하는 것들 중에서 고르기 시작해야지
			if ((copy[i].Arrival > -1) && (time >= copy[i].Arrival)) {

				if (copy[next].CPU_burst > copy[i].CPU_burst) {

					next = i;

				}
			}

			// 이미 도착한 놈들 중에서 CPU_burst가 가장 작은 놈을 찾아서 index로 지정
			//printf("P%d time: %d IO_t: %d IO_b : %d\n", index + 1, time, copy[i].IO_timing, copy[i].IO_burst);
			//printf("next : %d min : %d Arrival[%d] : %d index : %d finished : %d p[%d]cpburst : %d\n", next, min, i, copy[i].Arrival, index, finished, i ,copy[i].CPU_burst);
			//printf("IO_b : %d IO_t : %d IO_issue : %d\n", copy[i].IO_burst, copy[i].IO_timing,IO_issue);
			if ((copy[i].Arrival > -1) && (time >= (copy[i].IO_timing + copy[i].IO_burst))) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //이거 IO_timing 같고 끝나는 시간 빠른 애들이 먼저 처리되도록 추가
			} // 얘는 나중에 지워도 될듯?
		}
		if ((index != -2) && (index != next) && (IO_issue == 0)) {
			process[print] = copy[index].PID;
			//printf("추가 완료1!!!!!!!\n");
			process_time[print] = time;
			finished_time[index] = time;
			//copy[index].CPU_burst -= current_time;
			//current_time = 0;
			print += 1;
		}
		if ((index != -2) && (index != next) && (IO_issue == 1)) {
			process[print] = copy[index].PID;
			//printf("추가 완료2!!!!!!!\n");
			process_time[print] = time - 1;
			finished_time[index] = time - 1;
			//copy[index].CPU_burst -= current_time;
			//current_time = 0;
			print += 1;
		}

		index = next;



		if (time < min) {

			//printf("%d idle %d\n",time,copy[index].Arrival); //체크용
			IDLE_time += min - time;
			time += min - time;
			//printf("real minus 1 대입\n");
			index = -2;
			process[print] = -1;
			//printf("추가 완료3!!!!!!!\n");
			process_time[print] = time;
			//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
			print += 1;
			continue;

		}//idle 한번에

		else {

			//            35         35            34         14                  36                        10
			if ((copy[index].IO_timing > time) && ((time + copy[index].CPU_burst) > (copy[index].IO_timing)) && (copy[index].IO_burst != 0)) {

				copy[index].CPU_burst -= 1;
				if (copy[index].CPU_burst == 0) {
					copy[index].Arrival = -1;
					finished += 1;

				}

				time++;
				//current_time ++;
				IO_issue = 0;
			}
			else {
				if (IO_issue == 1) {
					copy[index].CPU_burst -= 1;
					if (copy[index].CPU_burst == 0) {
						copy[index].Arrival = -1;
						finished += 1;

					}
					IO_issue = 0;
				}

				if ((time >= copy[index].IO_timing) && (time < copy[index].Arrival)) {

					if (copy[index].IO_burst != 0) time++;
					IO_issue = 1;
				}


				else {
					copy[index].CPU_burst -= 1;
					time++;
					if (copy[index].CPU_burst == 0) {
						copy[index].Arrival = -1;
						finished += 1;
					}
				}

			}

			if (finished == process_num) {
				process[print] = copy[index].PID;
				process_time[print] = time;
				finished_time[index] = time;
				print += 1;
			}
		}
	}
	printf("<P_SJF>\n");
	Gantt(print, process, process_time);
	printf("Average Turnaround Time : ( ");
	int sum = 0;
	printf("%d ", finished_time[0] - info[0].Arrival);
	sum += finished_time[0] - info[0].Arrival;
	for (int i = 1; i < process_num; i++) {
		printf("+ %d ", finished_time[i] - info[i].Arrival);
		sum += finished_time[i] - info[i].Arrival;
	}
	printf(") / %d = %d\n", process_num, sum / process_num);


	printf("Average Waiting Time : ( ");
	sum = 0;
	if (finished_time[0] > info[0].IO_timing) sum += finished_time[0] - info[0].Arrival - info[0].IO_burst - info[0].CPU_burst;
	else sum += finished_time[0] - info[0].Arrival - info[0].CPU_burst;
	printf("%d", sum);
	for (int i = 1; i < process_num; i++) {
		if (finished_time[i] > info[i].IO_timing) {
			sum += finished_time[i] - info[i].Arrival - info[i].IO_burst - info[i].CPU_burst;
			printf("+ %d ", finished_time[i] - info[i].Arrival - info[i].IO_burst - info[i].CPU_burst);

		}
		else {
			sum += finished_time[i] - info[i].Arrival - info[i].CPU_burst;
			printf("+ %d ", finished_time[i] - info[i].Arrival - info[i].CPU_burst);
		}
	}
	printf(") / %d = %d\n", process_num, sum / process_num);
	float Utilization = ((float)(time - IDLE_time)) / (float)time;
	printf("CPU Utilization : (%d/%d)*100 = %.1f%%\n", time - IDLE_time, time, (Utilization * 100));
}


void Priority(process copy[]) {
	int time = 0;
	int finished = 0;
	int print = 0;
	int process[100] = { 0 };
	int process_time[100] = { 0 };
	int finished_time[8] = { 0 }; //원래 arrival과 끝나는 시간 차이 = turnaround time
	int IDLE_time = 0;
	//Priority 큰 거 먼저, 기본 틀은 SJF 들고 왔으니까 CPU_burst 짧은거 대신 Priority 큰거로 바꾸면 됨
	Copy();
	while (finished < process_num) {
		int index = -1;
		for (int i = 0; i < process_num; i++) {
			if ((copy[i].Arrival > -1) && (time >= copy[i].IO_timing)) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //이거 IO_timing 같고 끝나는 시간 빠른 애들이 먼저 처리되도록 추가
			} //Arrival에 IO 제일 먼저 반영
		}

		//index 골라내기

		for (int i = 0; i < process_num; i++) {
			if (copy[i].Arrival > -1) {
				index = i;
				break;
			}
		} // 멀쩡한 시작점 찾기

		for (int i = 0; i < process_num; i++) {

			if ((copy[i].Arrival > -1) && (copy[index].Arrival > copy[i].Arrival)) {
				index = i;
			}
		}// 그 중에서 일단 Arrival 제일 작은 걸로

		for (int i = 0; i < process_num; i++) {

			if ((copy[i].Arrival > -1) && (copy[index].Arrival >= copy[i].Arrival)) {



				if (copy[index].Arrival == copy[i].Arrival) {

					if (copy[index].Priority < copy[i].Priority) {
						index = i; //Priority가 더 크면 바꿔
					}
				}
				else {
					//copy[i]가 더 빨리 도착한다면
					if (copy[index].Arrival <= time) {
						//둘다 시간 안에 이미 도착
						if (copy[index].Priority <= copy[i].Priority) {
							index = i; //i가 더 작으면 인덱스로 지정
						}
					}

					else index = i;
				}
			}

			else if ((copy[i].Arrival > -1) && (copy[index].Arrival < copy[i].Arrival)) {
				if (time >= copy[i].Arrival) {
					if (copy[index].Priority < copy[i].Priority) {
						index = i;
					}
				}


			}



			// 이미 도착한 놈들 중에서 CPU_burst가 가장 작은 놈을 찾아서 index로 지정
			//printf("P%d time: %d IO_t: %d IO_b : %d\n", index + 1, time, copy[i].IO_timing, copy[i].IO_burst);
			//printf("Arrival[%d] : %d index : %d finished : %d p[%d]cpburst : %d\n", i, copy[i].Arrival, index, finished, i ,copy[i].CPU_burst);
			//printf("IO_b : %d IO_t : %d \n", copy[i].IO_burst, copy[i].IO_timing);
			if ((copy[i].Arrival > -1) && (time >= (copy[i].IO_timing + copy[i].IO_burst))) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //이거 IO_timing 같고 끝나는 시간 빠른 애들이 먼저 처리되도록 추가
			}

		}



		if (time < copy[index].Arrival) {
			IDLE_time += copy[index].Arrival - time;
			//printf("%d idle %d\n",time,copy[index].Arrival); //체크용
			time = copy[index].Arrival;

			//printf("real minus 1 대입\n");
			if ((print != 0) && (process[print - 1] == -1)) {
				process_time[print - 1] = time;
				finished_time[index] = time;

			}
			else {
				process[print] = -1;
				process_time[print] = time;
				finished_time[index] = time;
				//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
				print += 1;
			}



			//자 여기 남은 시간 어케 쓸거야 ex 현재시간 15, 도착시간 20 남은 5초
		}
		//지금 시간이 도착시간보다 더 크면? 지금시간은 25 // 20 > 15                    15                  9                          20
		//근데 여기서 문제가 되는게 현재 시간보다 IO_timing이 더 작아서 문제군
		else {
			//            36        34				34			14						36								10
			if ((copy[index].IO_timing > time) && ((time + copy[index].CPU_burst) > (copy[index].IO_timing)) && (copy[index].IO_burst != 0)) {

				int temp = (copy[index].IO_timing) - time;
				//int temp = (time + copy[index].CPU_burst) - (copy[index].IO_timing);
				copy[index].Arrival = copy[index].IO_timing + copy[index].IO_burst;
				copy[index].CPU_burst -= temp;
				//copy[index].IO_burst = 0;
				time += temp;

				//printf("check\n");
				//printf("P%d  %d\n", index+1, time);

				process[print] = copy[index].PID;
				process_time[print] = time;
				finished_time[index] = time;
				//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
				print += 1;




			}
			else {
				if ((time >= copy[index].IO_timing) && (time < copy[index].IO_timing + copy[index].IO_burst)) {
					copy[index].Arrival = copy[index].IO_timing + copy[index].IO_burst;
					copy[index].IO_burst = 0;
				}
				else {
					time += copy[index].CPU_burst;
					copy[index].Arrival = -1;
					copy[index].CPU_burst = 0;
					finished += 1;
					//printf("here ");
					//printf("P%d  %d\n", index+1, time);
					process[print] = copy[index].PID;
					process_time[print] = time;
					finished_time[index] = time;
					//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
					print += 1;
				}
			}


		}




	}
	printf("<Priority>\n");
	Gantt(print, process, process_time);
	printf("Average Turnaround Time : ( ");
	int sum = 0;
	printf("%d ", finished_time[0] - info[0].Arrival);
	sum += finished_time[0] - info[0].Arrival;
	for (int i = 1; i < process_num; i++) {
		printf("+ %d ", finished_time[i] - info[i].Arrival);
		sum += finished_time[i] - info[i].Arrival;
	}
	printf(") / %d = %d\n", process_num, sum / process_num);


	printf("Average Waiting Time : ( ");
	sum = 0;
	if (finished_time[0] > info[0].IO_timing) sum += finished_time[0] - info[0].Arrival - info[0].IO_burst - info[0].CPU_burst;
	else sum += finished_time[0] - info[0].Arrival - info[0].CPU_burst;
	printf("%d", sum);
	for (int i = 1; i < process_num; i++) {
		if (finished_time[i] > info[i].IO_timing) {
			sum += finished_time[i] - info[i].Arrival - info[i].IO_burst - info[i].CPU_burst;
			printf("+ %d ", finished_time[i] - info[i].Arrival - info[i].IO_burst - info[i].CPU_burst);

		}
		else {
			sum += finished_time[i] - info[i].Arrival - info[i].CPU_burst;
			printf("+ %d ", finished_time[i] - info[i].Arrival - info[i].CPU_burst);
		}
	}
	printf(") / %d = %d\n", process_num, sum / process_num);
	float Utilization = ((float)(time - IDLE_time)) / (float)time;
	printf("CPU Utilization : (%d/%d)*100 = %.1f%%\n", time - IDLE_time, time, (Utilization * 100));
}


void P_Priority(process copy[]) {
	//SJF말고 Priority 큰거 우선으로 바꾸자
	int time = 0;
	int finished = 0;
	int print = 0;
	int process[100] = { 0 };
	int process_time[100] = { 0 };
	int IO_issue = 0;
	int current_time = 0; //preemptive 될 때 burst 감소시킬 때 쓸 변수
	int min = 0;
	int next_index = 0;
	int before_index = 0;
	int before_time = 0;
	int finished_time[8] = { 0 }; //원래 arrival과 끝나는 시간 차이 = turnaround time
	int IDLE_time = 0;
	Copy();
	int index = -2; // preemption이라 밖에 놔두었음.

	while (finished < process_num) {

		int next = 0;

		for (int i = 0; i < process_num; i++) {
			if ((copy[i].Arrival > -1) && (time >= copy[i].IO_timing)) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //이거 IO_timing 같고 끝나는 시간 빠른 애들이 먼저 처리되도록 추가
			} //Arrival에 IO 반영
		}


		for (int i = 0; i < process_num; i++) {

			if (copy[i].Arrival > -1) {
				min = copy[i].Arrival;
				break;
			}

		}

		for (int i = 0; i < process_num; i++) {
			if (copy[i].Arrival > -1) {
				if (min >= copy[i].Arrival) {
					min = copy[i].Arrival;
					next = i;
					if (copy[i].Arrival > time) next = -2;
				}
			}
		}


		// 이미 도착한 놈 빼고 제일 적은놈부터 시작 index 찾기
		//if( finished == process_num) break;

		for (int i = 0; i < process_num; i++) {



			// 당장 도착하는 것들 중에서 고르기 시작해야지
			if ((copy[i].Arrival > -1) && (time >= copy[i].Arrival)) {

				if (copy[next].Priority <= copy[i].Priority) {

					next = i;

				}
			}

			// 이미 도착한 놈들 중에서 CPU_burst가 가장 작은 놈을 찾아서 index로 지정
			//printf("P%d time: %d IO_t: %d IO_b : %d\n", index + 1, time, copy[i].IO_timing, copy[i].IO_burst);
			//printf("next : %d min : %d Arrival[%d] : %d index : %d finished : %d p[%d]cpburst : %d\n", next, min, i, copy[i].Arrival, index, finished, i ,copy[i].CPU_burst);
			//printf("IO_b : %d IO_t : %d IO_issue : %d\n", copy[i].IO_burst, copy[i].IO_timing,IO_issue);
			if ((copy[i].Arrival > -1) && (time >= (copy[i].IO_timing + copy[i].IO_burst))) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //이거 IO_timing 같고 끝나는 시간 빠른 애들이 먼저 처리되도록 추가
			} // 얘는 나중에 지워도 될듯?
		}
		if ((index != -2) && (index != next) && (IO_issue == 0)) {
			process[print] = copy[index].PID;
			//printf("추가 완료1!!!!!!!\n");
			process_time[print] = time;
			finished_time[index] = time;
			//copy[index].CPU_burst -= current_time;
			//current_time = 0;
			print += 1;
		}
		if ((index != -2) && (index != next) && (IO_issue == 1)) {
			process[print] = copy[index].PID;
			//printf("추가 완료2!!!!!!!\n");
			process_time[print] = time - 1;
			finished_time[index] = time - 1;
			//copy[index].CPU_burst -= current_time;
			//current_time = 0;
			print += 1;
		}

		index = next;



		if (time < min) {
			IDLE_time += min - time;
			//printf("%d idle %d\n",time,copy[index].Arrival); //체크용
			time += min - time;
			//printf("real minus 1 대입\n");
			index = -2;
			process[print] = -1;
			//printf("추가 완료3!!!!!!!\n");
			process_time[print] = time;

			//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
			print += 1;
			continue;

		}//idle 한번에

		else {

			//            35         35            34         14                  36                        10
			if ((copy[index].IO_timing > time) && ((time + copy[index].CPU_burst) > (copy[index].IO_timing)) && (copy[index].IO_burst != 0)) {

				copy[index].CPU_burst -= 1;
				if (copy[index].CPU_burst == 0) {
					copy[index].Arrival = -1;
					finished += 1;

				}

				time++;
				//current_time ++;
				IO_issue = 0;
			}
			else {
				if (IO_issue == 1) {
					copy[index].CPU_burst -= 1;
					if (copy[index].CPU_burst == 0) {
						copy[index].Arrival = -1;
						finished += 1;

					}
					IO_issue = 0;
				}

				if ((time >= copy[index].IO_timing) && (time < copy[index].Arrival)) {

					if (copy[index].IO_burst != 0) time++;
					IO_issue = 1;
				}


				else {
					copy[index].CPU_burst -= 1;
					time++;
					if (copy[index].CPU_burst == 0) {
						copy[index].Arrival = -1;
						finished += 1;
					}
				}

			}

			if (finished == process_num) {
				process[print] = copy[index].PID;
				process_time[print] = time;
				finished_time[index] = time;
				print += 1;
			}
		}
	}
	printf("<P_Priority>\n");
	Gantt(print, process, process_time);
	printf("Average Turnaround Time : ( ");
	int sum = 0;
	printf("%d ", finished_time[0] - info[0].Arrival);
	sum += finished_time[0] - info[0].Arrival;
	for (int i = 1; i < process_num; i++) {
		printf("+ %d ", finished_time[i] - info[i].Arrival);
		sum += finished_time[i] - info[i].Arrival;
	}
	printf(") / %d = %d\n", process_num, sum / process_num);


	printf("Average Waiting Time : ( ");
	sum = 0;
	if (finished_time[0] > info[0].IO_timing) sum += finished_time[0] - info[0].Arrival - info[0].IO_burst - info[0].CPU_burst;
	else sum += finished_time[0] - info[0].Arrival - info[0].CPU_burst;
	printf("%d", sum);
	for (int i = 1; i < process_num; i++) {
		if (finished_time[i] > info[i].IO_timing) {
			sum += finished_time[i] - info[i].Arrival - info[i].IO_burst - info[i].CPU_burst;
			printf("+ %d ", finished_time[i] - info[i].Arrival - info[i].IO_burst - info[i].CPU_burst);

		}
		else {
			sum += finished_time[i] - info[i].Arrival - info[i].CPU_burst;
			printf("+ %d ", finished_time[i] - info[i].Arrival - info[i].CPU_burst);
		}
	}
	printf(") / %d = %d\n", process_num, sum / process_num);
	float Utilization = ((float)(time - IDLE_time)) / (float)time;
	printf("CPU Utilization : (%d/%d)*100 = %.1f%%\n", time - IDLE_time, time, (Utilization * 100));
}



void RR(process copy[]) {
	int time = 0;
	int finished = 0;
	int print = 0;
	int process[400] = { 0 };
	int process_time[400] = { 0 };
	int pass = 0;
	int index = -2;
	int before_index = 0;
	int again = 0;
	int finished_time[8] = { 0 }; //원래 arrival과 끝나는 시간 차이 = turnaround time
	int IDLE_time = 0;
	//printf("RR time : %d", RR_time);
	while (finished < process_num) {



		//프로세스 순서대로 돌아
		pass = 0;

		for (int i = 0; i < process_num; i++) {



			//printf("1번\n");
			for (int t = 0; t < RR_time; t++) {
				if ((copy[i].Arrival > -1) && (time >= copy[i].IO_timing)) {
					copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //이거 IO_timing 같고 끝나는 시간 빠른 애들이 먼저 처리되도록 추가
				} //Arrival에 IO 반영
				//printf("2번\n");
				if ((copy[i].Arrival <= time) && (copy[i].Arrival > -1)) {
					if (index == -1) {
						//printf("3번\n");
						process[print] = -1;
						process_time[print] = time;
						print++;
					}
					index = i;


					//printf("3번\n");
					copy[index].CPU_burst -= 1;
					if (copy[index].CPU_burst == 0) {
						copy[index].Arrival = -1;
						finished++;
						time++;
						//printf("1번\n");
						process[print] = copy[index].PID;
						process_time[print] = time;
						finished_time[index] = time;
						print++;
						again = 1;
						break;
					}
					else {
						time++;

					}

				}
				else if ((copy[i].Arrival > time) || (copy[i].Arrival == -1)) {
					pass++;
				}



				if ((t == (RR_time - 1)) && (index == i) && (copy[index].Arrival != -1)) {
					//printf("2번\n");
					process[print] = copy[index].PID;
					process_time[print] = time;
					finished_time[index] = time;
					print++;
				}


			}




			if (pass == (RR_time * process_num)) {
				IDLE_time++;
				index = -1;
				time++;
				pass = 0;
			}
			//printf("P%d time: %d IO_t: %d IO_b : %d\n", i + 1, time, copy[i].IO_timing, copy[i].IO_burst);
			//printf("cpu_b : %d\n", copy[i].CPU_burst);
			//printf("[p%d]Arrival : %d, index = %d pass : %d\n", i + 1, copy[i].Arrival, index, pass);


		}

	}
	printf("<RR>\n");
	Gantt(print, process, process_time);
	printf("Average Turnaround Time : ( ");
	int sum = 0;
	printf("%d ", finished_time[0] - info[0].Arrival);
	sum += finished_time[0] - info[0].Arrival;
	for (int i = 1; i < process_num; i++) {
		printf("+ %d ", finished_time[i] - info[i].Arrival);
		sum += finished_time[i] - info[i].Arrival;
	}
	printf(") / %d = %d\n", process_num, sum / process_num);


	printf("Average Waiting Time : ( ");
	sum = 0;
	if (finished_time[0] > info[0].IO_timing) sum += finished_time[0] - info[0].Arrival - info[0].IO_burst - info[0].CPU_burst;
	else sum += finished_time[0] - info[0].Arrival - info[0].CPU_burst;
	printf("%d", sum);
	for (int i = 1; i < process_num; i++) {
		if (finished_time[i] > info[i].IO_timing) {
			sum += finished_time[i] - info[i].Arrival - info[i].IO_burst - info[i].CPU_burst;
			printf("+ %d ", finished_time[i] - info[i].Arrival - info[i].IO_burst - info[i].CPU_burst);

		}
		else {
			sum += finished_time[i] - info[i].Arrival - info[i].CPU_burst;
			printf("+ %d ", finished_time[i] - info[i].Arrival - info[i].CPU_burst);
		}
	}
	printf(") / %d = %d\n", process_num, sum / process_num);
	float Utilization = ((float)(time - IDLE_time)) / (float)time;
	printf("CPU Utilization : (%d/%d)*100 = %.1f%%\n", time - IDLE_time, time, (Utilization * 100));

}

int main() {

	Create_Process();
	Copy();
	FCFS(copy);
	Copy();
	SJF(copy);
	Copy();
	P_SJF(copy);
	Copy();
	Priority(copy);
	Copy();
	P_Priority(copy);
	Copy();
	RR(copy);

	/*
	printf("|\t|\n");
	printf("|   P1  |\n");//앞에3개 뒤에2개
	printf("|\t|\n");
	printf("12\t19\n");
	//Gannt(3, 7, 3);
	*/
}