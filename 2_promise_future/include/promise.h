//
// Created by eugene on 21.9.2017
//
#pragma once

#include <exception>
#include <memory>

#include "future.h"
#include "shared_state.h"

template<typename T>
class Promise {
public:
    Promise() {
        _shared_state = std::make_shared<shared_state<T>>();
    }

    Promise(Promise<T>&& promise)
        : _shared_state(std::move(promise._shared_state))
    {}

    ~Promise() {
        if (_shared_state) {
            _shared_state->set_promise_died();
        }
    }

    Future<T> GetFuture() {
        return Future<T>(_shared_state);
    }

    void Set(T const & value) {
        _shared_state->set(value);
    }

    void Set(T&& value) {
        _shared_state->set(std::move(value));
    }

    void SetException(std::exception_ptr &);

private:
    std::shared_ptr<shared_state<T>> _shared_state;
};

template<typename T>
class Promise<T&> {
public:
    Promise() {
        _shared_state = std::make_shared<shared_state<T&>>();
    }

    Promise(Promise<T&>&& other)
        : _shared_state(std::move(other._shared_state))
    {}

    ~Promise() {
        if (_shared_state) {
            _shared_state->set_promise_died();
        }
    }

    Future<T&> GetFuture() {
        return Future<T&>(_shared_state);
    }

    void Set(T& value) {
        _shared_state->set(value);
    }

    void SetException(std::exception_ptr &);

private:
    std::shared_ptr<shared_state<T&>> _shared_state;
};

template<>
class Promise<void> {
public:
    Promise() {
        _shared_state = std::make_shared<shared_state<void>>();
    }

    Promise(Promise<void>&& other)
        : _shared_state(std::move(other._shared_state))
    {}

    ~Promise() {
        if (_shared_state) {
            _shared_state->set_promise_died();
        }
    }

    Future<void> GetFuture() {
        return Future<void>(_shared_state);
    }

    void Set() {
        _shared_state->set();
    }

    void SetException(std::exception_ptr &);

private:
    std::shared_ptr<shared_state<void>> _shared_state;
};
