#include <iostream>
#include <query/cpp/cs_backend_metadata.h>

namespace cell {
namespace query {

cs_backend_metadata::cs_backend_metadata(metadata& _m) :
		m(_m) {

}

cs_backend_metadata::~cs_backend_metadata() {
}

void cs_backend_metadata::generate_type(const data_type& t) {
	switch (t.k) {
	case data_type::kind::BOOL:
		s << "bool";
		break;

	case data_type::kind::TINYINT:
		s << "char";
		break;

	case data_type::kind::SMALLINT:
		s << "short";
		break;

	case data_type::kind::INT:
		s << "int";
		break;

	case data_type::kind::BIGINT:
		s << "long";
		break;

	case data_type::kind::FLOAT:
		s << "float";
		break;

	case data_type::kind::DOUBLE:
		s << "double";
		break;

	case data_type::kind::VARCHAR:
		s << "string";
		break;
	}
}

void cs_backend_metadata::generate_table(metadata::table& t) {
	s << "\t\tpublic class " << t.name().c_str() << " {" << std::endl;
	for (const auto& col : t.column_name_map()) {
		const auto& name = col.first;
		const auto& id = col.second;
		const auto& type = t.column_map().find(id)->second;

		s << "\t\t\tpublic ";
		generate_type(type);
		s << " " << name.c_str() << ";" << std::endl;
	}
	s << "\t\t}" << std::endl;
}

void cs_backend_metadata::generate_database(metadata::database& d) {
	s << "\tpublic class " << d.name().c_str() << " {" << std::endl;
	for (const auto& t : d.table_name_map()) {
		const auto& name = t.first;
		const auto& id = t.second;
		auto& tbl = d.table_map().find(id)->second;

		generate_table(tbl);
	}
	s << "\t}" << std::endl;
}

void cs_backend_metadata::generate() {
	s << "namespace schema {" << std::endl;
	for (const auto& db : m.database_name_map()) {
		const auto& name = db.first;
		const auto& id = db.second;
		auto& d = m.database_map().find(id)->second;

		generate_database(d);
	}
	s << "}" << std::endl;

	std::wcout << s.str();
}

} // end namespace query
} // end namespace cell
