#include <sstream>
#include <query/cpp/metadata.h>

namespace cell {
namespace query {

class cs_backend_metadata
{
   /** The stream to use to write the output to. */
   std::wstringstream s;

   metadata& m;

   void generate_table(metadata::table& t);
   void generate_database(metadata::database& d);
   void generate_type(const data_type& t);

public:
   cs_backend_metadata(metadata& _m);
   ~cs_backend_metadata();
   

   /** Generates the C# code for the metadata. */
   void generate();
   


};

} // end namespace query
} // end namespace cell
