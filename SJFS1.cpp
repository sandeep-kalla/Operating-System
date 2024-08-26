#include <bits/stdc++.h>
using namespace std;

struct Process
{
  int id;
  int arrival;
  int burst;
  int completion;
  int waiting;
  int turnaround;
};

void completionTime(vector<Process> &p, int n, vector<int> &gantt, vector<int> &startTimes, vector<int> &endTimes)
{
  int currentTime = 0, completed = 0;
  vector<bool> isCompleted(n, false);

  while (completed < n)
  {
    int idx = -1, minBurst = INT_MAX;

    for (int i = 0; i < n; i++)
    {
      if (p[i].arrival <= currentTime && !isCompleted[i] && p[i].burst < minBurst)
      {
        minBurst = p[i].burst;
        idx = i;
      }
    }

    if (idx != -1)
    {
      startTimes.push_back(currentTime);
      currentTime += p[idx].burst;
      p[idx].completion = currentTime;
      endTimes.push_back(currentTime);
      isCompleted[idx] = true;
      gantt.push_back(p[idx].id);
      completed++;
    }
    else
    {
      currentTime++;
    }
  }
}

void turnaroundTime(vector<Process> &p, int n)
{
  for (int i = 0; i < n; i++)
  {
    p[i].turnaround = p[i].completion - p[i].arrival;
  }
}

void waitingTime(vector<Process> &p, int n)
{
  for (int i = 0; i < n; i++)
  {
    p[i].waiting = p[i].turnaround - p[i].burst;
  }
}

void print(vector<Process> &p, int n)
{
  cout << "SJF Non-Preemptive Scheduling:\n";
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

void printGanttChart(const vector<int> &gantt, const vector<int> &startTimes, const vector<int> &endTimes)
{
  cout << "Gantt Chart:\n";

  // Print the top line
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

  // Print the bottom line
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

void SJFN(vector<Process> &p, int n, vector<int> &gantt, vector<int> &startTimes, vector<int> &endTimes)
{
  completionTime(p, n, gantt, startTimes, endTimes);
  turnaroundTime(p, n);
  waitingTime(p, n);
}

void input(vector<Process> &Processes, int n)
{
  for (int i = 0; i < n; i++)
  {
    Process p;
    p.id = i + 1;
    cout << "Enter arrival and burst time for process " << p.id << ": ";
    cin >> p.arrival >> p.burst;
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
  SJFN(Processes, n, gantt, startTimes, endTimes);
  print(Processes, n);
  printGanttChart(gantt, startTimes, endTimes);
  return 0;
}
