#include <iostream>
#include <query/cpp/cs_backend_metadata.h>

namespace cell {
namespace query {

cs_backend_metadata::cs_backend_metadata(const metadata& _m) :
		m(_m) {

}

cs_backend_metadata::~cs_backend_metadata() {}

bool cs_backend_metadata::generate_table(const metadata::table& t) {
	s << "public class " << t.name().c_str() << "{" << std::endl;
	for (const auto& col : t.column_name_map()) {
		const auto& name = col.first;
		const auto& id = col.second;
		const auto& type = t.column_map().find(id)->second;
		s << "\tpublic " << name.c_str() << std::endl;
	}
	s << "}" << std::endl;
}

} // end namespace query
} // end namespace cell
