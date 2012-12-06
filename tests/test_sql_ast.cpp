#include <cstdint>

#include <query/cpp/sql_parser.h>

#include <gtest/gtest.h>

#define CHECK_BINOP(out, expected_op, n)                       \
sql::select::binary_expr* out;                                 \
{                                                              \
   ASSERT_TRUE(nullptr!=n);                                    \
   ASSERT_EQ(sql::select::expr::kind::OP, n->k);               \
   out = dynamic_cast<sql::select::binary_expr*>(n);           \
   ASSERT_TRUE(out!=nullptr);                                  \
   ASSERT_TRUE(out->left != nullptr);                          \
   ASSERT_TRUE(out->right != nullptr);                         \
                                                               \
   ASSERT_EQ(expected_op, out->op);                            \
}

#define CHECK_VALUE(out, expected_value, expected_type, n)     \
sql::select::value_expr* out;                                  \
{                                                              \
   ASSERT_TRUE(nullptr!=n);                                    \
   ASSERT_EQ(sql::select::expr::kind::VALUE, n->k);            \
                                                               \
   out = dynamic_cast<sql::select::value_expr*>(n);            \
   ASSERT_TRUE(out!=nullptr);                                  \
                                                               \
   ASSERT_EQ(expected_value, out->value);                      \
   ASSERT_EQ(expected_type, out->t);                           \
}

#define CHECK_SUBSELECT(out1, out2, n)                         \
sql::select::sub_select_expr* out1;                            \
sql::select* out2;                                             \
{                                                              \
   ASSERT_TRUE(nullptr!=n);                                    \
   ASSERT_EQ(sql::select::expr::kind::SUB_SELECT, n->k);       \
                                                               \
   out1 = dynamic_cast<sql::select::sub_select_expr*>(n);      \
   ASSERT_TRUE(out1!=nullptr);                                 \
   out2 = out1->ss;                                            \
   ASSERT_TRUE(out2!=nullptr);                                 \   
}

#define CHECK_SELECT_QUERY(out, n)                             \
sql::select *out;                                              \
{                                                              \
   ASSERT_TRUE(n!=nullptr);                                    \
   ASSERT_EQ(sql::query::kind::SELECT, n->k);                  \
   out = dynamic_cast<sql::select*>(n.get());                  \
   ASSERT_TRUE(out!=nullptr);                                  \
}

#define CHECK_SELECT_EXPRESSION(out, n)                        \
   ASSERT_EQ(1, n->count_select_expressions());                \
   auto *out = n->select_expression(0).get();

TEST(SqlAstTest, SimpleLiteral)
{
   using namespace cell::query;
   
   std::string query("SELECT 1;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
   ASSERT_EQ(1, p.count_queries());
   
   auto q = p.query(0);      
   CHECK_SELECT_QUERY(sq, q)
   CHECK_SELECT_EXPRESSION(se, sq);
   CHECK_VALUE(v, L"1", sql::type::TINYINT, se);   
}

TEST(SqlAstTest, SimpleExpression1)
{
   using namespace cell::query;
   
   std::string query("SELECT 1+1;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
   ASSERT_EQ(1, p.count_queries());
   
   auto q = p.query(0);
   CHECK_SELECT_QUERY(sq, q)
   CHECK_SELECT_EXPRESSION(se, sq);
   CHECK_BINOP(b, L"+", se);
   
   CHECK_VALUE(l, L"1", sql::type::TINYINT, b->left.get());
   CHECK_VALUE(r, L"1", sql::type::TINYINT, b->right.get());
}

TEST(SqlAstTest, SimpleExpression2)
{
   using namespace cell::query;
   
   std::string query("SELECT 102*75;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
   ASSERT_EQ(1, p.count_queries());
   
   auto q = p.query(0);
   CHECK_SELECT_QUERY(sq, q)
   CHECK_SELECT_EXPRESSION(se, sq);
   CHECK_BINOP(b, L"*", se);
   
   CHECK_VALUE(l, L"102", sql::type::TINYINT, b->left.get());
   CHECK_VALUE(r, L"75", sql::type::TINYINT, b->right.get());
}

TEST(SqlAstTest, ComplexExpression)
{
   using namespace cell::query;
   
   std::string query("SELECT 19-23*79;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
   ASSERT_EQ(1, p.count_queries());
   
   auto q = p.query(0);
   CHECK_SELECT_QUERY(sq, q)
   CHECK_SELECT_EXPRESSION(se, sq);
   CHECK_BINOP(b, L"-", se);      
   CHECK_BINOP(r1, L"*",  b->right.get());
   
   CHECK_VALUE(l1, L"19", sql::type::TINYINT, b->left.get());
   CHECK_VALUE(l2, L"23", sql::type::TINYINT, r1->left.get());
   CHECK_VALUE(r2, L"79", sql::type::TINYINT, r1->right.get());
}

TEST(SqlAstTest, BetweenExpression)
{
   using namespace cell::query;
   
   std::string query("SELECT 5 BETWEEN 1 AND 10;");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
   ASSERT_EQ(1, p.count_queries());
   
   auto q = p.query(0);
   CHECK_SELECT_QUERY(sq, q)
   CHECK_SELECT_EXPRESSION(se, sq);
   
   CHECK_BINOP(b, L"AND", se);      
   CHECK_BINOP(l1, L">=", b->left.get());
   CHECK_BINOP(r1, L"<=", b->right.get());
   
   CHECK_VALUE(l2, L"5", sql::type::TINYINT, l1->left.get());
   CHECK_VALUE(r2, L"1", sql::type::TINYINT, l1->right.get());
   CHECK_VALUE(l3, L"5", sql::type::TINYINT, r1->left.get());
   CHECK_VALUE(r3, L"10", sql::type::TINYINT, r1->right.get());   
}

TEST(SqlAstTest, Subselect)
{
   using namespace cell::query;
   
   std::string query("SELECT (SELECT 1);");
   sql_parser p(query);
   
   ASSERT_EQ(0, p.parse());
   ASSERT_EQ(1, p.count_queries());
   
   auto q = p.query(0);
   CHECK_SELECT_QUERY(sq, q)
   CHECK_SELECT_EXPRESSION(se, sq);
   
   CHECK_SUBSELECT(sse, ss, se);   
   CHECK_SELECT_EXPRESSION(se2, ss);
   CHECK_VALUE(v, L"1", sql::type::TINYINT, se2);
}
