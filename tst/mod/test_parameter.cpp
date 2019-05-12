/********************************************************************************
 *  File Name:
 *    test_parameter.cpp
 *
 *  Description:
 *    Tests for the Parameter Manager
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <limits>
#include <functional>

/* Test Driver Includes */
#include <gtest/gtest.h>
#include <fixtures/parameter_test_fixture.hpp>

/* API Under Test */
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
  auto storedResult = pm->getControlBlock( key );
  EXPECT_EQ( memcmp( &ctrlBlk1, &storedResult, sizeof( ParamCtrlBlk ) ), 0 );

  /*------------------------------------------------
  Re-register the key and write the second control block
  ------------------------------------------------*/
  pm->registerParameter( key, ctrlBlk2 );
  storedResult = pm->getControlBlock( key );
  EXPECT_EQ( memcmp( &ctrlBlk2, &storedResult, sizeof( ParamCtrlBlk ) ), 0 );

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
AeroKernel::Parameter::Manager::unregisterParameter()
------------------------------------------------*/
TEST_F( PMTF, UnRegisterNormal )
{
  using namespace AeroKernel::Parameter;

  const std::string_view key = "IAmGroot";
  ParamCtrlBlk pcb           = { 0, 0, 0, nullptr };

  pm->init( 50 );
  pm->registerParameter( key, pcb );
  ASSERT_EQ( true, pm->isRegistered( key ) );
  EXPECT_EQ( true, pm->unregisterParameter( key ) );
}

TEST_F( PMTF, UnRegisterInvalidKey )
{
  const std::string_view key = "testing123";

  pm->init( 50 );
  EXPECT_EQ( false, pm->unregisterParameter( key ) );
}

TEST_F( PMTF, UnRegisterPreInitialization )
{
  const std::string_view key = "testing123";
  EXPECT_EQ( false, pm->unregisterParameter( key ) );
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

TEST_F( PMTF, isRegisteredPreInitialization )
{
  using namespace AeroKernel::Parameter;

  const std::string_view key = "testing123";
  ParamCtrlBlk ctrlBlk       = { 0, 0, 0, nullptr };

  EXPECT_EQ( false, pm->isRegistered( key ) );
  pm->registerParameter( key, ctrlBlk );
  EXPECT_EQ( false, pm->isRegistered( key ) );
}

/*------------------------------------------------
AeroKernel::Parameter::Manager::registerMemoryDriver()
------------------------------------------------*/
TEST_F( PMTF, registerMemoryDriver_invalidLocation )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  Device_sPtr device = InternalSRAM_VMD;

  pm->init( 50 );
  EXPECT_EQ( false, pm->registerMemoryDriver( Location::MAX_MEMORY_LOCATIONS, device ) );
}

TEST_F( PMTF, registerMemoryDriver_validLocation )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  Device_sPtr device = InternalSRAM_VMD;

  pm->init( 50 );
  EXPECT_EQ( true, pm->registerMemoryDriver( Location::INTERNAL_SRAM, device ) );
}

TEST_F( PMTF, registerMemoryDriver_preInitialization )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  Device_sPtr device = InternalSRAM_VMD;
  EXPECT_EQ( false, pm->registerMemoryDriver( Location::INTERNAL_SRAM, device ) );
}

/*------------------------------------------------
AeroKernel::Parameter::Manager::registerMemorySpecs()
------------------------------------------------*/
TEST_F( PMTF, registerMemorySpecs_invalidLocation )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  Device_sPtr device = InternalSRAM_VMD;

  pm->init( 50 );
  EXPECT_EQ( false, pm->registerMemorySpecs( Location::MAX_MEMORY_LOCATIONS, InternalSRAM_VMD->getSpecs() ) );
}

TEST_F( PMTF, registerMemorySpecs_validLocation )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  Device_sPtr device = InternalSRAM_VMD;

  pm->init( 50 );
  EXPECT_EQ( true, pm->registerMemorySpecs( Location::EXTERNAL_SRAM0, InternalSRAM_VMD->getSpecs() ) );
}

TEST_F( PMTF, registerMemorySpecs_preInitialization )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  Device_sPtr device = InternalSRAM_VMD;
  EXPECT_EQ( false, pm->registerMemorySpecs( Location::EXTERNAL_SRAM0, InternalSRAM_VMD->getSpecs() ) );
}

