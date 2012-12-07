#ifndef __CELL_ENGINE_TABLE_ID_H__
#define __CELL_ENGINE_TABLE_ID_H__

#include <cstdint>
#include <functional>

namespace cell {
namespace engine {

class table_id
{
   std::uint64_t id;

   table_id(std::uint64_t _id) :
         id(_id)
   {
   }
   ;

   friend class table_id_hash;

public:

   table_id() :
         id(0)
   {
   }
   ;

   static table_id from_uint64(std::uint64_t value)
   {
      return table_id(value);
   }

   table_id next()
   {
      return table_id(++id);
   }

   bool operator==(const table_id& o) const
   {
      return id == o.id;
   }

   bool operator<(const table_id& o) const
   {
      return id < o.id;
   }

   table_id operator+(int i)
   {
      return table_id(id + i);
   }

   table_id operator-(int i)
   {
      return table_id(id - i);
   }
};

class table_id_hash
{
public:
   std::size_t operator()(const table_id& id) const
   {
      std::hash<std::uint64_t> h;
      return h(id.id);
   }
};

} // namespace engine
} // namespace cell

#endif // __CELL_ENGINE_TABLE_ID_H__
