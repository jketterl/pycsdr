#include "convert.hpp"
#include "types.hpp"

#include <csdr/converter.hpp>

static int Convert_init(Convert* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "inFormat", (char*) "outFormat", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!O!", kwlist, FORMAT_TYPE, &self->inputFormat, FORMAT_TYPE, &self->outputFormat)) {
        return -1;
    }

    // this matrix should be extended in sync with what's available in csdr
    if (self->inputFormat == FORMAT_FLOAT) {
        if (self->outputFormat == FORMAT_SHORT) {
            self->setModule(new Csdr::Converter<float, short>());
        } else {
            PyErr_SetString(PyExc_ValueError, "unsupported conversion");
            return -1;
        }
    } else if (self->inputFormat == FORMAT_SHORT) {
        if (self->outputFormat == FORMAT_FLOAT) {
            self->setModule(new Csdr::Converter<short, float>());
        } else {
            PyErr_SetString(PyExc_ValueError, "unsupported conversion");
            return -1;
        }
    } else if (self->inputFormat == FORMAT_COMPLEX_FLOAT) {
        if (self->outputFormat == FORMAT_COMPLEX_SHORT) {
            self->setModule(new Csdr::Converter<Csdr::complex<float>, Csdr::complex<short>>());
        } else if (self->outputFormat == FORMAT_COMPLEX_CHAR) {
            self->setModule(new Csdr::Converter<Csdr::complex<float>, Csdr::complex<unsigned char>>());
        } else {
            PyErr_SetString(PyExc_ValueError, "unsupported conversion");
            return -1;
        }
    } else if (self->inputFormat == FORMAT_COMPLEX_SHORT) {
        if (self->outputFormat == FORMAT_COMPLEX_FLOAT) {
            self->setModule(new Csdr::Converter<Csdr::complex<short>, Csdr::complex<float>>());
        } else {
            PyErr_SetString(PyExc_ValueError, "unsupported conversion");
            return -1;
        }
    } else if (self->inputFormat == FORMAT_COMPLEX_CHAR) {
        if (self->outputFormat == FORMAT_COMPLEX_FLOAT) {
            self->setModule(new Csdr::Converter<Csdr::complex<unsigned char>, Csdr::complex<float>>());
        } else {
            PyErr_SetString(PyExc_ValueError, "unsupported conversion");
            return -1;
        }
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported conversion");
        return -1;
    }

    Py_INCREF(self->inputFormat);
    Py_INCREF(self->outputFormat);

    return 0;
}

static PyType_Slot ConvertSlots[] = {
    {Py_tp_init, (void*) Convert_init},
    {0, 0}
};

PyType_Spec ConvertSpec = {
    "pycsdr.modules.Convert",
    sizeof(Convert),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    ConvertSlots
};

