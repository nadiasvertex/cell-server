#include <query/coco/Scanner.h>
#include <query/coco/Parser.h>

namespace cell {
namespace query {

class sql_parser
{
   sql::Scanner* s;
   sql::Parser* p;
public:
   
   sql_parser(const std::string& query);
   ~sql_parser();
   
   /** 
    * Execute the parser.
    *
    * @returns: The number of errors encountered. */
   int parse();
};

} // end namespace query
} // end namespace cell