/*------------------------------------------------
AeroKernel::Parameter::Manager::read()
------------------------------------------------*/
TEST_F( PMTF, readPreInitialization )
{
  uint8_t someData = 0xaa;
  EXPECT_EQ( false, pm->read( "idk", &someData, sizeof( someData ) ) );
}

TEST_F( PMTF, readInvalidKey )
{
  uint8_t someData = 0xaa;

  pm->init( 50 );
  EXPECT_EQ( false, pm->read( "TotallyAValidKey...", &someData, sizeof( someData ) ) );
  EXPECT_EQ( someData, 0xaa );
}

TEST_F( PMTF, readUnregisteredMemoryDriver )
{
  using namespace AeroKernel::Parameter;

  uint8_t pod                = 0x85;
  const std::string_view key = "pod";
  ParamCtrlBlk cfg;

  cfg.address = 0x400;
  cfg.size    = sizeof( pod );
  cfg.config  = Location::INTERNAL_FLASH;
  cfg.update  = nullptr;

  pm->init( 50 );
  pm->registerParameter( key, cfg );
  EXPECT_EQ( false, pm->read( key, &pod, sizeof( pod ) ) );
  EXPECT_EQ( pod, 0x85 );
}

TEST_F( PMTF, readPOD )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  uint8_t pod                = 0x85;
  const std::string_view key = "a_cool_key";
  ParamCtrlBlk cfg;
  Device_sPtr driver = InternalFLASH_VMD;

  cfg.address = 0x100;
  cfg.size    = sizeof( pod );
  cfg.config  = Location::INTERNAL_FLASH;
  cfg.update  = nullptr;

  pm->init( 50 );
  pm->registerParameter( key, cfg );
  pm->registerMemoryDriver( Location::INTERNAL_FLASH, driver );

  EXPECT_EQ( true, pm->write( key, &pod, sizeof( pod ) ) );
  pod = 0x00;
  EXPECT_EQ( true, pm->read( key, &pod, sizeof( pod ) ) );
  EXPECT_EQ( pod, 0x85 );
}

TEST_F( PMTF, readOutOfBounds )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  uint8_t pod                = 0x85;
  const std::string_view key = "pod";
  ParamCtrlBlk cfg;
  Device_sPtr driver = InternalFLASH_VMD;

  cfg.address = std::numeric_limits<size_t>::max();
  cfg.size    = sizeof( pod );
  cfg.config  = Location::INTERNAL_FLASH;
  cfg.update  = nullptr;

  pm->init( 50 );
  pm->registerParameter( key, cfg );
  pm->registerMemoryDriver( Location::INTERNAL_FLASH, driver );
  EXPECT_EQ( false, pm->read( key, &pod, sizeof( pod ) ) );
}

TEST_F( PMTF, readSizingDifferentThanRegistered )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  uint8_t pod                = 0x85;
  const std::string_view key = "pod";
  ParamCtrlBlk cfg;
  Device_sPtr driver = InternalFLASH_VMD;

  cfg.address = std::numeric_limits<size_t>::max();
  cfg.size    = sizeof( pod );
  cfg.config  = Location::INTERNAL_FLASH;
  cfg.update  = nullptr;

  pm->init( 50 );
  pm->registerParameter( key, cfg );
  pm->registerMemoryDriver( Location::INTERNAL_FLASH, driver );

  EXPECT_EQ( false, pm->read( key, &pod, cfg.size * 2 ) );
}

/*------------------------------------------------
AeroKernel::Parameter::Manager::write()
------------------------------------------------*/
TEST_F( PMTF, writePreInitialization )
{
  uint8_t someData = 0xaa;
  EXPECT_EQ( false, pm->write( "idk", &someData, sizeof( someData ) ) );
}

TEST_F( PMTF, writeInvalidKey )
{
  uint8_t someData = 0xaa;

  pm->init( 50 );
  EXPECT_EQ( false, pm->write( "TotallyAValidKey...", &someData, sizeof( someData ) ) );
}

TEST_F( PMTF, writeUnregisteredMemoryDriver )
{
  using namespace AeroKernel::Parameter;

  uint8_t pod                = 0x85;
  const std::string_view key = "pod";
  ParamCtrlBlk cfg;

  cfg.address = 0x100;
  cfg.size    = sizeof( pod );
  cfg.config  = Location::INTERNAL_FLASH;
  cfg.update  = nullptr;

  pm->init( 50 );
  pm->registerParameter( key, cfg );
  EXPECT_EQ( false, pm->write( key, &pod, sizeof( pod ) ) );
}

