#include <sstream>
#include <query/cpp/metadata.h>

namespace cell {
namespace query {

class cs_backend_metadata
{
   /** The stream to use to write the output to. */
   std::wstringstream s;

   const metadata& m;

   bool generate_table(const metadata::table& t);
public:
   cs_backend_metadata(const metadata& _m);
   ~cs_backend_metadata();
   
   /** Generates the C# code for the metadata. */
   bool generate();
   
};

} // end namespace query
} // end namespace cell
