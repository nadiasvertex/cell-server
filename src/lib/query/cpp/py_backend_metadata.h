#include <sstream>
#include <query/cpp/metadata.h>

namespace cell {
namespace query {

class py_backend_metadata
{
   /** The stream to use to write the output to. */
   std::wstringstream s;

   metadata& m;

   void generate_table(metadata::table& t);
   void generate_database(metadata::database& d);
   void generate_type(const data_type& t);

public:
   py_backend_metadata(metadata& _m);
   ~py_backend_metadata();
   

   /** Generates the Python code for the metadata. */
   void generate();
   


};

} // end namespace query
} // end namespace cell