TEST_F( PMTF, writePOD )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  uint8_t pod                = 0x85;
  const std::string_view key = "pod";
  ParamCtrlBlk cfg;
  Device_sPtr driver = InternalFLASH_VMD;

  cfg.address = 0x100;
  cfg.size    = sizeof( pod );
  cfg.config  = Location::INTERNAL_FLASH;
  cfg.update  = nullptr;

  pm->init( 50 );
  pm->registerParameter( key, cfg );
  pm->registerMemoryDriver( Location::INTERNAL_FLASH, driver );
  EXPECT_EQ( true, pm->write( key, &pod, sizeof( pod ) ) );
}

TEST_F( PMTF, writeOutOfBounds )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  uint8_t pod                = 0x85;
  const std::string_view key = "pod";
  ParamCtrlBlk cfg;
  Device_sPtr driver = InternalFLASH_VMD;

  cfg.address = std::numeric_limits<size_t>::max();
  cfg.size    = sizeof( pod );
  cfg.config  = Location::INTERNAL_FLASH;
  cfg.update  = nullptr;

  pm->init( 50 );
  pm->registerParameter( key, cfg );
  pm->registerMemoryDriver( Location::INTERNAL_FLASH, driver );
  EXPECT_EQ( false, pm->write( key, &pod, sizeof( pod ) ) );
}

TEST_F( PMTF, writeSizingDifferentThanRegistered )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  uint8_t pod                = 0x85;
  const std::string_view key = "pod";
  ParamCtrlBlk cfg;
  Device_sPtr driver = InternalFLASH_VMD;

  cfg.address = std::numeric_limits<size_t>::max();
  cfg.size    = sizeof( pod );
  cfg.config  = Location::INTERNAL_FLASH;
  cfg.update  = nullptr;

  pm->init( 50 );
  pm->registerParameter( key, cfg );
  pm->registerMemoryDriver( Location::INTERNAL_FLASH, driver );

  EXPECT_EQ( false, pm->write( key, &pod, cfg.size * 2 ) );
}

/*------------------------------------------------
AeroKernel::Parameter::Manager::update()
------------------------------------------------*/
TEST_F( PMTF, updatePreInitialization )
{
  EXPECT_EQ( false, pm->update( "idk" ) );
}

TEST_F( PMTF, updateInvalidKey )
{
  const std::string_view invalidKey = "1234IDeclareATugOfWar";

  pm->init( 50 );
  EXPECT_EQ( false, pm->update( invalidKey ) );
}

TEST_F( PMTF, updateValidKeyNoRegisteredUpdateFunc )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  uint8_t pod                = 0x85;
  const std::string_view key = "pod";
  ParamCtrlBlk cfg;
  Device_sPtr driver = InternalFLASH_VMD;

  cfg.address = 0x100;
  cfg.size    = sizeof( pod );
  cfg.config  = Location::INTERNAL_FLASH;
  cfg.update  = nullptr;

  pm->init( 50 );
  pm->registerParameter( key, cfg );
  pm->registerMemoryDriver( Location::INTERNAL_FLASH, driver );

  EXPECT_EQ( false, pm->update( key ) );
  EXPECT_EQ( pod, 0x85 );
}

TEST_F( PMTF, updateValidKeyWithRegisteredUpdateFunc )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;
  using namespace std::placeholders;

  uint8_t pod                = 0x85;
  const std::string_view key = "pod";
  ParamCtrlBlk cfg;
  Device_sPtr driver = InternalFLASH_VMD;

  cfg.address = 0x100;
  cfg.size    = sizeof( pod );
  cfg.config  = Location::INTERNAL_FLASH;
  cfg.update  = std::bind(&PMTF::updateProc1, this, _1);

  pm->init( 50 );
  pm->registerParameter( key, cfg );
  pm->registerMemoryDriver( Location::INTERNAL_FLASH, driver );

  EXPECT_EQ( true, pm->update( key ) );

  pm->read( key, &pod, sizeof( pod ) );
  EXPECT_EQ( pod, updateValue1 );
}