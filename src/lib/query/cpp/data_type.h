#include <cstdint>

namespace cell {
namespace query {

/** 
 * Stores information about data types.
 */
class data_type
{
public:
   enum class kind 
   {
         BOOL,
         TINYINT,
         SMALLINT,
         INT,
         BIGINT,
         FLOAT,
         DOUBLE,
         DECIMAL,
         VARCHAR
   };
   
   /** The kind of data. */
   kind k;
   
   /** If true, this type allows null data. */
   bool nullable;
   
   /** The maximum size for data stored in this type. This
    * also doubles as precision for types that have that. */
   std::uint32_t size;   
};

} // end namespace query
} // end namespace cell