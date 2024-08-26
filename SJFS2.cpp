#include <bits/stdc++.h>
using namespace std;

struct Process
{
  int id;
  int arrival;
  int burst;
  int remaining;
  int completion;
  int waiting;
  int turnaround;
};

// function to calculate completion time.
void completionTime(vector<Process> &p, int n, vector<int> &gantt, vector<int> &startTimes, vector<int> &endTimes)
{
  int currentTime = 0, completed = 0, lastProcess = -1;
  while (completed < n)
  {
    int idx = -1, minRemaining = INT_MAX;

    // Find the process with the shortest remaining time
    for (int i = 0; i < n; i++)
    {
      if (p[i].arrival <= currentTime && p[i].remaining > 0 && p[i].remaining < minRemaining)
      {
        minRemaining = p[i].remaining;
        idx = i;
      }
    }

    if (idx != -1)
    {
      // Check if we need to add a new entry in the Gantt chart
      if (lastProcess != idx)
      {
        if (lastProcess != -1)
        {
          endTimes.push_back(currentTime);
        }
        gantt.push_back(p[idx].id);
        startTimes.push_back(currentTime);
        lastProcess = idx;
      }
      p[idx].remaining--;
      currentTime++;
      if (p[idx].remaining == 0)
      {
        p[idx].completion = currentTime;
        completed++;
        endTimes.push_back(currentTime);
        lastProcess = -1; // Reset lastProcess
      }
    }
    else
    {
      currentTime++;
    }
  }
}

// function to calculate the turn around time.
void turnaroundTime(vector<Process> &p, int n)
{
  for (int i = 0; i < n; i++)
  {
    p[i].turnaround = p[i].completion - p[i].arrival;
  }
}

// function to calculate the waiting time.
void waitingTime(vector<Process> &p, int n)
{
  for (int i = 0; i < n; i++)
  {
    p[i].waiting = p[i].turnaround - p[i].burst;
  }
}

// function to print table.
void print(vector<Process> &p, int n)
{
  cout << "SJF Preemptive (SRTF) Scheduling:\n";
  cout << "+----+--------------+--------------+-------------------+-------------------+--------------+\n";
  cout << "| PID| Arrival Time |  Burst Time  |  Completion Time  |  TurnAround Time  | Waiting Time |\n";
  cout << "+----+--------------+--------------+-------------------+-------------------+--------------+\n";

  for (int i = 0; i < n; i++)
  {
    cout << "| " << setw(2) << p[i].id << " | "
         << setw(12) << p[i].arrival << " | "
         << setw(12) << p[i].burst << " | "
         << setw(17) << p[i].completion << " | "
         << setw(17) << p[i].turnaround << " | "
         << setw(12) << p[i].waiting << " |\n";
    cout << "+----+--------------+--------------+-------------------+-------------------+--------------+\n";
  }
}

// function to print gantt chart.
void printGanttChart(const vector<int> &gantt, const vector<int> &startTimes, const vector<int> &endTimes)
{
  cout << "Gantt Chart:\n";

  cout << "+";
  for (size_t i = 0; i < gantt.size(); i++)
  {
    for (int j = 0; j < 5; j++)
      cout << "-";
    cout << "+";
  }
  cout << "\n|";

  // Print the process IDs
  for (size_t i = 0; i < gantt.size(); i++)
  {
    cout << " P" << gantt[i] << "  |";
  }
  cout << "\n+";

  for (size_t i = 0; i < gantt.size(); i++)
  {
    for (int j = 0; j < 5; j++)
      cout << "-";
    cout << "+";
  }
  cout << "\n";

  // Print the start and end times
  cout << startTimes[0];
  for (size_t i = 0; i < endTimes.size(); i++)
  {
    int spaces = 5 - to_string(endTimes[i]).length();
    for (int j = 0; j <= spaces; j++)
      cout << " ";
    cout << endTimes[i];
  }
  cout << "\n\n\n";
}

// main driver function.
void SJFP(vector<Process> &p, int n, vector<int> &gantt, vector<int> &startTimes, vector<int> &endTimes)
{
  completionTime(p, n, gantt, startTimes, endTimes);
  turnaroundTime(p, n);
  waitingTime(p, n);
}

// function to take user input for arrival and burst time.
void input(vector<Process> &Processes, int n)
{
  for (int i = 0; i < n; i++)
  {
    Process p;
    p.id = i + 1;
    cout << "Enter arrival and burst time for process " << p.id << ": ";
    cin >> p.arrival >> p.burst;
    p.remaining = p.burst;
    Processes.push_back(p);
  }
}

int main()
{
  int n;
  cout << "Enter number of processes: ";
  cin >> n;
  vector<Process> Processes;
  vector<int> gantt, startTimes, endTimes;
  input(Processes, n);
  SJFP(Processes, n, gantt, startTimes, endTimes);
  print(Processes, n);
  printGanttChart(gantt, startTimes, endTimes);
  return 0;
}
