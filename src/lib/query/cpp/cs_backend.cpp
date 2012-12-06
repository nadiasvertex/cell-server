#include <iostream>
#include <query/cpp/cs_backend.h>

namespace cell {
namespace query {

class block
{
   std::wstringstream &s;
   int depth;
public:   
   block(std::wstringstream &_s, int _depth):s(_s), depth(_depth)
   {     
      s << L"{" << std::endl;
   }
   
   ~block()
   {
      depth--;
      indent();
      s << "}" << std::endl;      
   }
   
   void indent()
   {
      for(auto i=0; i<depth; ++i)
      {
         s << "   ";
      }
   }
};

cs_backend::cs_backend(sql_parser &_p):p(_p)
{
}

cs_backend::~cs_backend()
{
   std::wcout << s.str();
}

bool 
cs_backend::generate_binary_expr(sql::select::binary_expr *b)
{
   generate_expression(b->left);
   
   s << L" "
     << b->op;
   
   generate_expression(b->right);
}

bool 
cs_backend::generate_unary_expr(sql::select::unary_expr *u)
{
   s << L" "
     << u->op;
   
   generate_expression(u->child);
}

bool 
cs_backend::generate_value_expr(sql::select::value_expr *v)
{
   s << L" " 
     << v->value;
}

bool
cs_backend::generate_expression(sql::select::expr_handle_type &e)
{
      switch(e->k)
      {
         case sql::select::expr::kind::OP: {
            auto *b = dynamic_cast<sql::select::binary_expr *>(e.get());
            if (b==nullptr)
            {
               generate_unary_expr(dynamic_cast<sql::select::unary_expr *>(e.get()));
            }
            else
            {
               generate_binary_expr(b);
            }
         } break;
         
         case sql::select::expr::kind::VALUE:
            generate_value_expr(dynamic_cast<sql::select::value_expr *>(e.get()));
         break;
      }      
      
      return true;
}

bool 
cs_backend::generate_select(sql::select *sq)
{
   s << L"public class select_query ";
   block cb(s,1);
   
   auto se_count = sq->count_select_expressions();
   for(auto i = 0; i< se_count; ++i)
   {
      cb.indent();
      s << L"some_type select_expr_"
        << i
        << L"() ";
        
      block seb(s,2);
      seb.indent();
      s << L"return ";
      
      auto se = sq->select_expression(i);
      if (generate_expression(se)==false)
      {
         return false;
      }
      
      s << L";" << std::endl;
   }     
   
   return true;   
}
   
bool 
cs_backend::generate_query(sql_parser::query_handle_type &q)
{
   if (q->k == sql::query::kind::SELECT)
   {
      if (generate_select(dynamic_cast<sql::select*>(q.get()))==false)
      {
         return false;
      }
   }
   return true;   
}
   
bool 
cs_backend::generate()
{
   
   auto query_count = p.count_queries();
   for(auto i = 0; i< query_count; ++i)
   {
      auto q = p.query(i);
      if (generate_query(q)==false)
      {
         return false;
      }
   }
   
   return true;
}

} // end namespace query
} // end namespace cell