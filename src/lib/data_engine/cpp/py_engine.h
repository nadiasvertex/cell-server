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
     PyThreadState *old_thread_state;
  public:
     py_guard(const py_engine& _e):e(_e)
     {
        old_thread_state = PyThreadState_Swap(e.thread_state);
     }

     ~py_guard()
     {
        PyThreadState_Swap(old_thread_state);
     }
  };

  friend class py_guard;

public:
  py_engine();
  ~py_engine();

  bool exec(const std::string& script);
  bool test(const std::string& script);
};

} // end namespace engine
} // end namespace cell
