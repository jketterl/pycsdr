#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdbool.h>
#include <pthread.h>
#include <libcsdr.h>

#include "api.h"
#include "buffer.h"

// we expect 32bit float IQ samples coming in on the socket
#define SOCKET_ITEM_SIZE 8

typedef struct {
    PyObject base1;
    PyObject base2;
    int port;
    int socket;
    Buffer* outputBuffer;
    bool run;
    pthread_t worker;
} SocketClient;

extern PyType_Spec SocketClientSpec;