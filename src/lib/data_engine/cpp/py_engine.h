#include <Python.h>
#include <string>

namespace cell {
namespace engine {

PyMODINIT_FUNC PyInit_native_data_engine(void);

/** Provides a Python interpreter environment. */
class py_engine
{
public:
   py_engine()
   {
      PyImport_AppendInittab("native_data_engine",
                             &PyInit_native_data_engine);
      Py_Initialize();
   }

   ~py_engine()
   {
      Py_Finalize();
   }

   bool exec(const std::string& script)
   {
      return PyRun_SimpleString(script.c_str()) == 0;
   }

};

} // end namespace engine
} // end namespace cell
