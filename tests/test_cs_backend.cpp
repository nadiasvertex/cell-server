#include <cstdint>
#include <memory>

#include <query/cpp/cs_backend.h>

#include <gtest/gtest.h>

TEST(CsBackendTest, CanCreate)
{
   using namespace cell::query;
   
   std::string query("SELECT * FROM test;");
   sql_parser p(query);
   
   std::unique_ptr<cs_backend> q;
   ASSERT_NO_THROW(
         q =std::unique_ptr<cs_backend>( new cs_backend(p)));
}

TEST(CsBackendTest, CanParseSelectOne)
{
   using namespace cell::query;
   
   std::string query("SELECT 1;");
   sql_parser p(query);
   cs_backend b(p);
   
   ASSERT_EQ(0, p.parse());
   ASSERT_TRUE(b.generate());
}

/*
TEST(CsBackendTest, CanParseSelectHexOne)
{
   using namespace cell::query;
   
   std::string query("SELECT 0x1;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectFloatOne)
{
   using namespace cell::query;
   
   std::string query("SELECT 1.0;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectFloatZeroPointOne)
{
   using namespace cell::query;
   
   std::string query("SELECT 0.1;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectFloatZeroPointOneEee)
{
   using namespace cell::query;
   
   std::string query("SELECT 0.1E+10;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectOneCommaTwo)
{
   using namespace cell::query;
   
   std::string query("SELECT 1, 2;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}


TEST(CsBackendTest, CanParseSelectOnePlusOne)
{
   using namespace cell::query;
   
   std::string query("SELECT 1+1;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectPosOne)
{
   using namespace cell::query;
   
   std::string query("SELECT +1;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectNegOne)
{
   using namespace cell::query;
   
   std::string query("SELECT -1;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectOneTimesOne)
{
   using namespace cell::query;
   
   std::string query("SELECT 1*1;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectComplexLiteralExpr)
{
   using namespace cell::query;
   
   std::string query("SELECT 10*5+4-3/2%11;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectMultiExpr)
{
   using namespace cell::query;
   
   std::string query("SELECT 100*2, 50*4;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectBooleanExpr)
{
   using namespace cell::query;
   
   std::string query("SELECT 1<2, 2>1, 5>=2, 10<=21, 1!=0;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectComplexBooleanExpr)
{
   using namespace cell::query;
   
   std::string query("SELECT 1<2 AND 2>1 OR 5>=2 AND 10<=21;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectBetweenExpr)
{
   using namespace cell::query;
   
   std::string query("SELECT 10 BETWEEN 1 AND 100;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectIsNull)
{
   using namespace cell::query;
   
   std::string query("SELECT 1 IS NULL;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectIsNotNull)
{
   using namespace cell::query;
   
   std::string query("SELECT 1 IS NOT NULL;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectIsDistinctFrom)
{
   using namespace cell::query;
   
   std::string query("SELECT 1 IS DISTINCT FROM 2;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectIsNotDistinctFrom)
{
   using namespace cell::query;
   
   std::string query("SELECT 1 IS NOT DISTINCT FROM 2;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectIn)
{
   using namespace cell::query;
   
   std::string query("SELECT 1 IN (1,2,3,4,5);");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectInSelect)
{
   using namespace cell::query;
   
   std::string query("SELECT 1 IN (SELECT 1);");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}

TEST(CsBackendTest, CanParseSelectSubSelect)
{
   using namespace cell::query;
   
   std::string query("SELECT (SELECT 1);");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
}*/