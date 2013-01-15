#include <cstdint>
#include <memory>

#include <data_engine/cpp/py_engine.h>

#include <gtest/gtest.h>

TEST(PyEngineTest, CanCreate)
{
   std::unique_ptr<cell::engine::py_engine> e;
   ASSERT_NO_THROW(
         e =std::unique_ptr<cell::engine::py_engine>(
         new cell::engine::py_engine()));
}
