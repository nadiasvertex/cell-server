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

TEST(PyEngineTest, CanImportNativeDataEngine)
{
   cell::engine::py_engine e;

   ASSERT_TRUE(e.exec("import native_data_engine"));
}

TEST(PyEngineTest, CanCreateColumn)
{
   cell::engine::py_engine e;

   ASSERT_TRUE(e.exec("import native_data_engine;"
                      "t=native_data_engine.Column(data_type=0, default=0)"));
}

TEST(PyEngineTest, CanInsert)
{
   cell::engine::py_engine e;

   ASSERT_TRUE(e.exec("import native_data_engine;"
                      "t=native_data_engine.Column(data_type=0, default=0);"
                      "t.insert(10);"));
}

TEST(PyEngineTest, CanErase)
{
   cell::engine::py_engine e;

   ASSERT_TRUE(e.exec("import native_data_engine;"
                      "t=native_data_engine.Column(data_type=0, default=0);"
                      "id=t.insert(10);"
                      "t.erase(id)"));
}
