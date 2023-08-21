#include "module.hpp"
#include "types.hpp"
#include "buffer.hpp"
#include <csdr/ringbuffer.hpp>

static void stopRunner(Module* self) {
    Csdr::AsyncRunner* old;
    {
        std::lock_guard<std::mutex> lock(self->runnerMutex);
        if (self->runner == nullptr) return;

        old = self->runner;
        self->runner = nullptr;
    }

    // avoid deadlocks
    // other threads may need the GIL for whatever they do before they shut down
    Py_BEGIN_ALLOW_THREADS
    old->stop();
    delete old;
    Py_END_ALLOW_THREADS
}

static PyObject* checkRunner(Module* self) {
    if (self->reader != nullptr && self->writer != nullptr) {
        std::lock_guard<std::mutex> lock(self->runnerMutex);
        if (self->runner == nullptr || !self->runner->isRunning()) {
            delete self->runner;
            self->runner = new Csdr::AsyncRunner(self->module);
        }
    } else {
        stopRunner(self);
    }
    Py_RETURN_NONE;
}

static PyObject* Module_setReader(Module* self, PyObject* args, PyObject* kwds) {
    if (Sink_setReader((Sink*) self, args, kwds) == NULL) {
        return NULL;
    }

    return checkRunner(self);
}

static PyObject* Module_setWriter(Module* self, PyObject* args, PyObject* kwds) {
    if (Source_setWriter((Source*) self, args, kwds) == NULL) {
        return NULL;
    }

    return checkRunner(self);
}

static PyObject* Module_stop(Module* self) {
    stopRunner(self);
    Py_RETURN_NONE;
}

static int Module_finalize(Module* self) {
    stopRunner(self);

    auto old = self->module;
    self->setModule(nullptr);

    // deletion of a module may perform blocking I/O
    Py_BEGIN_ALLOW_THREADS
    delete old;
    Py_END_ALLOW_THREADS

    if (Sink_finalize(self) != 0) {
        return -1;
    }

    if (Source_finalize(self) != 0) {
        return -1;
    }

    return 0;
}

static PyObject* Module_getOutputFormat(Module* self) {
    Py_INCREF(self->outputFormat);
    return self->outputFormat;
}

static PyObject* Module_getInputFormat(Module* self) {
    Py_INCREF(self->inputFormat);
    return self->inputFormat;
}

static PyMethodDef Module_methods[] = {
    {"setReader", (PyCFunction) Module_setReader, METH_VARARGS | METH_KEYWORDS,
     "set the reader to read data from"
    },
    {"setWriter", (PyCFunction) Module_setWriter, METH_VARARGS | METH_KEYWORDS,
     "set the writer to write data to"
    },
    {"stop", (PyCFunction) Module_stop, METH_NOARGS,
     "stop module processing"
    },
    {"getOutputFormat", (PyCFunction) Module_getOutputFormat, METH_NOARGS,
     "get output format"
    },
    {"getInputFormat", (PyCFunction) Module_getInputFormat, METH_NOARGS,
     "get input format"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot ModuleSlots[] = {
    {Py_tp_finalize, (void*) Module_finalize},
    {Py_tp_methods, Module_methods},
    {0, 0}
};

PyType_Spec ModuleSpec = {
    "pycsdr.modules.Module",
    sizeof(Module),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_FINALIZE,
    ModuleSlots
};
