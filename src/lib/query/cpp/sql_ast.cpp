#include <limits>
#include <query/cpp/sql_ast.h>

namespace cell {
namespace query {
namespace sql {

select::numeric_expr::numeric_expr(const std::wstring& _v, int base, bool decimal):value_expr(decimal ? type::INT : type::FLOAT, _v)
{
   if (!decimal)
   {
      auto nv = std::stoll(value, nullptr, base);
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
      auto nv = std::stod(value, nullptr);
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

select::unary_expr::unary_expr(const type& _t, const std::wstring& _op, expr* c):
   expr(expr::kind::OP, _t), op(_op), child(c)
{}

select::unary_expr::unary_expr(const std::wstring& _op, expr* c):
    unary_expr(c->t, _op, c)
{}

select::unary_expr::~unary_expr()
{
   if (child!=nullptr)
   {
      delete child;
   }
}

select::binary_expr::binary_expr(const type& _t, const std::wstring& _op, expr* l, expr* r):
   expr(expr::kind::OP, _t), op(_op), left(l), right(r)
{}

select::binary_expr::binary_expr(const std::wstring& _op, expr* l, expr* r):
   binary_expr(l->t, _op, l, r)
{
   if (r->t > t)
   {
      // The type on the left side is not big enough to contain
      // (potentially) the type on the right side. So we have to
      // match the left type to the right type.
      t = r->t;
   }
}

select::binary_expr::~binary_expr()
{
   if (left!=nullptr)
   {
      delete left;
   }
   
   if (right!=nullptr)
   {
      delete right;
   }
}

select::list_expr::list_expr():
   expr(expr::kind::LIST, type::INT) {}
   
select::list_expr::~list_expr() 
{ 
   for(const auto* v : values) 
   { 
      delete v; 
   } 
}

void select::list_expr::add_expression(expr* e) 
{ 
   values.push_back(e); 
}

select::sub_select_expr::sub_select_expr(select* _ss):expr(kind::SUB_SELECT, type::INT), ss(_ss)
{
   if (ss!=nullptr && ss->count_select_expressions() > 0)
   {
      t = ss->select_expression(0)->t;
   }
}

select::sub_select_expr::~sub_select_expr()
{
   if (ss!=nullptr)
   {
      delete ss;
   }
}

select::select(select *_parent):query(query::kind::SELECT), parent(_parent) 
{
}

select::~select()
{
   for(auto* se : select_expressions)
   {
      delete se;
   }
}

} // end namespace sql
} // end namespace query
} // end namespace cell