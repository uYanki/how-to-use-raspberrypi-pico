#pragma once

#include <stdio.h>
#include "pico/stdlib.h"

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef unsigned int size_t;

#define println(fmt, ...) printf(fmt "\r\n", ##__VA_ARGS__)

#define BV(n)             (1u << (n))

typedef enum {
    LOW = 0,
    HIGH,
} pin_level_t;

typedef enum {
    OUT = 0,
    IN,
} pin_dir_t;

#define INLINE inline

#ifndef nullptr
#define nullptr ((void*)0)
#endif

#define __IN
#define __OUT

typedef enum {
    // ERROR
    _err_begin = -128,
    ERR_UNKOWUN,
    ERR_BUSY,
    ERR_TIMEOUT,
    _err_end,
    // SUCCESS
    SUCCESS = 0,
} err_t;

static inline bool IS_OK(err_t state) { return state == SUCCESS; }
static inline bool IS_ERR(err_t state) { return (_err_begin < (state)) && ((state) < _err_end); }

// static inline DelayBlockUS(uint32_t n) {}