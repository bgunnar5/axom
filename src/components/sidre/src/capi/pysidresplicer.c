! C code that will be inserted into Python module via shroud splicer blocks

// ----------------------------------------------------------------------
// ----- pySidremodule.hpp

// splicer begin include
#include "sidre/sidre.hpp"
#include "sidre/SidreTypes.h"
#include "SidreWrapperHelpers.hpp"
// splicer end include

// ----------------------------------------------------------------------
// ----- pySidremodule.cpp


// ----------------------------------------------------------------------
// ----- pyDataStoretype.cpp

// splicer begin class.DataStore.type.init
DataStore * ds = new DataStore();
self->BBB = ds;
return 0;
// splicer end class.DataStore.type.init

// ----------------------------------------------------------------------
// ----- pyDataGrouptype.cpp

// splicer begin class.DataGroup.type.init
    PyObject *grpobj;

    /* By requiring a PyCapsule, it is difficult to call directly from Python.
     * But the C++ constructors are private so that makes sense.
     */
    if (!PyArg_ParseTuple(args, "O!:DataGroup_init",
                          &PyCapsule_Type, &grpobj))
        return -1;

    /* capsule_dbnode */
    DataGroup *grp = static_cast<DataGroup *>(PyCapsule_GetPointer(grpobj, PY_DataGroup_capsule_name));
    self->BBB = grp;
    if (grp == NULL && PyErr_Occurred())
	return -1;
    
    return 0;
// splicer end class.DataGroup.type.init


// splicerX begin class.DataGroup.method.getName
const std::string & name = self->BBB->getName();
PyObject * rv = PyString_FromString(name.c_str());
return rv;
// splicerX end class.DataGroup.method.getName


// ----------------------------------------------------------------------
// ----- pyDataBuffertype.cpp


// ----------------------------------------------------------------------
// ----- pyDataViewtype.cpp


