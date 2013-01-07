#include <cstdint>
#include <memory>

#include <data_engine/cpp/column_store.h>

#include <gtest/gtest.h>

TEST(ColumnStoreTest, CanCreate)
{
   std::unique_ptr<cell::engine::column_store<int>> t;
   ASSERT_NO_THROW(
         t =std::unique_ptr<cell::engine::column_store<int>>( new cell::engine::column_store<int>() ));
}
