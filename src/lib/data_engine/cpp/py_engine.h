#include <Python.h>
#include <string>

namespace cell
{
namespace engine
{

PyMODINIT_FUNC PyInit_native_data_engine(void);

/** Provides a Python interpreter environment. */
class py_engine
{
  static bool initialized;
  PyThreadState *thread_state;

  class py_guard
  {
     const py_engine& e;
     PyGILState_STATE saved;
     PyThreadState *old_thread_state;
  public:
     py_guard(const py_engine& _e):e(_e)
     {
        saved = PyGILState_Ensure();
        old_thread_state = PyThreadState_Swap(e.thread_state);
     }

     ~py_guard()
     {
        PyThreadState_Swap(old_thread_state);
        PyGILState_Release(saved);
     }
  };

  friend class py_guard;

public:
  py_engine() {
    if (!initialized) {
      PyImport_AppendInittab("native_data_engine",
                             &PyInit_native_data_engine);
      Py_Initialize();
      initialized = true;
    }

    thread_state = Py_NewInterpreter();

  }

  ~py_engine() {
     py_guard guard(*this);
     Py_EndInterpreter(thread_state);

  }

  bool exec(const std::string& script) {
    py_guard guard(*this);
    return PyRun_SimpleString(script.c_str()) == 0;

  }

  bool test(const std::string& script) {
     py_guard guard(*this);

  }

};

} // end namespace engine
} // end namespace cell
