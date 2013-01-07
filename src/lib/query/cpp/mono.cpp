#include <query/cpp/mono.h>

namespace cell {
namespace query {

mono::mono()
{	
    domain = mono_jit_init ("cell-engine");	
}

bool mono::load(const std::string& path)
{
	MonoAssembly *assembly; 
	assembly = mono_domain_assembly_open (domain, path.c_str());
	if (!assembly) return false;
}

} // end namespace query
} // end namespace cell
