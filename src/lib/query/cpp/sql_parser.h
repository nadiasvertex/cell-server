#include <query/coco/Scanner.h>
#include <query/coco/Parser.h>

namespace cell {
namespace query {

class sql_parser
{
   sql::Scanner* s;
   sql::Parser* p;
   
public:
   typedef typename sql::Parser::query_list_type::size_type size_type;
   typedef typename sql::Parser::query_handle_type query_handle_type;

   
   sql_parser(const std::string& query);
   ~sql_parser();
   
   /** 
    * Execute the parser.
    *
    * @returns: The number of errors encountered. */
   int parse();
   
   /**
    * Counts the number of queries parsed out of the batch. */
   size_type count_queries();
   
   /** 
    * Gets the query at 'index' */
   query_handle_type query(size_type index);
};

} // end namespace query
} // end namespace cell