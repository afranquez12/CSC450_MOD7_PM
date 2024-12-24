//============================================================================
// Name        : CSC450_MOD7_PM.cpp
// Author      : Abnel Franquez
// Version     :
// Copyright   : CSU_Global_Educational
// Description : Concurrency in c++
//============================================================================

#include <iostream>                // For std::cout
#include <thread>                  // For std::thread
#include <mutex>                   // For std::mutex and std::lock_guard
#include <condition_variable>      // For std::condition_variable
#include <chrono>                  // For std::this_thread::sleep_for

/*
 * Purpose:
 * This program demonstrates C++ concurrency concepts using two threads.
 * Thread 1 counts up to 20, while Thread 2 waits for Thread 1 to finish
 * and then counts down from 20 to 0. The program uses std::mutex and
 * std::condition_variable to synchronize the threads, ensuring no race
 * conditions or improper access to shared data.
 */

std::mutex mtx;                  // Mutex for thread synchronization
std::condition_variable cv;      // Condition variable for signaling between threads
bool threadOneFinished = false;  // Flag to indicate when Thread 1 finishes

// Function for Thread One: Count up to 20
void countUp() {
    {
        std::lock_guard<std::mutex> lock(mtx); // Lock mutex for thread-safe operation
        for (int i = 0; i <= 20; ++i) {
            std::cout << "Thread 1 - Count Up: " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
        }
        threadOneFinished = true; // Signal that Thread One is finished
    }
    cv.notify_one(); // Notify Thread Two to proceed
}

// Function for Thread Two: Count down from 20 to 0
void countDown() {
    std::unique_lock<std::mutex> lock(mtx); // Lock mutex with unique_lock
    cv.wait(lock, [] { return threadOneFinished; }); // Wait for Thread One to finish
    for (int i = 20; i >= 0; --i) {
        std::cout << "Thread 2 - Count Down: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
    }
}

int main() {
    // Create two threads
    std::thread thread1(countUp);
    std::thread thread2(countDown);

    // Wait for threads to finish
    thread1.join();
    thread2.join();

    return 0;
}
