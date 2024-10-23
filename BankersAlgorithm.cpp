#include <iostream>
#include <vector>
using namespace std;
class BankersAlgorithm {
    int numProcesses, numResources;
    vector<vector<int>> allocation, max, need;
    vector<int> available;
public:
    BankersAlgorithm(int p, int r)
        : numProcesses(p), numResources(r), allocation(p, vector<int>(r)),
          max(p, vector<int>(r)), need(p, vector<int>(r)), available(r) {}
    void inputResources() {
        cout << "Enter allocation matrix:\n";
        for (auto& row : allocation) for (auto& elem : row) cin >> elem;
        cout << "Enter maximum matrix:\n";
        for (auto& row : max) for (auto& elem : row) cin >> elem;
        cout << "Enter available resources:\n";
        for (auto& res : available) cin >> res;
        calculateNeed();
    }
    void calculateNeed() {
        for (int i = 0; i < numProcesses; ++i)
            for (int j = 0; j < numResources; ++j)
                need[i][j] = max[i][j] - allocation[i][j];
    }
    bool isSafe() {
        vector<bool> finish(numProcesses, false);
        vector<int> work = available, safeSequence;
        int count = 0;
        while (count < numProcesses) {
            bool found = false;
            for (int p = 0; p < numProcesses; ++p) {
                if (!finish[p] && canAllocate(p, work)) {
                    for (int r = 0; r < numResources; ++r) work[r] += allocation[p][r];
                    safeSequence.push_back(p);
                    finish[p] = true;
                    found = true;
                    count++;
                }
            }
            if (!found) return false;
        }
        cout << "System is in a safe state.\nSafe sequence is: ";
        for (int i : safeSequence) cout << i << " ";
        cout << endl;
        return true;
    }
private:
    bool canAllocate(int p, const vector<int>& work) const {
        for (int r = 0; r < numResources; ++r)
            if (need[p][r] > work[r]) return false;
        return true;
    }
};
int main() {
    int processes, resources;
    cout << "Enter number of processes and resources: ";
    cin >> processes >> resources;
    BankersAlgorithm ba(processes, resources);
    ba.inputResources();
    if (!ba.isSafe()) cout << "System is not in a safe state.\n";
    return 0;
}
