#ifndef __CELL_ENGINE_COLUMN_ID_H__
#define __CELL_ENGINE_COLUMN_ID_H__

#include <cstdint>

namespace cell   {
namespace engine {

class column_id
{
	std::uint64_t id;
	
	friend class column_id_hash;

public:
	/** Create a new, empty column id. */
	column_id():id(0) {}
   
   static column_id from_uint64(std::uint64_t value)
   {
      return table_id(value);
   }
	
	/**
	 * Copy a column id.
	 *
	 * @param cid: The column id to copy.
	 */
	column_id(const column_id& cid) {
		id = cid.id;
	}
	
	/**
	 * Used for generating a new column id. Modifies the
	 * existing column id.
	 */
	column_id next()
	{
		column_id cid;
		cid.id = ++id;
		
		return cid;
	}
	
	/** 
	 * Returns true if this column id is empty.
	 */
	bool empty()
	{
		return id == 0;
	}	
	
   bool operator==(const column_id& o) const
   {
      return id == o.id;
   }

   bool operator<(const column_id& o) const
   {
      return id < o.id;
   }
};

class column_id_hash
{
public:
   std::size_t operator()(const column_id& cid) const
   {
      std::hash<std::uint64_t> h;
      return h(cid.id);
   }
};

} // end namespace engine
} // end namespace cell

#endif // __CELL_ENGINE_COLUMN_ID_H__
