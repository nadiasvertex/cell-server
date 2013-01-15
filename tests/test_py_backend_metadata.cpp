#include <cstdint>
#include <memory>

#include <query/cpp/py_backend_metadata.h>

#include <gtest/gtest.h>

TEST(PyBackendMetadataTest, CanCreate)
{
   using namespace cell::query;

   metadata m;
   
   std::unique_ptr<py_backend_metadata> csm;
   ASSERT_NO_THROW(
         csm =std::unique_ptr<py_backend_metadata>( new py_backend_metadata(m)));
}

TEST(PyBackendMetadataTest, CanGenerate)
{
   using namespace cell::query;
   
   metadata m;
   py_backend_metadata csm(m);
   
   csm.generate();
}

TEST(PyBackendMetadataTest, CanGenerateDb)
{
   using namespace cell::query;

   metadata m;
   py_backend_metadata csm(m);

   m.create_database("testdb");

   csm.generate();
}

TEST(PyBackendMetadataTest, CanGenerateTable)
{
   using namespace cell::query;

   metadata m;
   py_backend_metadata csm(m);

   m.create_database("testdb");
   auto db_id = m.db("testdb").get();
   auto& db = m.database_map().at(db_id);

   db.create_table("test_table");

   csm.generate();
}

TEST(PyBackendMetadataTest, CanGenerateColumn)
{
   using namespace cell::query;

   metadata m;
   py_backend_metadata csm(m);

   m.create_database("testdb");
   auto db_id = m.db("testdb").get();
   auto& db = m.database_map().at(db_id);

   db.create_table("test_table");
   auto tbl_id = db.table("test_table").get();
   auto& tbl = db.table_map().at(tbl_id);

   tbl.add_column("c1", data_type {data_type::kind::BIGINT, false, 0});

   csm.generate();
}

TEST(PyBackendMetadataTest, CanGenerateColumns)
{
   using namespace cell::query;

   metadata m;
   py_backend_metadata csm(m);

   m.create_database("test_db");
   auto db_id = m.db("test_db").get();
   auto& db = m.database_map().at(db_id);

   db.create_table("test_table");
   auto tbl_id = db.table("test_table").get();
   auto& tbl = db.table_map().at(tbl_id);

   tbl.add_column("c1", data_type {data_type::kind::BIGINT, false, 0});
   tbl.add_column("c2", data_type {data_type::kind::VARCHAR, false, 0});
   tbl.add_column("c3", data_type {data_type::kind::INT, false, 0});

   csm.generate();
}
