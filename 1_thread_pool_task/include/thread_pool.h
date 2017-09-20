//
// Created by eugene on 14.9.2017
//
#pragma once

#include <functional>
#include <cstdint>
#include <vector>
#include <queue>

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

class thread_pool {
public:
    typedef std::function<void()> task_t;

    thread_pool(int32_t num_threads);
    ~thread_pool();

    void execute(task_t const & task);

private:
    std::vector<std::thread> _threads;
    std::condition_variable _cond_variable;
    std::mutex _mutex;
    std::atomic_bool _finished;

    std::queue<task_t> _tasks;
};
