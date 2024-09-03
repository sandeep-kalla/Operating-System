#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <algorithm>

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

struct Result
{
  double avgTurnaround;
  double avgWaiting;
};

void input(vector<Process> &processes, int n)
{
  for (int i = 0; i < n; i++)
  {
    Process p;
    p.id = i + 1;
    cout << "Enter arrival time and burst time for process " << p.id << ": ";
    cin >> p.arrival >> p.burst;
    p.remaining = p.burst;
    processes.push_back(p);
  }
}

void completionTime(vector<Process> &processes, int n, int time_quantum, vector<pair<int, int>> &ganttChart)
{
  queue<int> readyQueue;
  int currentTime = 0;
  int completed = 0;
  vector<bool> inQueue(n, false);

  sort(processes.begin(), processes.end(), [](const Process &a, const Process &b)
       { return a.arrival < b.arrival; });

  readyQueue.push(0);
  inQueue[0] = true;

  while (completed < n)
  {
    if (!readyQueue.empty())
    {
      int index = readyQueue.front();
      readyQueue.pop();

      if (processes[index].remaining > 0)
      {
        ganttChart.push_back({processes[index].id, currentTime});
      }

      if (processes[index].remaining > time_quantum)
      {
        currentTime += time_quantum;
        processes[index].remaining -= time_quantum;
      }
      else
      {
        currentTime += processes[index].remaining;
        processes[index].completion = currentTime;
        processes[index].remaining = 0;
        completed++;
      }

      // Update the Gantt chart with the current time
      ganttChart.back().second = currentTime;

      for (int i = 0; i < n; i++)
      {
        if (processes[i].arrival <= currentTime && processes[i].remaining > 0 && !inQueue[i])
        {
          readyQueue.push(i);
          inQueue[i] = true;
        }
      }

      if (processes[index].remaining > 0)
      {
        readyQueue.push(index);
        inQueue[index] = true;
      }
      else
      {
        inQueue[index] = false;
      }
    }
    else
    {
      currentTime++;
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
  cout << "Round Robin Scheduling:" << endl;
  cout << "+----+--------------+--------------+-------------------+-------------------+--------------+" << endl;
  cout << "| PID| Arrival Time |  Burst Time  |  Completion Time  |  TurnAround Time  | Waiting Time |" << endl;
  cout << "+----+--------------+--------------+-------------------+-------------------+--------------+" << endl;

  for (int i = 0; i < n; i++)
  {
    cout << "| " << setw(2) << processes[i].id << " | "
         << setw(12) << processes[i].arrival << " | "
         << setw(12) << processes[i].burst << " | "
         << setw(17) << processes[i].completion << " | "
         << setw(17) << processes[i].turnaround << " | "
         << setw(12) << processes[i].waiting << " |" << endl;
    cout << "+----+--------------+--------------+-------------------+-------------------+--------------+" << endl;
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

Result RoundRobin(vector<Process> &processes, int n, int time_quantum)
{
  vector<pair<int, int>> ganttChart;

  completionTime(processes, n, time_quantum, ganttChart);
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

  cout << "\nTime Quantum: " << time_quantum << endl;
  cout << "Average Turnaround Time: " << fixed << setprecision(2) << avgTurnaround << endl;
  cout << "Average Waiting Time: " << fixed << setprecision(2) << avgWaiting << endl;

  return {avgTurnaround, avgWaiting};
}

int main()
{
  int n;
  cout << "Enter number of processes: ";
  cin >> n;

  vector<Process> original_processes;
  input(original_processes, n);

  vector<int> time_quanta;
  cout << "Enter time quanta (space-separated, end with 0): ";
  int q;
  while (cin >> q && q != 0)
  {
    time_quanta.push_back(q);
  }

  vector<Result> results;

  for (int time_quantum : time_quanta)
  {
    cout << "\n\n--- Round Robin Scheduling (Time Quantum: " << time_quantum << ") ---\n";
    vector<Process> processes = original_processes; // Create a copy for each run
    results.push_back(RoundRobin(processes, n, time_quantum));
  }

  cout << "\n\nComparison of Average Times:\n";
  cout << "+---------------+------------------------+---------------------+\n";
  cout << "| Time Quantum  | Avg Turnaround Time    | Avg Waiting Time    |\n";
  cout << "+---------------+------------------------+---------------------+\n";
  for (size_t i = 0; i < time_quanta.size(); ++i)
  {
    cout << "| " << setw(13) << time_quanta[i] << " | "
         << setw(22) << fixed << setprecision(2) << results[i].avgTurnaround << " | "
         << setw(19) << fixed << setprecision(2) << results[i].avgWaiting << " |\n";
  }
  cout << "+---------------+------------------------+---------------------+\n";

  return 0;
}
