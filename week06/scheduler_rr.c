#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/time.h>

#define PS_MAX 10

int quantum;

// holds the scheduling data of one process
typedef struct{
    int idx; // process idx (index)
    int at, bt, rt, wt, ct, tat; // arrival time, burst time, response time, waiting time, completion time, turnaround time.
    int burst; // remaining burst (this should decrement when the process is being executed)
    int quantum; // remaining part of quantum
} ProcessData;

// the idx of the running process
int running_process = -1; // -1 means no running processes

// the total time of the timer
unsigned total_time; // should increment one second at a time by the scheduler

// data of the processes
ProcessData data[PS_MAX]; // array of process data

// array of all process pids
pid_t ps[PS_MAX]; // zero valued pids - means the process is terminated or not created yet

// size of data array
unsigned data_size;

void read_file(FILE* file){
    char line[256];
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d %d %d", &data[i].idx, &data[i].at, &data[i].bt) != 3) {
            continue;
        }
        data[i].burst = data[i].bt;
        ps[i] = 0;
        i++;
    }
    data_size = i;
}

// send signal SIGCONT to the worker process
void resume(pid_t process) {
    if(process > 0 && kill(process, 0) == 0) {
        kill(process, SIGCONT);
    }
}

// send signal SIGTSTP to the worker process
void suspend(pid_t process) {
    if(process > 0 && kill(process, 0) == 0) {
        kill(process, SIGTSTP);
    }
}

// send signal SIGTERM to the worker process
void terminate(pid_t process) {
    if(process > 0 && kill(process, 0) == 0) {
        kill(process, SIGTERM);
        waitpid(process, NULL, 0);
    }
}

// create a process using fork
void create_process(int new_process) {
    suspend(ps[running_process]);
    pid_t pid = fork();
    if(pid == 0) {
        char arg[10];
        sprintf(arg, "%d", new_process);
        char* args[] = {"./worker", arg, NULL};
        execvp(args[0], args);
        exit(EXIT_SUCCESS);
    } else {
        ps[new_process] = pid;
        running_process = new_process;
    }
}

// find next process for running
ProcessData find_next_process() {
    int location = 0;

    bool allZeros = true;
    for (int i = 0; i < data_size; i++) {
        if (data[i].burst > 0 && data[i].quantum > 0) {
            allZeros = false;
            break;
        }
    }
    if (allZeros) {
        for (int i = 0; i < data_size; i++) {
            if (data[i].burst > 0 && data[i].quantum <= 0) {
                data[i].quantum = quantum;
            }
        }
    }
    location = -1;
    int min_at = INT_MAX;
    for (int i = 0; i < data_size; i++){
        if (data[i].burst > 0 && data[i].quantum > 0 && data[i].at <= total_time && data[i].at < min_at) {
            location = i;
            min_at = data[location].at;
        }
    }
    // If no process has arrived yet, increment the time and recursively call the function
    if (location == -1) {
        printf("Scheduler: Runtime: %u seconds.\nNo process has arrived yet.\n", total_time);
        total_time++;
        return find_next_process();
    }
    if(data[location].at > total_time){
        printf("Scheduler: Runtime: %u seconds.\nProcess %d: has not arrived yet.\n", total_time, location);
        // increment the time
        total_time++;
        return find_next_process();
    }
    return data[location];
}

// reports the metrics and simulation results
void report() {
    printf("Simulation results.....\n");
    int sum_wt = 0;
    int sum_tat = 0;
    for (int i=0; i< data_size; i++){
        printf("process %d: \n", i);
        printf("	at=%d\n", data[i].at);
        printf("	bt=%d\n", data[i].bt);
        printf("	ct=%d\n", data[i].ct);
        printf("	wt=%d\n", data[i].wt);
        printf("	tat=%d\n", data[i].tat);
        printf("	rt=%d\n", data[i].rt);
        sum_wt += data[i].wt;
        sum_tat += data[i].tat;
    }

    printf("data size = %d\n", data_size);
    float avg_wt = (float)sum_wt/(float)data_size;
    float avg_tat = (float)sum_tat/(float)data_size;
    printf("Average results for this run:\n");
    printf("	avg_wt=%f\n", avg_wt);
    printf("	avg_tat=%f\n", avg_tat);
}

void check_burst(){

    for(int i = 0; i < data_size; i++)
        if (data[i].burst > 0)
            return;

    // report simulation results
    report();

    // terminate the scheduler :)
    exit(EXIT_SUCCESS);
}


// This function is called every one second as handler for SIGALRM signal
void schedule_handler(int signum) {
    // increment the total time
    total_time++;

    if(running_process != -1) {
        data[running_process].burst--;
        data[running_process].quantum--;
        printf("Scheduler: Runtime: %u seconds\n", total_time);
        printf("Process %d is running with %d seconds left\n", running_process, data[running_process].burst);
        if(data[running_process].burst == 0) {
            printf("Scheduler: Terminating Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            terminate(ps[running_process]);
            waitpid(ps[running_process], NULL, 0);
            data[running_process].ct = (int) total_time;
            data[running_process].tat = data[running_process].ct - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;
            running_process = -1;
        } else if (data[running_process].quantum == 0) {
            printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            suspend(ps[running_process]);
            running_process = -1;
        }
    }
    check_burst();
    if (data[running_process].burst <= 0) {
        ProcessData next_process = find_next_process();
        if (running_process != next_process.idx) {
            int next_idx = next_process.idx;
            if (ps[next_idx] == 0) {
                create_process(next_idx);
                printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", next_idx, data[next_idx].burst);
                data[next_idx].rt = (int) total_time - data[next_idx].at;
            } else {
                if (running_process != -1) {
                    printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process,
                           data[running_process].burst);
                    suspend(ps[running_process]);
                }
                printf("Scheduler: Resuming Process %d (Remaining Time: %d)\n", next_idx, data[next_idx].burst);
                resume(ps[next_idx]);
            }
            running_process = next_idx;
            data[running_process].quantum = quantum;
        }
    }
}


int main(int argc, char *argv[]) {
    // read the data file
    FILE *in_file  = fopen(argv[1], "r");
    if (in_file == NULL) {
        printf("File is not found or cannot open it!\n");
        exit(EXIT_FAILURE);
    } else {
        read_file(in_file);
    }
    printf("Input quantum: ");
    scanf("%d", &quantum);
    // set a timer
    struct itimerval timer;
    // the timer goes off 1 second after reset
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    // timer increments 1 second at a time
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    // this counts down and sends SIGALRM to the scheduler process after expiration.
    setitimer(ITIMER_REAL, &timer, NULL);
    // register the handler for SIGALRM signal
    signal(SIGALRM, schedule_handler);
    // Wait till all processes finish
    while(1); // infinite loop
}