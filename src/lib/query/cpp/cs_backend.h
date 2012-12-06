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
   
   
   bool generate_query(sql_parser::query_handle_type &q);
   bool generate_select(sql::select *sq);
public:
   cs_backend(sql_parser &_p);
   ~cs_backend();
   
   /** Generates the C# code for the query. */
   bool generate();
   
};

} // end namespace query
} // end namespace cell