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

void 
cs_backend::generate_type(const sql::type& t)
{
   switch(t)
   {
      case sql::type::BOOL:
         s << L"bool";
         break;
         
      case sql::type::TINYINT:
         s << L"char";
         break;
         
      case sql::type::SMALLINT:
         s << L"short";
         break;
         
      case sql::type::INT:
         s << L"int";
         break;
         
      case sql::type::BIGINT:
         s << L"long";
         break;
         
      case sql::type::FLOAT:
         s << L"float";
         break;
         
      case sql::type::DOUBLE:
         s << L"double";
         break;
         
      case sql::type::VARCHAR:
         s << L"string";
         break;
   }
}

bool 
cs_backend::generate_binary_expr(sql::select::binary_expr *b)
{
   generate_expression(b->left);
   
   s << L" ";
   
   if (b->op == L"AND")
   {
      s << L"&&";
   } else if (b->op == L"OR")
   {
      s << L"||";
   }
   else
   {
      s << b->op;
   }
   
   generate_expression(b->right);
   
   return true;
}

bool 
cs_backend::generate_unary_expr(sql::select::unary_expr *u)
{
   s << L" "
     << u->op;
   
   generate_expression(u->child);
   
   return true;
}

bool 
cs_backend::generate_value_expr(sql::select::value_expr *v)
{
   s << L" " 
     << v->value;
     
   return true;
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
               if (generate_unary_expr(dynamic_cast<sql::select::unary_expr *>(e.get())))
               {
                  return false;
               }  
            }
            else
            {
               if (generate_binary_expr(b)==false)
               {
                  return false;
               }
            }
         } break;
         
         case sql::select::expr::kind::VALUE:
            if (generate_value_expr(dynamic_cast<sql::select::value_expr *>(e.get()))==false)
            {
               return false;
            }
         break;
         
         default:
            return false;
      }      
      
      return true;
}

bool 
cs_backend::generate_select(sql::select *sq)
{
   s << "public class select_query ";
   block cb(s,1);
   
   auto se_count = sq->count_select_expressions();
   
   cb.indent();
   s << "[ProtoContract]" << std::endl;
   cb.indent();
   s << "public struct output {" << std::endl;
   // Generate struct to contain select expression output
   for(auto i = 0; i< se_count; ++i)
   {
      cb.indent();
      s << "  [ProtoMember(" << i << ")] public ";       
      auto se = sq->select_expression(i);
      generate_type(se->t);
      s << " c"
        << i << ";"
        << std::endl;
   }
   cb.indent();
   s << L"};" << std::endl;
   
   // Generate select expression evaluators
   for(auto i = 0; i< se_count; ++i)
   {
      auto se = sq->select_expression(i);
      
      cb.indent();
      generate_type(se->t);
      s << " select_expr_"
        << i
        << "() ";
        
      block seb(s,2);
      seb.indent();
      s << "return ";
            
      if (generate_expression(se)==false)
      {
         return false;
      }
      
      s << ";" << std::endl;
   }

   // Generate serializer.
   cb.indent();
   s << "public void serialize() {" << std::endl;   
   cb.indent();
   s << "   var data = new output {";
   for(auto i = 0; i< se_count; ++i)
   {      
      s << "c" << i << "=select_expr_" << i << "()";
      if (i < se_count-1)
      {
         s << ", ";
      }
   }
   s << "};" << std::endl;
   cb.indent();
   s << "}" << std::endl;
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