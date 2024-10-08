#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Process
{
    int id;
    int arrival;
    int burst;
    int priority;
    int remainingBurst;
    int completion;
    int waiting;
    int turnaround;
};

void input(vector<Process> &processes, int n)
{
    for (int i = 0; i < n; i++)
    {
        Process p;
        p.id = i + 1;
        cout << "Enter arrival time, burst time, and priority for process " << p.id << ": ";
        cin >> p.arrival >> p.burst >> p.priority;
        p.remainingBurst = p.burst; // Initialize remaining burst time
        processes.push_back(p);
    }
}

void completionTime(vector<Process> &processes, int n, vector<pair<int, int>> &ganttChart)
{
    int currentTime = 0;
    int completed = 0;
    int lastProcessId = -1; // To track the process in the last Gantt chart entry

    while (completed < n)
    {
        // Find the process with the highest priority that has arrived and is not completed
        int idx = -1;
        int highestPriority = INT_MAX;

        for (int i = 0; i < n; i++)
        {
            if (processes[i].arrival <= currentTime && processes[i].remainingBurst > 0)
            {
                if (processes[i].priority < highestPriority)
                {
                    highestPriority = processes[i].priority;
                    idx = i;
                }
            }
        }

        if (idx != -1)
        {
            // Update Gantt chart if the process changes
            if (lastProcessId != processes[idx].id)
            {
                ganttChart.push_back({processes[idx].id, currentTime});
                lastProcessId = processes[idx].id;
            }

            // Execute the selected process for 1 unit of time
            processes[idx].remainingBurst--;
            currentTime++;

            // If the process is completed
            if (processes[idx].remainingBurst == 0)
            {
                processes[idx].completion = currentTime;
                completed++;
            }

            // Update the end time in Gantt chart
            ganttChart.back().second = currentTime;
        }
        else
        {
            currentTime++; // If no process is found, increase the current time
        }
    }
}

void turnaroundTime(vector<Process> &processes, int n)
{
    for (int i = 0; i < n; i++)
    {
        processes[i].turnaround = processes[i].completion - processes[i].arrival;
    }
}

void waitingTime(vector<Process> &processes, int n)
{
    for (int i = 0; i < n; i++)
    {
        processes[i].waiting = processes[i].turnaround - processes[i].burst;
    }
}

void printTable(const vector<Process> &processes, int n)
{
    cout << "Priority Preemptive Scheduling:" << endl;
    cout << "+----+--------------+--------------+------------+-------------------+-------------------+--------------+" << endl;
    cout << "| PID| Arrival Time |  Burst Time  |  Priority  |  Completion Time  |  TurnAround Time  | Waiting Time |" << endl;
    cout << "+----+--------------+--------------+------------+-------------------+-------------------+--------------+" << endl;

    for (int i = 0; i < n; i++)
    {
        cout << "| " << setw(2) << processes[i].id << " | "
             << setw(12) << processes[i].arrival << " | "
             << setw(12) << processes[i].burst << " | "
             << setw(10) << processes[i].priority << " | "
             << setw(17) << processes[i].completion << " | "
             << setw(17) << processes[i].turnaround << " | "
             << setw(12) << processes[i].waiting << " |" << endl;
        cout << "+----+--------------+--------------+------------+-------------------+-------------------+--------------+" << endl;
    }
}

void printGanttChart(const vector<pair<int, int>> &ganttChart)
{
    cout << "\nGantt Chart:" << endl;
    cout << "+";
    for (size_t i = 0; i < ganttChart.size(); i++)
        cout << "----+";
    cout << endl
         << "|";

    for (const auto &entry : ganttChart)
        cout << " P" << setw(1) << entry.first << " |";
    cout << endl
         << "+";

    for (size_t i = 0; i < ganttChart.size(); i++)
        cout << "----+";
    cout << endl;

    cout << setw(2) << 0; // Start with 0
    for (const auto &entry : ganttChart)
        cout << setw(5) << entry.second;
    cout << endl;
}

void PriorityScheduling(vector<Process> &processes, int n)
{
    vector<pair<int, int>> ganttChart;

    completionTime(processes, n, ganttChart);
    turnaroundTime(processes, n);
    waitingTime(processes, n);

    printTable(processes, n);
    printGanttChart(ganttChart);

    double avgTurnaround = 0, avgWaiting = 0;
    for (const auto &process : processes)
    {
        avgTurnaround += process.turnaround;
        avgWaiting += process.waiting;
    }
    avgTurnaround /= n;
    avgWaiting /= n;

    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << avgTurnaround << endl;
    cout << "Average Waiting Time: " << fixed << setprecision(2) << avgWaiting << endl;
}

int main()
{
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes;
    input(processes, n);

    cout << "\n\n--- Priority Preemptive Scheduling ---\n";
    PriorityScheduling(processes, n);

    return 0;
}
