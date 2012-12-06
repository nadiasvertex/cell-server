#include <string>
#include <vector>

namespace cell {
namespace query {
namespace sql {

class query
{
};

enum class type
{
   TINYINT,
   SMALLINT,
   INT,
   BIGINT,
   FLOAT,
   DOUBLE,
   DECIMAL,
   VARCHAR
};

/**
 * Contains the information storage about a SQL SELECT query.
 */
class select : public query
{
 public:
   //===--------------------------------------------------------------------------------===//
   // select expression AST classes
   //===--------------------------------------------------------------------------------===//
   class expr
   {
   public:
      enum class kind
      {
         VALUE,
         COLUMN_NAME,
         OP,
         SUB_SELECT
      };
      
      kind k;
      type t;
      
      expr(const kind& _k, const type& _t):k(_k), t(_t) {}
   };
   
   class sub_select_expr : public expr
   {
   public:
      select *ss;
      sub_select_expr(select *_ss);
   };
   
   class value_expr : public expr
   {      
   public:
      std::wstring value;
      value_expr(const type& _t, const std::wstring& _v):expr(expr::kind::VALUE, _t), value(_v) {}
   };
   
   class numeric_expr : public value_expr
   {   
   public:
      numeric_expr(const std::wstring& _v, int base, bool decimal);
   };
   
   class unary_expr : public expr
   {
      expr *child;
   public:
      std::wstring op;
      unary_expr(const std::wstring& _op, expr* c);
   };
   
   class binary_expr : public expr
   {
      expr *left, *right;
   public:
      std::wstring op;
      binary_expr(const std::wstring& _op, expr* l, expr* r);
   };   
   
   //===--------------------------------------------------------------------------------===//
   // select query types
   //===--------------------------------------------------------------------------------===//
public:   
   typedef std::vector<expr*> select_expression_list;
   
   //===--------------------------------------------------------------------------------===//
   // select query data
   //===--------------------------------------------------------------------------------===//
private:
   select_expression_list select_expressions;
   
   //===--------------------------------------------------------------------------------===//
   // select query API
   //===--------------------------------------------------------------------------------===//
public:
   select() {}
   virtual ~select();

   /** Find out how many select expressions are in this query. */
   select_expression_list::size_type count_select_expressions()
   {
      return select_expressions.size();
   }
   
   /** Gets a specific select expression. */
   expr* select_expression(select_expression_list::size_type index)
   {
      return select_expressions[index];
   }
   
   /** Adds a new select expression to the list. */
   void add_select_expression(expr* e)
   {
      select_expressions.push_back(e);
   }
   
};

} // end namespace sql
} // end namespace query
} // end namespace cell