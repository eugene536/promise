//
// Created by eugene on 11.10.2017
//
#pragma once

#include <future.h>
#include <promise.h>

template<typename T>
Future<T> Flatten(Future<T> const &);

template<typename T>
Future<T> Flatten(Future<Future<T>> const &);

template<template<typename> class C, typename T>
Future<C<T>> Flatten(C<Future<T>> const &);
