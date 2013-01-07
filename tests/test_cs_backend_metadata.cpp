#include <cstdint>
#include <memory>

#include <query/cpp/cs_backend_metadata.h>

#include <gtest/gtest.h>

TEST(CsBackendMetadataTest, CanCreate)
{
   using namespace cell::query;

   metadata m;
   
   std::unique_ptr<cs_backend_metadata> csm;
   ASSERT_NO_THROW(
         csm =std::unique_ptr<cs_backend_metadata>( new cs_backend_metadata(m)));
}

TEST(CsBackendMetadataTest, CanGenerate)
{
   using namespace cell::query;
   
   metadata m;
   cs_backend_metadata csm(m);
   
   csm.generate();

}
