//
// Created by eugene on 21.9.2017
//
#pragma once

#include "shared_state.h"

#include <memory>
#include <utility>

template<typename T>
class Promise;

template<typename T>
class Future {
public:
    Future(Future && other)
        : _shared_state(std::move(other._shared_state))
    {}

    T const & Get() const {
        return _shared_state->get();
    }

    bool IsReady() const {
        return _shared_state->is_ready();
    }

    void Wait() {
        _shared_state->wait();
    }

private:
    Future(std::shared_ptr<shared_state<T>> shared_state)
        : _shared_state(shared_state)
    {}

    friend class Promise<T>;
private:
    std::shared_ptr<shared_state<T>> _shared_state;
};


template<typename T>
class Future<T&> {
public:
    Future(Future && other)
        : _shared_state(std::move(other._shared_state))
    {}

    T& Get() const {
        return _shared_state->get();
    }

    bool IsReady() const {
        return bool(_shared_state->is_ready());
    }

    void Wait() {
        _shared_state->wait();
    }


private:
    Future(std::shared_ptr<shared_state<T&>> shared_state)
        : _shared_state(shared_state)
    {}

    friend class Promise<T&>;
private:
    std::shared_ptr<shared_state<T&>> _shared_state;
};

template<>
class Future<void> {
public:
    Future(Future && other)
        : _shared_state(std::move(other._shared_state))
    {}

    void Get() const {
        _shared_state->get();
    }

    bool IsReady() const {
        return bool(_shared_state->is_ready());
    }

    void Wait() {
        _shared_state->wait();
    }

private:
    Future(std::shared_ptr<shared_state<void>> shared_state)
        : _shared_state(shared_state)
    {}

    friend class Promise<void>;
private:
    std::shared_ptr<shared_state<void>> _shared_state;
};
