#include <string>
#include <unordered_map>

#include <query/cpp/data_type.h>
#include <query/cpp/database_id.h>
#include <data_engine/cpp/table_id.h>
#include <data_engine/cpp/column_id.h>

namespace cell {
namespace query {

/** 
 * Stores metadata information about
 * the various databases available.
 */
class metadata
{
public:
   class table
   {
   public:
      typedef std::unordered_map<std::string, engine::column_id> id_map_type;
      
      typedef std::unordered_map<engine::column_id, data_type> column_map_type;
      
   private:
      std::string name;      
      
      id_map_type column_ids;
      
      column_map_type columns;
      
      column_id generator;
      
   public:
      table(const std::string& _name):name(_name) {}      
      
      bool add_column(const std::string& name, const data_type& type);
      
      void drop_column(const column_id& cid);
      
      column_id column(const std::string& name);
   };
      
   class database
   {
   public:
      typedef std::unordered_map<std::string, engine::table_id> id_map_type;
      
      typedef std::unordered_map<engine::table_id, table> table_map_type;
   private:
      std::string name;
      
      id_map_type table_ids;
      
      table_map_type tables;      
            
   public:
      database(const std::string& _name):name(_name) {}
      
      bool create_table(const std::string& name);
      
      void drop_table(const table_id& tid);
      
      table_id table(const std::string& name);
   }

};

} // end namespace query
} // end namespace cell