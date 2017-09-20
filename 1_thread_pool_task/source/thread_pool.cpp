//
// Created by eugene on 14.9.2017
//

#include "thread_pool.h"

thread_pool::thread_pool(int32_t num_threads)
    : _finished(false)
{
    for (int32_t i = 0; i < num_threads; ++i) {
        _threads.emplace_back([this] {
            while (!_finished) {
                task_t task;

                {
                    std::unique_lock<std::mutex> ul(_mutex);
                    _cond_variable.wait(ul, [this]{ return !_tasks.empty() || _finished; });

                    if (_finished) {
                        return;
                    }

                    task = _tasks.front();
                    _tasks.pop();
                }

                task();
            }
        });
    }
}

thread_pool::~thread_pool()
{
    _finished = true;
    _cond_variable.notify_all();
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
