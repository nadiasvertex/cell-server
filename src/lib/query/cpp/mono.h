#include <string>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace cell {
namespace query {

class mono
{
	MonoDomain *domain; 
	
public:
	mono();

	bool load(const std::string& path);
};

} // end namespace query
} // end namespace cell
