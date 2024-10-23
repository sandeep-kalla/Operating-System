#include <iostream>
#include <vector>
using namespace std;

class Banker {
    int p, r;
    vector<vector<int>> max, alloc, need;
    vector<int> avail;
public:
    Banker(int np, int nr) : p(np), r(nr), max(np, vector<int>(nr)), 
        alloc(np, vector<int>(nr)), need(np, vector<int>(nr)), avail(nr) {}
    
    void setup(vector<vector<int>>& m, vector<vector<int>>& a, vector<int>& v) {
        max = m; alloc = a; avail = v;
        for(int i = 0; i < p; i++)
            for(int j = 0; j < r; j++)
                need[i][j] = max[i][j] - alloc[i][j];
    }
    
    bool isSafe() {
        vector<bool> fin(p);
        vector<int> work = avail;
        int count = 0;
        
        while(count < p) {
            bool found = false;
            for(int i = 0; i < p && !found; i++) {
                if(!fin[i]) {
                    int j;
                    for(j = 0; j < r && need[i][j] <= work[j]; j++);
                    if(j == r) {
                        for(j = 0; j < r; j++) work[j] += alloc[i][j];
                        fin[i] = found = true;
                        count++;
                    }
                }
            }
            if(!found) return false;
        }
        return true;
    }
    
    bool request(int pid, vector<int>& req) {
        for(int i = 0; i < r; i++)
            if(req[i] > need[pid][i] || req[i] > avail[i]) return false;
            
        vector<int> temp_avail = avail;
        vector<vector<int>> temp_alloc = alloc, temp_need = need;
        
        for(int i = 0; i < r; i++) {
            avail[i] -= req[i];
            alloc[pid][i] += req[i];
            need[pid][i] -= req[i];
        }
        
        if(isSafe()) return true;
        
        avail = temp_avail;
        alloc = temp_alloc;
        need = temp_need;
        return false;
    }
};
