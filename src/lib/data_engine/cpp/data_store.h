#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <data_engine/cpp/column_id.h>

namespace cell   {
namespace engine {

class data_store
{
public:
	typedef std::uint64_t size_type;

public:
	virtual size_type size()=0;
	
	/**
	 * Run the given method to perform a filter on the given column id.
	 *
	 * @param cid: The column id to run the filter against.
	 * @param m: The method to use as the filter.
	 * 
	 * @returns: true if the 'm' filter method returned true, false otherwise.
	 */
    virtual bool filter(const column_id& cid, MonoMethod *m)=0;	
	
	/**
	 * Run the given method to transform the value to some other value.
	 *
	 * @param cid: The column id to run the filter against.
	 * @param m: The method to use as the filter.
	 * 
	 * @returns: A new object which contains the value projected from
	 *           from the specified column id.
	 */
	virtual MonoObject* transform(const column_id& cid, MonoMethod *m)=0;
};

} // end namespace engine
} // end namespace cell
