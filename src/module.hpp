#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <csdr/module.hpp>
#include <csdr/async.hpp>

#include "buffer.hpp"
#include "sink.hpp"
#include "source.hpp"

struct Module: Sink, Source {
    Csdr::UntypedModule* module;
    Csdr::AsyncRunner* runner;

    void setModule(Csdr::UntypedModule* module);
};

extern PyType_Spec ModuleSpec;