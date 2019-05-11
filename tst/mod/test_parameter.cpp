#include <gtest/gtest.h>

#include <fixtures/parameter_test_fixture.hpp>
#include <AeroKernel/parameter.hpp>

/*------------------------------------------------
AeroKernel::Parameter::Manager::registerParameter()
------------------------------------------------*/
TEST_F( PMTF, RegistrationNormal )
{
  using namespace AeroKernel::Parameter;

  const std::string_view key = "helloWorld";
  ParamCtrlBlk ctrlBlk       = { 0, 0, 0, nullptr };

  pm->init( 50 );

  EXPECT_EQ( true, pm->registerParameter( key, ctrlBlk ) );
}

TEST_F( PMTF, RegistrationOverwrite )
{
  using namespace AeroKernel::Parameter;

  const std::string_view key = "helloWorld";
  ParamCtrlBlk ctrlBlk1      = { 0, 0, 0, nullptr };
  ParamCtrlBlk ctrlBlk2      = { 1, 2, 3, nullptr };

  pm->init( 50 );

  /*------------------------------------------------
  Register the key and write the first control block
  ------------------------------------------------*/
  pm->registerParameter( key, ctrlBlk1 );
  EXPECT_TRUE( memcmp( &ctrlBlk1, &pm->getControlBlock( key ), sizeof( ParamCtrlBlk ) ) == 0 );

  /*------------------------------------------------
  Re-register the key and write the second control block
  ------------------------------------------------*/
  pm->registerParameter( key, ctrlBlk2 );
  EXPECT_TRUE( memcmp( &ctrlBlk2, &pm->getControlBlock( key ), sizeof( ParamCtrlBlk ) ) == 0 );

  /*------------------------------------------------
  Make sure the current control block does not equal the first
  ------------------------------------------------*/
  EXPECT_FALSE( memcmp( &ctrlBlk1, &pm->getControlBlock( key ), sizeof( ParamCtrlBlk ) ) == 0 );
}

TEST_F( PMTF, RegistrationBeforeInit )
{
  using namespace AeroKernel::Parameter;

  const std::string_view key = "helloWorld";
  ParamCtrlBlk ctrlBlk       = { 0, 0, 0, nullptr };
  EXPECT_EQ( false, pm->registerParameter( key, ctrlBlk ) );
}

/*------------------------------------------------
AeroKernel::Parameter::Manager::isRegistered()
------------------------------------------------*/
TEST_F( PMTF, isRegistered )
{
  using namespace AeroKernel::Parameter;

  const std::string_view key = "testing123";
  ParamCtrlBlk ctrlBlk       = { 0, 0, 0, nullptr };

  pm->init( 50 );

  EXPECT_EQ( false, pm->isRegistered( key ) );
  pm->registerParameter( key, ctrlBlk );
  EXPECT_EQ( true, pm->isRegistered( key ) );
}

/*------------------------------------------------
AeroKernel::Parameter::Manager::registerMemoryDriver()
------------------------------------------------*/
TEST_F( PMTF, registerMemoryDriver_invalidLocation )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  const std::string_view key = "helloWorld";
  ParamCtrlBlk ctrlBlk       = { 0, 0, 0, nullptr };
  Device_sPtr device         = InternalSRAM_VMD;


  pm->init( 50 );
  EXPECT_EQ( false, pm->registerMemoryDriver(MemoryLocation::MAX_MEMORY_LOCATIONS, device ) );
}

/*------------------------------------------------
AeroKernel::Parameter::Manager::registerMemorySpecs()
------------------------------------------------*/


/*------------------------------------------------
AeroKernel::Parameter::Manager::read()
------------------------------------------------*/


/*------------------------------------------------
AeroKernel::Parameter::Manager::write()
------------------------------------------------*/


/*------------------------------------------------
AeroKernel::Parameter::Manager::update()
------------------------------------------------*/


/*------------------------------------------------
AeroKernel::Parameter::Manager::remove()
------------------------------------------------*/