#include <data_engine/cpp/column_id.h>

namespace cell   {
namespace engine {

class data_store
{
public:
	typedef std::uint64_t size_type;

public:
	virtual ~data_store() {}
	virtual size_type size()=0;
};

} // end namespace engine
} // end namespace cell
