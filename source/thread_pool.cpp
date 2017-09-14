//
// Created by eugene on 14.9.2017
//

#include "thread_pool.h"

#include <mutex>

thread_pool::thread_pool(int32_t num_threads)
    : _num_threads(num_threads)
{
    for (int32_t i = 0; i < _num_threads; ++i) {
        _threads.emplace_back([this]() {
            task_t task;
            {
                std::unique_lock<std::mutex> ul(_mutex);
                _cond_variable.wait(ul, [this]{ return !_tasks.empty(); });

                task = _tasks.front();
                _tasks.pop();
            }

            task();
        });
    }
}

thread_pool::~thread_pool()
{
    for (std::thread & t: _threads) {
        t.join();
    }
}

void thread_pool::execute(const thread_pool::task_t &task)
{
    std::unique_lock<std::mutex> ul(_mutex);
    _tasks.push(task);
    _cond_variable.notify_one();
}
