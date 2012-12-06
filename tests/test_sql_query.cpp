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

TEST(SqlQueryTest, CanParseSelectPosOne)
{
   using namespace cell::query;
   
   std::string query("SELECT +1;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(SqlQueryTest, CanParseSelectNegOne)
{
   using namespace cell::query;
   
   std::string query("SELECT -1;");
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

TEST(SqlQueryTest, CanParseSelectMultiExpr)
{
   using namespace cell::query;
   
   std::string query("SELECT 100*2, 50*4;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(SqlQueryTest, CanParseSelectBooleanExpr)
{
   using namespace cell::query;
   
   std::string query("SELECT 1<2, 2>1, 5>=2, 10<=21, 1!=0;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(SqlQueryTest, CanParseSelectComplexBooleanExpr)
{
   using namespace cell::query;
   
   std::string query("SELECT 1<2 AND 2>1 OR 5>=2 AND 10<=21;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(SqlQueryTest, CanParseSelectBetweenExpr)
{
   using namespace cell::query;
   
   std::string query("SELECT 10 BETWEEN 1 AND 100;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(SqlQueryTest, CanParseSelectIsNull)
{
   using namespace cell::query;
   
   std::string query("SELECT 1 IS NULL;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(SqlQueryTest, CanParseSelectIsNotNull)
{
   using namespace cell::query;
   
   std::string query("SELECT 1 IS NOT NULL;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(SqlQueryTest, CanParseSelectIsDistinctFrom)
{
   using namespace cell::query;
   
   std::string query("SELECT 1 IS DISTINCT FROM 2;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(SqlQueryTest, CanParseSelectIsNotDistinctFrom)
{
   using namespace cell::query;
   
   std::string query("SELECT 1 IS NOT DISTINCT FROM 2;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}