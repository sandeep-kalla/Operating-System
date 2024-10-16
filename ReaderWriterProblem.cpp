#include <iostream>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
#include <vector>

std::binary_semaphore rw_mutex(1);  // Semaphore for ensuring mutual exclusion for the writers.
std::binary_semaphore mutex(1);     // Semaphore for controlling access to the reader count.
int read_count = 0;                 // Counter for tracking the number of active readers.
int shared_data = 0;                // Shared data resource.

void reader(int reader_id) {
    while (true) {
        // Entry section for readers.
        mutex.acquire();  // Ensure that only one reader at a time modifies read_count.
        read_count++;
        if (read_count == 1) {
            rw_mutex.acquire();  // If this is the first reader, lock the resource for writing.
        }
        mutex.release();

        // Critical section for reading.
        std::cout << "Reader " << reader_id << " is reading the data: " << shared_data << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));  // Simulate read time.

        // Exit section for readers.
        mutex.acquire();
        read_count--;
        if (read_count == 0) {
            rw_mutex.release();  // If this is the last reader, release the write lock.
        }
        mutex.release();

        std::this_thread::sleep_for(std::chrono::seconds(1));  // Simulate delay before the next read.
    }
}

void writer(int writer_id) {
    while (true) {
        rw_mutex.acquire();  // Ensure mutual exclusion for the writer.

        // Critical section for writing.
        shared_data++;
        std::cout << "Writer " << writer_id << " is writing the data: " << shared_data << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));  // Simulate write time.

        rw_mutex.release();  // Release the lock after writing.

        std::this_thread::sleep_for(std::chrono::seconds(2));  // Simulate delay before the next write.
    }
}

int main() {
    // Create threads for readers and writers.
    std::vector<std::thread> readers, writers;
    
    // Launch 5 readers.
    for (int i = 0; i < 5; ++i) {
        readers.push_back(std::thread(reader, i + 1));
    }

    // Launch 2 writers.
    for (int i = 0; i < 2; ++i) {
        writers.push_back(std::thread(writer, i + 1));
    }

    // Join all threads.
    for (auto& th : readers) {
        th.join();
    }
    for (auto& th : writers) {
        th.join();
    }

    return 0;
}
