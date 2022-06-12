#pragma once

#include <vector>
#include <thread>
#include <condition_variable>
#include <queue>
#include <functional>


class ThreadPool {
public:
    void Start();
    void QueueJob(const std::function<void()>& job);
    void Stop();
    void busy();

private:
    void ThreadLoop();

    bool should_terminate = false;           // Tells threads to stop looking for jobs
    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination 
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;
};

void ThreadPool::ThreadLoop() {
    while(true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(lock, [this] {
                return !jobs.empty() || should_terminate;
                });
            if(should_terminate) {
                return;
            }
            job = jobs.front();
            jobs.pop();
        }
        job();
    }
}

void ThreadPool::Start() {
    const uint32_t num_threads = std::thread::hardware_concurrency(); // Max # of threads the system supports
    threads.resize(num_threads);
    for(uint32_t i = 0; i < num_threads; i++) {
        threads.at(i) = std::thread(ThreadLoop);
    }
}

void ThreadPool::ThreadLoop() {
    while(true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(lock, [this] {
                return !jobs.empty() || should_terminate;
                });
            if(should_terminate) {
                return;
            }
            job = jobs.front();
            jobs.pop();
        }
        job();
    }
}

void ThreadPool::busy() {
    bool poolbusy;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        poolbusy = jobs.empty();
    }
    return poolbusy;
}


