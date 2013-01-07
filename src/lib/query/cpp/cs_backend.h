#include <sstream>
#include <query/cpp/sql_parser.h>

namespace cell {
namespace query {

class cs_backend
{
   /** The parser to use to walk the query. */
   sql_parser &p;
   
   /** The stream to use to write the output to. */
   std::wstringstream s;
   
   void generate_type(const sql::type& t);
   bool generate_query(sql_parser::query_handle_type &q);
   bool generate_select(sql::select *sq);
   bool generate_expression(sql::select::expr_handle_type &e);
   bool generate_binary_expr(sql::select::binary_expr *b);
   bool generate_unary_expr(sql::select::unary_expr *u);
   bool generate_value_expr(sql::select::value_expr *v);

public:
   cs_backend(sql_parser &_p);
   ~cs_backend();
   
   /** Generates the C# code for the query. */
   bool generate();
   
};

} // end namespace query
} // end namespace cell
