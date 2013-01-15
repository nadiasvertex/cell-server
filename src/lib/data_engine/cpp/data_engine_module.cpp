#include <Python.h>
#include <cstdint>

#include <data_engine/cpp/column_store.h>

namespace cell {
namespace engine {

enum class data_kind
{
   INTEGER
};

struct column_object {
   PyObject_HEAD

   // The data store.
   data_store *store;

   // Kind of data being stored in this column.
   data_kind data_type;
};

PyObject *
column_insert(column_object *self, PyObject *arg)
{
   column_id cid;

   switch (self->data_type)
      {
      case data_kind::INTEGER:
         {
         int value = int(PyLong_AsLong(arg));
         auto* store = static_cast<column_store<int>*>(self->store);
         cid = store->insert(value);
         }
      break;

      }

   return PyLong_FromUnsignedLongLong(cid.as_uint64());
}

static void
column_type_dealloc(column_object* self)
{
   //Py_XDECREF(self->first);
   //Py_XDECREF(self->last);
   delete self->store;
   Py_TYPE(self)->tp_free((PyObject*) self);
}

static PyObject *
column_type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
   column_object *self;

   self = (column_object*) type->tp_alloc(type, 0);
   return (PyObject *) self;
}

static int
column_type_init(column_object *self, PyObject *args, PyObject *kwds)
{

   PyObject *default_value = nullptr;

   int data_type = 0;
   int nullable = 0;
   int precision = 0;

   static char *kwlist[] =
            {
            "data_type", "nullable", "precision", "default", NULL
            };

   if (!PyArg_ParseTupleAndKeywords(args, kwds, "|$ipiO", kwlist,
         &data_type, &nullable,
         &precision, &default_value))
      {
      return -1;
      }

   switch (data_kind(data_type))
      {
      case data_kind::INTEGER:
         self->store = new column_store<int>();
         self->data_type = data_kind(data_type);
      break;
      }

   return 0;
}

static PyMethodDef column_type_methods[] = {
    {"insert", (PyCFunction)column_insert, METH_O,
     "Insert a new value into the column, returning the new column id."
    },
    {NULL}  /* Sentinel */
};

static PyTypeObject column_type =
   {
   PyVarObject_HEAD_INIT(NULL, 0)
   "native_data_engine.Column", /* tp_name */
   sizeof(column_object), /* tp_basicsize */
   0, /* tp_itemsize */
   (destructor)column_type_dealloc, /* tp_dealloc */
   0, /* tp_print */
   0, /* tp_getattr */
   0, /* tp_setattr */
   0, /* tp_reserved */
   0, /* tp_repr */
   0, /* tp_as_number */
   0, /* tp_as_sequence */
   0, /* tp_as_mapping */
   0, /* tp_hash  */
   0, /* tp_call */
   0, /* tp_str */
   0, /* tp_getattro */
   0, /* tp_setattro */
   0, /* tp_as_buffer */
   Py_TPFLAGS_DEFAULT |
   Py_TPFLAGS_BASETYPE, /* tp_flags */
   "column_objects", /* tp_doc */
   0, /* tp_traverse */
   0, /* tp_clear */
   0, /* tp_richcompare */
   0, /* tp_weaklistoffset */
   0, /* tp_iter */
   0, /* tp_iternext */
   column_type_methods, /* tp_methods */
   nullptr, /* tp_members */
   0, /* tp_getset */
   0, /* tp_base */
   0, /* tp_dict */
   0, /* tp_descr_get */
   0, /* tp_descr_set */
   0, /* tp_dictoffset */
   (initproc)column_type_init, /* tp_init */
   0, /* tp_alloc */
   column_type_new, /* tp_new */
   }
;

static struct PyModuleDef data_engine_module =
         {
         PyModuleDef_HEAD_INIT,
               "native_data_engine", /* name of module */
               nullptr, /* module documentation, may be NULL */
               -1, /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
               nullptr
         };

PyMODINIT_FUNC
PyInit_native_data_engine(void)
{
   PyObject * m;

   column_type.tp_new = PyType_GenericNew;
   if (PyType_Ready(&column_type) < 0)
      {
      return nullptr;
      }

   m = PyModule_Create(&data_engine_module);
   if (m == nullptr)
      {
      return nullptr;
      }

   Py_INCREF (&column_type);
   PyModule_AddObject(m, "Table", (PyObject *) &column_type);
   return m;
}

} // end engine namespace
} // end cell namespace
