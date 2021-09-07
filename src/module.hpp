#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <csdr/module.hpp>
#include <csdr/async.hpp>
#include <mutex>

#include "buffer.hpp"
#include "sink.hpp"
#include "source.hpp"

struct Module: Sink, Source {
    Csdr::UntypedModule* module;
    Csdr::AsyncRunner* runner;
    std::mutex runnerMutex;

    void setModule(Csdr::UntypedModule* module) {
        this->module = module;
        this->source = dynamic_cast<Csdr::UntypedSource*>(module);
        this->sink = dynamic_cast<Csdr::UntypedSink*>(module);
    }
};

extern PyType_Spec ModuleSpec;