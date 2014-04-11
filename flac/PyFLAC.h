/*
 *	This file is part of python-flac.
 *
 *	Copyright (c) 2014 Christian Schmitz <tynn.dev@gmail.com>
 *
 *	python-flac is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	python-flac is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public License
 *	along with python-flac.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __PyFLAC_h__
#define __PyFLAC_h__

#include <Python.h>


#if PY_MAJOR_VERSION >= 3
	#define __PyFLAC3__

	#define PyInt_FromLong PyLong_FromLong

	#define PyStringObject PyUnicodeObject
	#define PyString_Type PyUnicode_Type
	#define PyString_GET_SIZE PyUnicode_GET_LENGTH
	#define PyString_FromFormat PyUnicode_FromFormat
	#define PyString_FromString PyUnicode_FromString

	#define PyFLAC_data_format "y"

#else

	#define PyFLAC_data_format "s"

#endif


#define PyFLAC_RAISE_NotImplementedError { \
	PyErr_SetString(PyExc_NotImplementedError, "someone should have implemented this"); \
	return NULL; \
}

#define PyFLAC_RETURN_bool(bool_value) if (bool_value) { Py_RETURN_TRUE; } else { Py_RETURN_FALSE; }

#define PyFLAC_RETURN_bool_OR_RAISE(bool_value) \
	if (bool_value) { if (PyErr_Occurred()) { return NULL; } else { Py_RETURN_TRUE; } } \
	else { if (PyErr_Occurred()) { return NULL; } else { Py_RETURN_FALSE; } }

#define PyFLAC_RETURN_int(int_value) return Py_BuildValue("i", int_value);

#define PyFLAC_RETURN_string(string_value) return Py_BuildValue("s", string_value);

#define PyFLAC_RETURN_string2(string_value,string_length) return Py_BuildValue("s#", string_value, string_length);

#define PyFLAC_RETURN_data(data,length) return Py_BuildValue(PyFLAC_data_format "#", data, length);

#define PyFLAC_RETURN_uint64(uint64_value) return Py_BuildValue("K", uint64_value);

#define PyFLAC_RETURN_uint32(uint32_value) return Py_BuildValue("k", uint32_value);

#define PyFLAC_RETURN_unsigned(unsigned_value) return Py_BuildValue("I", unsigned_value);

#define PyFLAC_CHECK_status(status) if (status < 0) { return -1; }

#define PyFLAC_type(object) &flac_##object##Type

#define PyFLAC_type_Check(object,type) PyFLAC_##type##_Check(object)

#define PyFLAC_type_Check_use(type) int PyFLAC_type_Check(PyObject *object,type)

#define PyFLAC_type_Check_function(type) \
PyFLAC_type_Check_use(type) \
{ \
	return PyObject_TypeCheck(object, PyFLAC_type(type)); \
}


#if PY_MAJOR_VERSION >= 3

	#define PyFLAC_PyModule_Prepare(name,functions,doc) \
	static struct PyModuleDef moduledef = { \
		PyModuleDef_HEAD_INIT, \
		#name,					/* m_name */ \
		doc,					/* m_doc */ \
		-1,						/* m_size */ \
		functions,				/* m_methods */ \
		NULL,					/* m_reload */ \
		NULL,					/* m_traverse */ \
		NULL,					/* m_clear */ \
		NULL,					/* m_free */ \
	};
	#define PyFLAC_PyModule_Init(name) PyInit_##name
	#define PyFLAC_PyModule_Create(name,functions,doc) PyModule_Create(&moduledef)
	#define PyFLAC_PyModule_RETURN return

#else

	#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
		#define PyMODINIT_FUNC void
	#endif

	#define PyFLAC_PyModule_Prepare(name,functions,doc)
	#define PyFLAC_PyModule_Init(name) init##name
	#define PyFLAC_PyModule_Create(name,functions,doc) Py_InitModule3(#name, functions, doc)
	#define PyFLAC_PyModule_RETURN (void)

#endif


#define PyFLAC_MODINIT(name,initfunction,functions,doc) \
	PyFLAC_PyModule_Prepare(name,functions,doc) \
	PyMODINIT_FUNC \
	PyFLAC_PyModule_Init(name) ( void ) \
	{ \
		PyObject *module; \
		module = PyFLAC_PyModule_Create(name,functions,doc); \
		if (initfunction(module) < 0) { \
			if (!PyErr_Occurred()) \
				PyErr_SetString(PyExc_SystemError, "error return without exception set"); \
			Py_DECREF(module); \
			module = NULL; \
		} \
		PyFLAC_PyModule_RETURN module; \
	}


#endif // __PyFLAC_h__
