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

	#if PY_MAJOR_VERSION >= 4

		#define PyString_GET_SIZE PyUnicode_GET_LENGTH

	#else // PY_MAJOR_VERSION >= 4

		#define PyString_GET_SIZE PyUnicode_GET_SIZE

	#endif // PY_MAJOR_VERSION >= 4

	#define PyInt_FromLong PyLong_FromLong

	#define PyStringObject PyUnicodeObject
	#define PyString_Type PyUnicode_Type
	#define PyString_FromFormat PyUnicode_FromFormat
	#define PyString_FromString PyUnicode_FromString
	#define PyString_FromStringAndSize PyUnicode_FromStringAndSize

	#define PyFLAC_data_format "y"

	#define PyFLAC_unsigned(unsigned_value) PyLong_FromUnsignedLong((unsigned long) unsigned_value)

#else // PY_MAJOR_VERSION >= 3

	#define PyFLAC_data_format "s"

	#define PyFLAC_unsigned(unsigned_value) ((long) unsigned_value < 0 ? PyLong_FromUnsignedLong((unsigned long) unsigned_value) : PyFLAC_int(unsigned_value))

#endif // PY_MAJOR_VERSION >= 3


#define PyFLAC_RAISE_NotImplementedError { \
	PyErr_SetString(PyExc_NotImplementedError, "someone should have implemented this"); \
	return NULL; \
}
#define PyFLAC_setter_NotImplemented \
static int \
flac_setter_NotImplemented (PyObject *self, PyObject *value, void *closure) \
{ \
	PyErr_SetString(PyExc_NotImplementedError, "setter not implemented yet"); \
	return -1; \
}


#define PyFLAC_name(type) "flac." #type


#define PyFLAC_RuntimeError(msg) PyErr_SetString(PyExc_RuntimeError, msg)

#define PyFLAC_getter_error(type) { PyFLAC_RuntimeError("wrong use of getter of " PyFLAC_name(type)); return NULL; }

#define PyFLAC_setter_error(type) { PyFLAC_RuntimeError("wrong use of setter of " PyFLAC_name(type)); return -1; }


#define PyFLAC_int(int_value) PyInt_FromLong((long) int_value)

#define PyFLAC_uint64(uint64_value) PyLong_FromUnsignedLongLong((unsigned PY_LONG_LONG) uint64_value)

#define PyFLAC_uint32(uint32_value) PyFLAC_unsigned(uint32_value)

#define PyFLAC_uint8(uint8_value) PyFLAC_unsigned(uint8_value)

#define PyFLAC_byte(byte_value) PyFLAC_uint8(byte_value)

#define PyFLAC_RETURN_bool(bool_value) { if (bool_value) { Py_RETURN_TRUE; } else { Py_RETURN_FALSE; } }

#define PyFLAC_RETURN_bool_OR_RAISE(bool_value) { if (PyErr_Occurred()) { return NULL; } else PyFLAC_RETURN_bool(bool_value) }

#define PyFLAC_RETURN_int(int_value) return PyFLAC_int(int_value);

#define PyFLAC_RETURN_string(string_value) return PyString_FromString((char *) string_value);

#define PyFLAC_RETURN_string2(string_value,string_length) return PyString_FromStringAndSize((char *) string_value, (Py_ssize_t) string_length);

#define PyFLAC_RETURN_data(data,length) return Py_BuildValue(PyFLAC_data_format "#", data, length);

#define PyFLAC_RETURN_unsigned(unsigned_value) return PyFLAC_unsigned(unsigned_value);

#define PyFLAC_RETURN_uint64(uint64_value) return PyFLAC_uint64(uint64_value);

#define PyFLAC_RETURN_uint32(uint32_value) PyFLAC_RETURN_unsigned(uint32_value)

#define PyFLAC_RETURN_uint8(uint8_value) PyFLAC_RETURN_unsigned(uint8_value)

#define PyFLAC_RETURN_byte(byte_value) PyFLAC_RETURN_uint8(byte_value)

#define PyFLAC_CHECK_status(status) if (status < 0) { return -1; }

#define PyFLAC_type(type) &PyFLAC_##type##Type

#define PyFLAC_type_Check(object,type) PyObject_TypeCheck(object, PyFLAC_type(type))


#ifdef __PyFLAC3__

	#define PyFLAC_PyModule_Prepare(name,functions,doc) \
	static struct PyModuleDef moduledef = { \
		PyModuleDef_HEAD_INIT, \
		PyFLAC_name(name),		/* m_name */ \
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

#else // __PyFLAC3__

	#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
		#define PyMODINIT_FUNC void
	#endif

	#define PyFLAC_PyModule_Prepare(name,functions,doc)
	#define PyFLAC_PyModule_Init(name) init##name
	#define PyFLAC_PyModule_Create(name,functions,doc) Py_InitModule3(PyFLAC_name(name), functions, doc)
	#define PyFLAC_PyModule_RETURN (void)

#endif // __PyFLAC3__


#define PyFLAC_MODINIT(name,initfunction,buildfunction,functions,doc) \
PyFLAC_PyModule_Prepare(name,functions,doc) \
PyMODINIT_FUNC \
PyFLAC_PyModule_Init(name) ( void ) \
{ \
	PyObject *module; \
	if (initfunction() < 0) { \
		if (!PyErr_Occurred()) \
			PyErr_SetString(PyExc_SystemError, "error return without exception set"); \
		PyFLAC_PyModule_RETURN NULL; \
	} \
	else \
	{ \
		module = PyFLAC_PyModule_Create(name,functions,doc); \
		if (module != NULL) \
			buildfunction(module); \
		PyFLAC_PyModule_RETURN module; \
	} \
}


#define PyFLAC_PyModule_AddType(module,type) \
	PyModule_AddObject(module, #type, (PyObject *) PyFLAC_type(type))


#endif // __PyFLAC_h__

