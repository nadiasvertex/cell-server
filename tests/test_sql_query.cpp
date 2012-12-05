#include <cstdint>
#include <memory>

#include <query/cpp/sql_parser.h>

#include <gtest/gtest.h>

TEST(SqlQueryTest, CanCreate)
{
   std::string query("SELECT * FROM test;");
   std::unique_ptr<cell::query::sql_parser> q;
   ASSERT_NO_THROW(
         q =std::unique_ptr<cell::query::sql_parser>( new cell::query::sql_parser(query)));
}

TEST(SqlQueryTest, CanParseSelectOne)
{
   using namespace cell::query;
   
   std::string query("SELECT 1;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}