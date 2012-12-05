#include <cstdlib>
#include <limits>
#include <query/cpp/sql_ast.h>

namespace cell {
namespace query {
namespace sql {

select::numeric_expr::numeric_expr(const std::string& _v, int base, bool decimal):value_expr(decimal ? type::INT : type::FLOAT, _v)
{
   if (!decimal)
   {
      auto nv = std::strtoll(value.c_str(), nullptr, base);
      if (nv>=-128 && nv<=127)
      {
         t = type::TINYINT;
      } 
      else if (nv>=-32768 && nv <=32767)
      {
         t = type::SMALLINT;
      }
      else if (nv>=-2147483648 && nv <=2147483647)
      {
         t = type::INT;
      }
      else
      {
         t = type::BIGINT;
      }
   }
   else
   {
      auto nv = strtod(value.c_str(), nullptr);
      if (nv >= std::numeric_limits<float>::min() &&
          nv <= std::numeric_limits<float>::max())
          {
            t = type::FLOAT;
          }
      else
      {
         t = type::DOUBLE;
      }      
   }
}

select::unary_expr::unary_expr(const std::string& _op, expr* c):
    expr(expr::kind::OP, c->t), op(_op), child(c)
{}

select::binary_expr::binary_expr(const std::string& _op, expr* l, expr* r):
   expr(expr::kind::OP, l->t), op(_op), left(l), right(r) 
{
   if (r->t > t)
   {
      // The type on the left side is not big enough to contain
      // (potentially) the type on the right side. So we have to
      // match the left type to the right type.
      t = r->t;
   }
}

} // end namespace sql
} // end namespace query
} // end namespace cell