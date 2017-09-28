//
// Created by eugene on 28.9.2017
//
#pragma once

#include <condition_variable>
#include <mutex>
#include <memory>
#include <atomic>

struct shared_state_base {
    shared_state_base()
        : _is_promise_alive(true)
        , _is_ready(false)
    {}

    virtual ~shared_state_base() {}

    bool valid() const {
        return _is_promise_alive;
    }

    void set_promise_died() {
        _is_promise_alive = false;
    }

    bool is_ready() const {
        return _is_ready;
    }

    void wait() {
        std::unique_lock<std::mutex> lock(_mutex);

        if (!_is_promise_alive && !is_ready()) {
            throw std::runtime_error("promise has already died");
        }

        _cond_var.wait(lock, [this] { return bool(_is_ready); });
    }

protected:
    std::atomic_bool _is_promise_alive;
    std::atomic_bool _is_ready;
    std::condition_variable _cond_var;
    std::mutex _mutex;
};

template<typename T>
struct shared_state
    : shared_state_base
{
    void set(T const & value) {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _value = value;
        }

        _is_ready = true;
        _cond_var.notify_one();
    }

    void set(T&& value) {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _value = std::move(value);
        }

        _is_ready = true;
        _cond_var.notify_one();
    }

    T const & get() {
        wait();
        return _value;
    }

private:
    T _value;
};

template<typename T>
struct shared_state<T&>
    : shared_state_base
{
    shared_state()
        : _value(nullptr)
    {}

    void set(T& value) {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _value = &value;
        }

        _is_ready = true;
        _cond_var.notify_one();
    }

    T& get() {
        wait();
        return *_value;
    }

private:
    T * _value;
};

template<>
struct shared_state<void>
    : shared_state_base
{
    void set() {
        _is_ready = true;
        _cond_var.notify_one();
    }

    void get() {
        wait();
    }
};
