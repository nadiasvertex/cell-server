#ifndef __CELL_ENGINE_COLUMN_STORE_H__
#define __CELL_ENGINE_COLUMN_STORE_H__

#include <cstdint>
#include <unordered_map>
#include <data_engine/cpp/data_store.h>

namespace cell
{
namespace engine
{

template<typename T>
class column_store : public data_store
{
public:
  typedef std::unordered_map<column_id, T, column_id_hash> id_value_map_type;

  typedef std::unordered_map<T, column_id> value_id_map_type;

  typedef std::unordered_map<column_id, std::uint64_t, column_id_hash> id_ref_map_type;

  typedef std::vector<column_id> column_id_list;

private:
  /** Maps ids to values. */
  id_value_map_type iv_map;

  /** Maps values to ids. */
  value_id_map_type vi_map;

  /** Maps ids to reference counts. */
  id_ref_map_type ref_map;

  /** The de-duplication threshold. Once 'threshold' unique
   * items have been found, stop trying to de-dupe. */
  typename id_value_map_type::size_type threshold;

  /** Generates column ids. */
  column_id id_generator;

private:
  void inc_ref(const column_id& id) {
    auto pos = ref_map.find(id);

    if (pos==ref_map.end()) {
      ref_map.insert(std::make_pair(id, 2));
    }

    (ref_map[id])++;
  }

  bool dec_ref(const column_id& id) {
    auto pos = ref_map.find(id);

    if (pos==ref_map.end()) {
      return false;
    }

    if (pos->second == 1) {
      ref_map.erase(pos);
      return false;
    }

    (pos->second)--;
    return true;
  }
public:
  column_store():threshold(1024) {}

  virtual size_type size() {
    return iv_map.size();
  }

  /**
   * Erase a column from the store. Checks to see if there is a reference count
   * on the column. Only removes columns when all references to the column have
   * been removed.
   *
   * @param id: The id to remove from the store.
   */
  virtual void erase(const column_id& id) {
    auto pos = iv_map.find(id);

    if (pos==iv_map.end()) {
      return;
    }

    if (!vi_map.empty()) {
      vi_map.erase(pos->second);
    }

    if (!dec_ref(id)) {
      //vi_map.erase(pos);
    }
  }

  /**
   * Inserts the value into the store. Performs de-duplication if the
   * number of unique items in the store is below the given threshold.
   *
   * @param value: The value to insert into the table.
   */
  column_id insert(const T& value) {
    if (iv_map.size() < threshold) {
      auto pos = vi_map.find(value);

      if (pos != vi_map.end()) {
        inc_ref(column_id::from_uint64(pos->first));
        return pos->second;
      }
    }

    auto new_id = id_generator.next();
    iv_map.insert(std::make_pair(new_id, value));

    if (iv_map.size() >= threshold 
	&& (!vi_map.empty())) {
      vi_map.clear();
    } else {
      vi_map.insert(std::make_pair(value, new_id));
    }

    return new_id;
  }

  /**
   * Executes a predicate over a column. Columns which
   * match will have their ids stored in a vector. The
   * vector is returned to the caller.
   */
  column_id_list find(const binary_predicate& p)
  {
    column_id_list matches;
    for(auto& el : iv_map)
      {
	if (p(el.second))
	  {
	    matches.push_back(el.first);
	  }
      }
  }
};

} // end namespace engine
} // end namespace cell

#endif // __CELL_ENGINE_COLUMN_STORE_H__
