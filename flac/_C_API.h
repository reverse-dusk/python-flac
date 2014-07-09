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

#ifndef __PyFLAC__C_API_h__
#define __PyFLAC__C_API_h__

#define PyFLAC__C_API_INIT_begin(count) \
static int \
_c_api_init (PyObject *m) \
{ \
	static void *_C_API[count]; \
	PyObject *_c_api;

#define PyFLAC__C_API_PUT(i,name) _C_API[i] = (void *) name

#define PyFLAC__C_API_INIT_end(module) \
	_c_api = PyCapsule_New((void *) _C_API, "flac." #module "._C_API", NULL); \
	if (_c_api == NULL) return -1; \
	return PyModule_AddObject(m, "_C_API", _c_api); \
}


#define PyFLAC_API(module) PyFLAC_##module##_API

#define PyFLAC__C_API(module) \
static void **PyFLAC_API(module); \
static int \
PyFLAC_import_##module (void) \
{ \
	if (PyFLAC_API(module) != NULL) return 0; \
	if (PyImport_ImportModule("flac." #module) == NULL) return -1; \
	PyFLAC_API(module) = (void **) PyCapsule_Import("flac." #module "._C_API", 0); \
	if (PyFLAC_API(module) == NULL) return -1; \
	return 0; \
}


#define PyFLAC_type_Check_PUT(type,i) \
	PyFLAC__C_API_PUT(i,PyFLAC_type_Check_NAME(type))

#define PyFLAC_type_Check_API(module,i) \
	(*(int (*)(PyObject *object)) PyFLAC_API(module)[i])


#ifdef __PyFLAC_enum_h__

#define PyFLAC_Enum_FromEnum_PUT(type,i) \
	PyFLAC__C_API_PUT(i,PyFLAC_Enum_FromEnum_NAME(type))

#define PyFLAC_Enum_FromEnum_API(module,i) \
	(*(PyObject * (*)(int e_value)) PyFLAC_API(module)[i])

#endif // __PyFLAC_enum_h__

#endif // __PyFLAC__C_API_h__

