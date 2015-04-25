#pragma once

#include "Python.h"

#define PYDROPTOPDB(x) PyObject_Call(vpdb::pydebug_fn, Py_BuildValue("(O)", x), NULL)
#define PY_ERR_CHK if (PyErr_Occurred()) return NULL

namespace vpdb {
	extern PyObject* pydebug_fn;
}