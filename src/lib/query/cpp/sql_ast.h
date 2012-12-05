#include <string>

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
   class expr
   {
   public:
      enum class kind
      {
         VALUE,
         COLUMN_NAME,
         OP
      };
      
      kind k;
      type t;
      
      expr(const kind& _k, const type& _t):k(_k), t(_t) {}
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
      std::string op;
      unary_expr(const std::string& _op, expr* c);
   };
   
   class binary_expr : public expr
   {
      expr *left, *right;
   public:
      std::string op;
      binary_expr(const std::string& _op, expr* l, expr* r);
   };   
};

} // end namespace sql
} // end namespace query
} // end namespace cell