#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "writer.hpp"
#include <csdr/writer.hpp>
#include <functional>

struct CallbackWriter: Writer {};

extern PyType_Spec CallbackWriterSpec;

template <typename T>
class ProxyWriter: public Csdr::Writer<T> {
    public:
        explicit ProxyWriter(std::function<void(T* data, size_t size)> callback);
        ~ProxyWriter();
        size_t writeable() override;
        void advance(size_t how_much) override;
        T* getWritePointer() override;
    private:
        size_t bufferSize = 1024;
        std::function<void(T* data, size_t)> callback;
        T* buffer;
};