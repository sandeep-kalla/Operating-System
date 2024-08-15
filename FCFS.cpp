#include <bits/stdc++.h>
using namespace std;

// structure of a process.
struct Process
{
    int id;
    int arrival;
    int burst;
    int completion;
    int waiting;
    int turnaround;
};

// function to calculate the completion time. (using Gantt chart)
void completionTime(vector<Process>& p, int n) {
    p[0].completion = p[0].burst;
    for (int i=1; i<n; i++) {
        p[i].completion = p[i-1].completion + p[i].burst;
    }
}

// function to calculate the turn around time.
void turnaroundTime(vector<Process>& p, int n) {
    for (int i=0; i<n; i++) {
        // TAT = CT - AT.
        p[i].turnaround = p[i].completion - p[i].arrival;
    }
}

// function to calculate the waiting time.
void waitingTime(vector<Process>& p, int n) {
    // since first process is having waiting time = 0.
    p[0].waiting = 0;
    for (int i=1; i<n; i++) {
        // WT = TAT - BT.
        p[i].waiting = p[i].turnaround - p[i].burst;
    }
}

// function to print the FCFS table.
void print(vector<Process>& p, int n) {
    cout << "First Come First Serve Scheduling :" << endl;
    
    // Print the table header
    cout << "+----+--------------+--------------+-------------------+-------------------+--------------+" << endl;
    cout << "| PID| Arrival Time |  Burst Time  |  Completion Time  |  TurnAround Time  | Waiting Time |" << endl;
    cout << "+----+--------------+--------------+-------------------+-------------------+--------------+" << endl;

    // Print the table rows
    for (int i = 0; i < n; i++) {
        cout << "| " << setw(2) << p[i].id << " | " 
             << setw(12) << p[i].arrival << " | "
             << setw(12) << p[i].burst << " | "
             << setw(17) << p[i].completion << " | "
             << setw(17) << p[i].turnaround << " | "
             << setw(12) << p[i].waiting << " |" << endl;
        cout << "+----+--------------+--------------+-------------------+-------------------+--------------+" << endl;
    }
}

// function to perform FCFS algorithm.
void FCFS(vector<Process>& p, int n) {
    completionTime(p, n);
    turnaroundTime(p, n);
    waitingTime(p, n);
}

// function for get the initial details of the processes for the user.
void input(vector<Process>& Processes, int n) {
    for (int i=0; i<n; i++) {
        Process p;
        p.id = i+1;
        cout << "enter arrival and burst time for process " << p.id << " : ";
        cin >> p.arrival >> p.burst;
        Processes.push_back(p);
    }
}

int main() {
    int n;
    cout << "enter number of processes: ";
    cin >> n;
    vector<Process> Processes;
    input(Processes, n);
    FCFS(Processes, n);
    print(Processes, n);
}