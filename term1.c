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
			info[i].IO_timing = info[i].Arrival + rand() % 16; //IO_timing�� ���� �ð����� ���

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
			// ���⼭ printf("\n"); �� ������ �ؿ� ����� �ȵǰ� ������ ����� �ȴ�... �ֱ׷���?


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
	int finished_time[8] = { 0 }; //���� arrival�� ������ �ð� ���� = turnaround time
	//int p_waiting[8] = { 0 }; arrival�� ������ �ð� �����ε� ������ �ð��� IO_timing���� �ڶ�� IO �������ϱ� turnaround - IO_burst
	int IDLE_time = 0;
	//������ ��
	Copy();
	while (finished < process_num) {
		int index = -1;

		for (int i = 0; i < process_num; i++) {
			if ((copy[i].Arrival > -1) && (time >= copy[i].IO_timing)) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //�̰� IO_timing ���� ������ �ð� ���� �ֵ��� ���� ó���ǵ��� �߰�
			} //Arrival�� IO �ݿ�
		}

		for (int i = 0; i < process_num; i++) {
			if (copy[i].Arrival > -1) {
				index = i;
				break;
			} // �̹� �� ���� �� ���� ���� ��������� ����
		}
		for (int i = 0; i < process_num; i++) {
			if ((copy[i].Arrival > -1) && (copy[index].Arrival > copy[i].Arrival)) {
				index = i;
			}

			//printf("P%d time: %d IO_t: %d IO_b : %d\n", index + 1, time, copy[i].IO_timing, copy[i].IO_burst);
			//printf("Arrival[%d] : %d index : %d finished : %d p[%d]cpburst : %d\n", i, copy[i].Arrival, index, finished, i ,copy[i].CPU_burst);
			//printf("IO_b : %d IO_t : %d \n", copy[i].IO_burst, copy[i].IO_timing);
			//printf("P%d time: %d IO_t: %d IO_b : %d\n", index + 1, time, copy[i].IO_timing, copy[i].IO_burst);


		}//���� ���� �����ð� �ε��� ã��
		if (time < copy[index].Arrival) {
			IDLE_time += copy[index].Arrival - time;
			//printf("%d idle %d\n",time,copy[index].Arrival); //üũ��
			time = copy[index].Arrival;

			//printf("real minus 1 ����\n");
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



			//�� ���� ���� �ð� ���� ���ž� ex ����ð� 15, �����ð� 20 ���� 5��
		}



		/*
		if (time < copy[index].Arrival) {
			//printf("%d idle %d\n",time,copy[index].Arrival); //üũ��
			IDLE_time+= copy[index].Arrival - time;
			time = copy[index].Arrival;

			//printf("real minus 1 ����\n");
			process[print] = -1;
			process_time[print] = time;
			//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
			print += 1;

			//�� ���� ���� �ð� ���� ���ž� ex ����ð� 15, �����ð� 20 ���� 5��
		}*/




		//���� �ð��� �����ð����� �� ũ��? ���ݽð��� 25 // 20 > 15                    15                  9                          20
		//�ٵ� ���⼭ ������ �Ǵ°� ���� �ð����� IO_timing�� �� �۾Ƽ� ������
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
	int finished_time[8] = { 0 }; //���� arrival�� ������ �ð� ���� = turnaround time
	int IDLE_time = 0;
	//shortest job first
	Copy();
	while (finished < process_num) {
		int index = -1;
		for (int i = 0; i < process_num; i++) {
			if ((copy[i].Arrival > -1) && (time >= copy[i].IO_timing)) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //�̰� IO_timing ���� ������ �ð� ���� �ֵ��� ���� ó���ǵ��� �߰�
			} //Arrival�� IO �ݿ�
		}


		for (int i = 0; i < process_num; i++) {
			if (copy[i].Arrival > -1) {
				index = i;
				break;
			}
		}

		//���⵵ Arrival ���� ������ ������ �ʿ�?

		for (int i = 0; i < process_num; i++) {

			if ((copy[i].Arrival > -1) && (copy[index].Arrival >= copy[i].Arrival)) {



				if (copy[index].Arrival == copy[i].Arrival) {

					if (copy[index].CPU_burst > copy[i].CPU_burst) {
						index = i; //�ƴϸ� �״��
					}
				}
				else {
					//copy[i]�� �� ���� �����Ѵٸ�
					if (copy[index].Arrival <= time) {
						//�Ѵ� �ð� �ȿ� �̹� ����
						if (copy[index].CPU_burst >= copy[i].CPU_burst) {
							index = i; //i�� �� ������ �ε����� ����
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





			// �̹� ������ ��� �߿��� CPU_burst�� ���� ���� ���� ã�Ƽ� index�� ����
			//printf("P%d time: %d IO_t: %d IO_b : %d\n", i+1, time, copy[i].IO_timing, copy[i].IO_burst);
			//printf("[p%d]Arrival : %d, index : %d\n", i+1, copy[i].Arrival,index);
			if ((copy[i].Arrival > -1) && (time >= (copy[i].IO_timing + copy[i].IO_burst))) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //�̰� IO_timing ���� ������ �ð� ���� �ֵ��� ���� ó���ǵ��� �߰�
			}

		}

		if (time < copy[index].Arrival) {
			IDLE_time += copy[index].Arrival - time;
			//printf("%d idle %d\n",time,copy[index].Arrival); //üũ��
			time = copy[index].Arrival;

			//printf("real minus 1 ����\n");
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



			//�� ���� ���� �ð� ���� ���ž� ex ����ð� 15, �����ð� 20 ���� 5��
		}

		/*
		if (time < copy[index].Arrival) {
			//printf("%d idle %d\n",time,copy[index].Arrival); //üũ��
			IDLE_time += copy[index].Arrival - time;
			time = copy[index].Arrival;

			//printf("real minus 1 ����\n");
			process[print] = -1;
			process_time[print] = time;
			//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
			print += 1;

			//�� ���� ���� �ð� ���� ���ž� ex ����ð� 15, �����ð� 20 ���� 5��
		}*/






		//�ٵ� ���⼭ ������ �Ǵ°� ���� �ð����� IO_timing�� �� �۾Ƽ� ������
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
	int current_time = 0; //preemptive �� �� burst ���ҽ�ų �� �� ����
	int min = 0;
	int next_index = 0;
	int before_index = 0;
	int before_time = 0;
	int finished_time[8] = { 0 }; //���� arrival�� ������ �ð� ���� = turnaround time
	int IDLE_time = 0;
	Copy();
	int index = -2; // preemption�̶� �ۿ� ���ξ���.

	while (finished < process_num) {

		int next = 0;

		for (int i = 0; i < process_num; i++) {
			if ((copy[i].Arrival > -1) && (time >= copy[i].IO_timing)) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //�̰� IO_timing ���� ������ �ð� ���� �ֵ��� ���� ó���ǵ��� �߰�
			} //Arrival�� IO �ݿ�
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


		// �̹� ������ �� ���� ���� ��������� ���� index ã��
		//if( finished == process_num) break;

		for (int i = 0; i < process_num; i++) {



			// ���� �����ϴ� �͵� �߿��� ���� �����ؾ���
			if ((copy[i].Arrival > -1) && (time >= copy[i].Arrival)) {

				if (copy[next].CPU_burst > copy[i].CPU_burst) {

					next = i;

				}
			}

			// �̹� ������ ��� �߿��� CPU_burst�� ���� ���� ���� ã�Ƽ� index�� ����
			//printf("P%d time: %d IO_t: %d IO_b : %d\n", index + 1, time, copy[i].IO_timing, copy[i].IO_burst);
			//printf("next : %d min : %d Arrival[%d] : %d index : %d finished : %d p[%d]cpburst : %d\n", next, min, i, copy[i].Arrival, index, finished, i ,copy[i].CPU_burst);
			//printf("IO_b : %d IO_t : %d IO_issue : %d\n", copy[i].IO_burst, copy[i].IO_timing,IO_issue);
			if ((copy[i].Arrival > -1) && (time >= (copy[i].IO_timing + copy[i].IO_burst))) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //�̰� IO_timing ���� ������ �ð� ���� �ֵ��� ���� ó���ǵ��� �߰�
			} // ��� ���߿� ������ �ɵ�?
		}
		if ((index != -2) && (index != next) && (IO_issue == 0)) {
			process[print] = copy[index].PID;
			//printf("�߰� �Ϸ�1!!!!!!!\n");
			process_time[print] = time;
			finished_time[index] = time;
			//copy[index].CPU_burst -= current_time;
			//current_time = 0;
			print += 1;
		}
		if ((index != -2) && (index != next) && (IO_issue == 1)) {
			process[print] = copy[index].PID;
			//printf("�߰� �Ϸ�2!!!!!!!\n");
			process_time[print] = time - 1;
			finished_time[index] = time - 1;
			//copy[index].CPU_burst -= current_time;
			//current_time = 0;
			print += 1;
		}

		index = next;



		if (time < min) {

			//printf("%d idle %d\n",time,copy[index].Arrival); //üũ��
			IDLE_time += min - time;
			time += min - time;
			//printf("real minus 1 ����\n");
			index = -2;
			process[print] = -1;
			//printf("�߰� �Ϸ�3!!!!!!!\n");
			process_time[print] = time;
			//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
			print += 1;
			continue;

		}//idle �ѹ���

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
	int finished_time[8] = { 0 }; //���� arrival�� ������ �ð� ���� = turnaround time
	int IDLE_time = 0;
	//Priority ū �� ����, �⺻ Ʋ�� SJF ��� �����ϱ� CPU_burst ª���� ��� Priority ū�ŷ� �ٲٸ� ��
	Copy();
	while (finished < process_num) {
		int index = -1;
		for (int i = 0; i < process_num; i++) {
			if ((copy[i].Arrival > -1) && (time >= copy[i].IO_timing)) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //�̰� IO_timing ���� ������ �ð� ���� �ֵ��� ���� ó���ǵ��� �߰�
			} //Arrival�� IO ���� ���� �ݿ�
		}

		//index ��󳻱�

		for (int i = 0; i < process_num; i++) {
			if (copy[i].Arrival > -1) {
				index = i;
				break;
			}
		} // ������ ������ ã��

		for (int i = 0; i < process_num; i++) {

			if ((copy[i].Arrival > -1) && (copy[index].Arrival > copy[i].Arrival)) {
				index = i;
			}
		}// �� �߿��� �ϴ� Arrival ���� ���� �ɷ�

		for (int i = 0; i < process_num; i++) {

			if ((copy[i].Arrival > -1) && (copy[index].Arrival >= copy[i].Arrival)) {



				if (copy[index].Arrival == copy[i].Arrival) {

					if (copy[index].Priority < copy[i].Priority) {
						index = i; //Priority�� �� ũ�� �ٲ�
					}
				}
				else {
					//copy[i]�� �� ���� �����Ѵٸ�
					if (copy[index].Arrival <= time) {
						//�Ѵ� �ð� �ȿ� �̹� ����
						if (copy[index].Priority <= copy[i].Priority) {
							index = i; //i�� �� ������ �ε����� ����
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



			// �̹� ������ ��� �߿��� CPU_burst�� ���� ���� ���� ã�Ƽ� index�� ����
			//printf("P%d time: %d IO_t: %d IO_b : %d\n", index + 1, time, copy[i].IO_timing, copy[i].IO_burst);
			//printf("Arrival[%d] : %d index : %d finished : %d p[%d]cpburst : %d\n", i, copy[i].Arrival, index, finished, i ,copy[i].CPU_burst);
			//printf("IO_b : %d IO_t : %d \n", copy[i].IO_burst, copy[i].IO_timing);
			if ((copy[i].Arrival > -1) && (time >= (copy[i].IO_timing + copy[i].IO_burst))) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //�̰� IO_timing ���� ������ �ð� ���� �ֵ��� ���� ó���ǵ��� �߰�
			}

		}



		if (time < copy[index].Arrival) {
			IDLE_time += copy[index].Arrival - time;
			//printf("%d idle %d\n",time,copy[index].Arrival); //üũ��
			time = copy[index].Arrival;

			//printf("real minus 1 ����\n");
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



			//�� ���� ���� �ð� ���� ���ž� ex ����ð� 15, �����ð� 20 ���� 5��
		}
		//���� �ð��� �����ð����� �� ũ��? ���ݽð��� 25 // 20 > 15                    15                  9                          20
		//�ٵ� ���⼭ ������ �Ǵ°� ���� �ð����� IO_timing�� �� �۾Ƽ� ������
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
	//SJF���� Priority ū�� �켱���� �ٲ���
	int time = 0;
	int finished = 0;
	int print = 0;
	int process[100] = { 0 };
	int process_time[100] = { 0 };
	int IO_issue = 0;
	int current_time = 0; //preemptive �� �� burst ���ҽ�ų �� �� ����
	int min = 0;
	int next_index = 0;
	int before_index = 0;
	int before_time = 0;
	int finished_time[8] = { 0 }; //���� arrival�� ������ �ð� ���� = turnaround time
	int IDLE_time = 0;
	Copy();
	int index = -2; // preemption�̶� �ۿ� ���ξ���.

	while (finished < process_num) {

		int next = 0;

		for (int i = 0; i < process_num; i++) {
			if ((copy[i].Arrival > -1) && (time >= copy[i].IO_timing)) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //�̰� IO_timing ���� ������ �ð� ���� �ֵ��� ���� ó���ǵ��� �߰�
			} //Arrival�� IO �ݿ�
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


		// �̹� ������ �� ���� ���� ��������� ���� index ã��
		//if( finished == process_num) break;

		for (int i = 0; i < process_num; i++) {



			// ���� �����ϴ� �͵� �߿��� ���� �����ؾ���
			if ((copy[i].Arrival > -1) && (time >= copy[i].Arrival)) {

				if (copy[next].Priority <= copy[i].Priority) {

					next = i;

				}
			}

			// �̹� ������ ��� �߿��� CPU_burst�� ���� ���� ���� ã�Ƽ� index�� ����
			//printf("P%d time: %d IO_t: %d IO_b : %d\n", index + 1, time, copy[i].IO_timing, copy[i].IO_burst);
			//printf("next : %d min : %d Arrival[%d] : %d index : %d finished : %d p[%d]cpburst : %d\n", next, min, i, copy[i].Arrival, index, finished, i ,copy[i].CPU_burst);
			//printf("IO_b : %d IO_t : %d IO_issue : %d\n", copy[i].IO_burst, copy[i].IO_timing,IO_issue);
			if ((copy[i].Arrival > -1) && (time >= (copy[i].IO_timing + copy[i].IO_burst))) {
				copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //�̰� IO_timing ���� ������ �ð� ���� �ֵ��� ���� ó���ǵ��� �߰�
			} // ��� ���߿� ������ �ɵ�?
		}
		if ((index != -2) && (index != next) && (IO_issue == 0)) {
			process[print] = copy[index].PID;
			//printf("�߰� �Ϸ�1!!!!!!!\n");
			process_time[print] = time;
			finished_time[index] = time;
			//copy[index].CPU_burst -= current_time;
			//current_time = 0;
			print += 1;
		}
		if ((index != -2) && (index != next) && (IO_issue == 1)) {
			process[print] = copy[index].PID;
			//printf("�߰� �Ϸ�2!!!!!!!\n");
			process_time[print] = time - 1;
			finished_time[index] = time - 1;
			//copy[index].CPU_burst -= current_time;
			//current_time = 0;
			print += 1;
		}

		index = next;



		if (time < min) {
			IDLE_time += min - time;
			//printf("%d idle %d\n",time,copy[index].Arrival); //üũ��
			time += min - time;
			//printf("real minus 1 ����\n");
			index = -2;
			process[print] = -1;
			//printf("�߰� �Ϸ�3!!!!!!!\n");
			process_time[print] = time;

			//printf("p[%d] : %d , p_t[%d] : %d\n", print, process[print], print, process_time[print]);
			print += 1;
			continue;

		}//idle �ѹ���

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
	int finished_time[8] = { 0 }; //���� arrival�� ������ �ð� ���� = turnaround time
	int IDLE_time = 0;
	//printf("RR time : %d", RR_time);
	while (finished < process_num) {



		//���μ��� ������� ����
		pass = 0;

		for (int i = 0; i < process_num; i++) {



			//printf("1��\n");
			for (int t = 0; t < RR_time; t++) {
				if ((copy[i].Arrival > -1) && (time >= copy[i].IO_timing)) {
					copy[i].Arrival = copy[i].IO_timing + copy[i].IO_burst; //�̰� IO_timing ���� ������ �ð� ���� �ֵ��� ���� ó���ǵ��� �߰�
				} //Arrival�� IO �ݿ�
				//printf("2��\n");
				if ((copy[i].Arrival <= time) && (copy[i].Arrival > -1)) {
					if (index == -1) {
						//printf("3��\n");
						process[print] = -1;
						process_time[print] = time;
						print++;
					}
					index = i;


					//printf("3��\n");
					copy[index].CPU_burst -= 1;
					if (copy[index].CPU_burst == 0) {
						copy[index].Arrival = -1;
						finished++;
						time++;
						//printf("1��\n");
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
					//printf("2��\n");
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
	printf("|   P1  |\n");//�տ�3�� �ڿ�2��
	printf("|\t|\n");
	printf("12\t19\n");
	//Gannt(3, 7, 3);
	*/
}