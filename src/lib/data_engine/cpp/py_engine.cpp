#include <data_engine/cpp/py_engine.h>

namespace cell
{
namespace engine
{

  bool py_engine::initialized=false;

  py_engine::py_engine() {
    if (!initialized) {
      PyImport_AppendInittab("native_data_engine",
                             &PyInit_native_data_engine);
      Py_Initialize();
      initialized = true;
    }
    thread_state = Py_NewInterpreter();
  }

  py_engine::~py_engine() {
     py_guard guard(*this);
     Py_EndInterpreter(thread_state);

  }

  bool py_engine::exec(const std::string& script) {
    py_guard guard(*this);
    return PyRun_SimpleString(script.c_str()) == 0;
  }

  bool py_engine::test(const std::string& script) {
     py_guard guard(*this);
     auto result = PyRun_String(script.c_str(),
				Py_eval_input,  
				PyEval_GetGlobals(),
				PyEval_GetLocals());

     return result == Py_True;
     
  }


} // end namespace engine
} // end namespace cell
