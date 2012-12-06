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

TEST(SqlQueryTest, CanParseSelectOneCommaTwo)
{
   using namespace cell::query;
   
   std::string query("SELECT 1, 2;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}


TEST(SqlQueryTest, CanParseSelectOnePlusOne)
{
   using namespace cell::query;
   
   std::string query("SELECT 1+1;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(SqlQueryTest, CanParseSelectOneTimesOne)
{
   using namespace cell::query;
   
   std::string query("SELECT 1*1;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(SqlQueryTest, CanParseSelectComplexLiteralExpr)
{
   using namespace cell::query;
   
   std::string query("SELECT 10*5+4-3/2%11;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}