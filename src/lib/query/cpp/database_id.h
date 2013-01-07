#ifndef __CELL_QUERY_DATABASE_ID_H__
#define __CELL_QUERY_DATABASE_ID_H__

#include <cstdint>
#include <functional>

namespace cell {
namespace query {

class database_id
{
   std::uint64_t id;

   database_id(std::uint64_t _id) :
         id(_id)
   {
   }
   ;

   friend class database_id_hash;

public:

   database_id() :
         id(0)
   {
   }
   ;

   static database_id from_uint64(std::uint64_t value)
   {
      return database_id(value);
   }

   database_id next()
   {
      return database_id(++id);
   }

   bool operator==(const database_id& o) const
   {
      return id == o.id;
   }

   bool operator<(const database_id& o) const
   {
      return id < o.id;
   }

   database_id operator+(int i)
   {
      return database_id(id + i);
   }

   database_id operator-(int i)
   {
      return database_id(id - i);
   }
};

class database_id_hash
{
public:
   std::size_t operator()(const database_id& id) const
   {
      std::hash<std::uint64_t> h;
      return h(id.id);
   }
};

} // namespace cell
} // namespace lattice

#endif // __CELL_QUERY_DATABASE_ID_H__
