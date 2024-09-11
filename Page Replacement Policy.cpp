#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>

using namespace std;

// Function to check if a page is present in memory
bool isPagePresent(const vector<int>& memory, int page) {
    return find(memory.begin(), memory.end(), page) != memory.end();
}

// LRU Page Replacement
void LRU(vector<int> pages, int capacity) {
    unordered_map<int, int> recent;
    vector<int> memory;
    int page_faults = 0;

    for (int i = 0; i < pages.size(); i++) {
        if (!isPagePresent(memory, pages[i])) {
            if (memory.size() == capacity) {
                // Find the least recently used page
                int lru = i;
                int page_to_remove;
                for (int page : memory) {
                    if (recent[page] < lru) {
                        lru = recent[page];
                        page_to_remove = page;
                    }
                }
                // Remove the LRU page
                memory.erase(remove(memory.begin(), memory.end(), page_to_remove), memory.end());
            }
            memory.push_back(pages[i]);
            page_faults++;
        }
        // Update recent usage
        recent[pages[i]] = i;
        cout << "Step " << i + 1 << ": ";
        for (int page : memory) cout << page << " ";
        cout << endl;
    }
    cout << "LRU Page Faults: " << page_faults << endl;
}

// FIFO Page Replacement
void FIFO(vector<int> pages, int capacity) {
    queue<int> fifoQueue;
    unordered_map<int, bool> inMemory;
    int page_faults = 0;

    for (int i = 0; i < pages.size(); i++) {
        if (!inMemory[pages[i]]) {
            if (fifoQueue.size() == capacity) {
                int page_to_remove = fifoQueue.front();
                fifoQueue.pop();
                inMemory[page_to_remove] = false;
            }
            fifoQueue.push(pages[i]);
            inMemory[pages[i]] = true;
            page_faults++;
        }
        cout << "Step " << i + 1 << ": ";
        queue<int> temp = fifoQueue;
        while (!temp.empty()) {
            cout << temp.front() << " ";
            temp.pop();
        }
        cout << endl;
    }
    cout << "FIFO Page Faults: " << page_faults << endl;
}

// Optimal Page Replacement
void Optimal(vector<int> pages, int capacity) {
    vector<int> memory;
    int page_faults = 0;

    for (int i = 0; i < pages.size(); i++) {
        if (!isPagePresent(memory, pages[i])) {
            if (memory.size() == capacity) {
                // Find the page to remove based on future use
                int farthest = -1;
                int page_to_remove;
                for (int page : memory) {
                    auto it = find(pages.begin() + i + 1, pages.end(), page);
                    if (it == pages.end()) {
                        page_to_remove = page;
                        break;
                    } else {
                        int idx = distance(pages.begin(), it);
                        if (idx > farthest) {
                            farthest = idx;
                            page_to_remove = page;
                        }
                    }
                }
                memory.erase(remove(memory.begin(), memory.end(), page_to_remove), memory.end());
            }
            memory.push_back(pages[i]);
            page_faults++;
        }
        cout << "Step " << i + 1 << ": ";
        for (int page : memory) cout << page << " ";
        cout << endl;
    }
    cout << "Optimal Page Faults: " << page_faults << endl;
}

int main() {
    vector<int> pages = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3}; // Example reference string
    int capacity = 3;

    cout << "Page Replacement using LRU:\n";
    LRU(pages, capacity);

    cout << "\nPage Replacement using FIFO:\n";
    FIFO(pages, capacity);

    cout << "\nPage Replacement using Optimal:\n";
    Optimal(pages, capacity);

    return 0;
}
