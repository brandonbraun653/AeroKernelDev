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
  ControlBlockFactory factory;
  factory.setAddress( 0 );
  factory.setSize( 0 );
  factory.setStorage( StorageType::INTERNAL_SRAM );
  factory.setUpdateCallback( nullptr );

  pm->init( 50 );

  EXPECT_EQ( true, pm->registerParameter( key, factory.build() ) );
}

TEST_F( PMTF, RegistrationOverwrite )
{
  using namespace AeroKernel::Parameter;

  const std::string_view key = "helloWorld";

  ControlBlockFactory factory;
  factory.setAddress( 0 );
  factory.setSize( 0 );
  factory.setStorage( StorageType::INTERNAL_SRAM );
  factory.setUpdateCallback( nullptr );
  ControlBlock ctrlBlk1 = factory.build();

  factory.setAddress( 1 );
  factory.setSize( 2 );
  factory.setStorage( StorageType::INTERNAL_FLASH );
  factory.setUpdateCallback( nullptr );
  ControlBlock ctrlBlk2 = factory.build();

  pm->init( 50 );

  /*------------------------------------------------
  Register the key and write the first control block
  ------------------------------------------------*/
  pm->registerParameter( key, ctrlBlk1 );
  auto storedResult = pm->getControlBlock( key );

  EXPECT_EQ( ControlBlockInterpreter::getAddress( ctrlBlk1 ), ControlBlockInterpreter::getAddress( storedResult ) );
  EXPECT_EQ( ControlBlockInterpreter::getStorage( ctrlBlk1 ), ControlBlockInterpreter::getStorage( storedResult ) );
  EXPECT_EQ( ControlBlockInterpreter::getSize( ctrlBlk1 ), ControlBlockInterpreter::getSize( storedResult ) );

  /*------------------------------------------------
  Re-register the key and write the second control block
  ------------------------------------------------*/
  pm->registerParameter( key, ctrlBlk2 );
  storedResult = pm->getControlBlock( key );
  
  EXPECT_EQ( ControlBlockInterpreter::getAddress( ctrlBlk2 ), ControlBlockInterpreter::getAddress( storedResult ) );
  EXPECT_EQ( ControlBlockInterpreter::getStorage( ctrlBlk2 ), ControlBlockInterpreter::getStorage( storedResult ) );
  EXPECT_EQ( ControlBlockInterpreter::getSize( ctrlBlk2 ), ControlBlockInterpreter::getSize( storedResult ) );

  /*------------------------------------------------
  Make sure the current control block does not equal the first
  ------------------------------------------------*/
  EXPECT_NE( ControlBlockInterpreter::getAddress( ctrlBlk1 ), ControlBlockInterpreter::getAddress( storedResult ) );
  EXPECT_NE( ControlBlockInterpreter::getStorage( ctrlBlk1 ), ControlBlockInterpreter::getStorage( storedResult ) );
  EXPECT_NE( ControlBlockInterpreter::getSize( ctrlBlk1 ), ControlBlockInterpreter::getSize( storedResult ) );
}

TEST_F( PMTF, RegistrationBeforeInit )
{
  using namespace AeroKernel::Parameter;

  const std::string_view key = "helloWorld";

  ControlBlockFactory factory;
  factory.setAddress( 0 );
  factory.setSize( 0 );
  factory.setStorage( StorageType::INTERNAL_SRAM );
  factory.setUpdateCallback( nullptr );

  EXPECT_EQ( false, pm->registerParameter( key, factory.build() ) );
}

/*------------------------------------------------
AeroKernel::Parameter::Manager::unregisterParameter()
------------------------------------------------*/
TEST_F( PMTF, UnRegisterNormal )
{
  using namespace AeroKernel::Parameter;

  const std::string_view key = "IAmGroot";
  ControlBlockFactory factory;
  factory.setAddress( 0 );
  factory.setSize( 0 );
  factory.setStorage( StorageType::INTERNAL_SRAM );
  factory.setUpdateCallback( nullptr );

  pm->init( 50 );
  pm->registerParameter( key, factory.build() );
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
  ControlBlockFactory factory;
  factory.setAddress( 0 );
  factory.setSize( 0 );
  factory.setStorage( StorageType::INTERNAL_SRAM );
  factory.setUpdateCallback( nullptr );

  pm->init( 50 );

  EXPECT_EQ( false, pm->isRegistered( key ) );
  pm->registerParameter( key, factory.build() );
  EXPECT_EQ( true, pm->isRegistered( key ) );
}

TEST_F( PMTF, isRegisteredPreInitialization )
{
  using namespace AeroKernel::Parameter;

  const std::string_view key = "testing123";
  ControlBlockFactory factory;
  factory.setAddress( 0 );
  factory.setSize( 0 );
  factory.setStorage( StorageType::INTERNAL_SRAM );
  factory.setUpdateCallback( nullptr );

  EXPECT_EQ( false, pm->isRegistered( key ) );
  pm->registerParameter( key, factory.build() );
  EXPECT_EQ( false, pm->isRegistered( key ) );
}

/*------------------------------------------------
AeroKernel::Parameter::Manager::registerMemoryDriver()
------------------------------------------------*/
TEST_F( PMTF, registerMemoryDriver_invalidStorageType )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  Device_sPtr device = InternalSRAM_VMD;

  pm->init( 50 );
  EXPECT_EQ( false, pm->registerMemoryDriver( StorageType::NONE, device ) );
}

TEST_F( PMTF, registerMemoryDriver_validStorageType )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  Device_sPtr device = InternalSRAM_VMD;

  pm->init( 50 );
  EXPECT_EQ( true, pm->registerMemoryDriver( StorageType::INTERNAL_SRAM, device ) );
}

TEST_F( PMTF, registerMemoryDriver_preInitialization )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  Device_sPtr device = InternalSRAM_VMD;
  EXPECT_EQ( false, pm->registerMemoryDriver( StorageType::INTERNAL_SRAM, device ) );
}

/*------------------------------------------------
AeroKernel::Parameter::Manager::registerMemorySpecs()
------------------------------------------------*/
TEST_F( PMTF, registerMemorySpecs_invalidStorageType )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  Device_sPtr device = InternalSRAM_VMD;

  pm->init( 50 );
  EXPECT_EQ( false, pm->registerMemorySpecs( StorageType::NONE, InternalSRAM_VMD->getSpecs() ) );
}

TEST_F( PMTF, registerMemorySpecs_validStorageType )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  Device_sPtr device = InternalSRAM_VMD;

  pm->init( 50 );
  EXPECT_EQ( true, pm->registerMemorySpecs( StorageType::EXTERNAL_SRAM0, InternalSRAM_VMD->getSpecs() ) );
}

TEST_F( PMTF, registerMemorySpecs_preInitialization )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  Device_sPtr device = InternalSRAM_VMD;
  EXPECT_EQ( false, pm->registerMemorySpecs( StorageType::EXTERNAL_SRAM0, InternalSRAM_VMD->getSpecs() ) );
}

/*------------------------------------------------
AeroKernel::Parameter::Manager::read()
------------------------------------------------*/
TEST_F( PMTF, readPreInitialization )
{
  uint8_t someData = 0xaa;
  EXPECT_EQ( false, pm->read( "idk", &someData ) );
}

TEST_F( PMTF, readInvalidKey )
{
  uint8_t someData = 0xaa;

  pm->init( 50 );
  EXPECT_EQ( false, pm->read( "TotallyAValidKey...", &someData ) );
  EXPECT_EQ( someData, 0xaa );
}

TEST_F( PMTF, readUnregisteredMemoryDriver )
{
  using namespace AeroKernel::Parameter;

  uint8_t pod                = 0x85;
  const std::string_view key = "pod";
  ControlBlockFactory factory;
  factory.setAddress( 0x400 );
  factory.setSize( sizeof(pod) );
  factory.setStorage( StorageType::INTERNAL_FLASH );
  factory.setUpdateCallback( nullptr );

  pm->init( 50 );
  pm->registerParameter( key, factory.build() );
  EXPECT_EQ( false, pm->read( key, &pod ) );
  EXPECT_EQ( pod, 0x85 );
}

TEST_F( PMTF, readPOD )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  uint8_t pod                = 0x85;
  const std::string_view key = "a_cool_key";
  Device_sPtr driver = InternalFLASH_VMD;

  ControlBlockFactory factory;
  factory.setAddress( 0x100 );
  factory.setSize( sizeof( pod ) );
  factory.setStorage( StorageType::INTERNAL_FLASH );
  factory.setUpdateCallback( nullptr );

  pm->init( 50 );
  pm->registerParameter( key, factory.build() );
  pm->registerMemoryDriver( StorageType::INTERNAL_FLASH, driver );

  EXPECT_EQ( true, pm->write( key, &pod ) );
  pod = 0x00;
  EXPECT_EQ( true, pm->read( key, &pod ) );
  EXPECT_EQ( pod, 0x85 );
}

TEST_F( PMTF, readOutOfBounds )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  uint8_t pod                = 0x85;
  const std::string_view key = "pod";
  Device_sPtr driver = InternalFLASH_VMD;

  ControlBlockFactory factory;
  factory.setAddress( std::numeric_limits<size_t>::max() );
  factory.setSize( sizeof( pod ) );
  factory.setStorage( StorageType::INTERNAL_FLASH );
  factory.setUpdateCallback( nullptr );

  pm->init( 50 );
  pm->registerParameter( key, factory.build() );
  pm->registerMemoryDriver( StorageType::INTERNAL_FLASH, driver );
  EXPECT_EQ( false, pm->read( key, &pod ) );
}

/*------------------------------------------------
AeroKernel::Parameter::Manager::write()
------------------------------------------------*/
TEST_F( PMTF, writePreInitialization )
{
  uint8_t someData = 0xaa;
  EXPECT_EQ( false, pm->write( "idk", &someData ) );
}

TEST_F( PMTF, writeInvalidKey )
{
  uint8_t someData = 0xaa;

  pm->init( 50 );
  EXPECT_EQ( false, pm->write( "TotallyAValidKey...", &someData ) );
}

TEST_F( PMTF, writeUnregisteredMemoryDriver )
{
  using namespace AeroKernel::Parameter;

  uint8_t pod                = 0x85;
  const std::string_view key = "pod";

  ControlBlockFactory factory;
  factory.setAddress( 0x100 );
  factory.setSize( sizeof( pod ) );
  factory.setStorage( StorageType::INTERNAL_FLASH );
  factory.setUpdateCallback( nullptr );

  pm->init( 50 );
  pm->registerParameter( key, factory.build() );
  EXPECT_EQ( false, pm->write( key, &pod ) );
}

TEST_F( PMTF, writePOD )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  uint8_t pod                = 0x85;
  const std::string_view key = "pod";
  Device_sPtr driver = InternalFLASH_VMD;

  ControlBlockFactory factory;
  factory.setAddress( 0x100 );
  factory.setSize( sizeof( pod ) );
  factory.setStorage( StorageType::INTERNAL_FLASH );
  factory.setUpdateCallback( nullptr );

  pm->init( 50 );
  pm->registerParameter( key, factory.build() );
  pm->registerMemoryDriver( StorageType::INTERNAL_FLASH, driver );
  EXPECT_EQ( true, pm->write( key, &pod ) );
}

TEST_F( PMTF, writeOutOfBounds )
{
  using namespace AeroKernel::Parameter;
  using namespace Chimera::Modules::Memory;

  uint8_t pod                = 0x85;
  const std::string_view key = "pod";
  Device_sPtr driver = InternalFLASH_VMD;

  ControlBlockFactory factory;
  factory.setAddress( std::numeric_limits<size_t>::max() );
  factory.setSize( sizeof( pod ) );
  factory.setStorage( StorageType::INTERNAL_FLASH );
  factory.setUpdateCallback( nullptr );

  pm->init( 50 );
  pm->registerParameter( key, factory.build() );
  pm->registerMemoryDriver( StorageType::INTERNAL_FLASH, driver );
  EXPECT_EQ( false, pm->write( key, &pod ) );
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
  Device_sPtr driver = InternalFLASH_VMD;

  ControlBlockFactory factory;
  factory.setAddress( 0x100 );
  factory.setSize( sizeof( pod ) );
  factory.setStorage( StorageType::INTERNAL_FLASH );
  factory.setUpdateCallback( nullptr );

  pm->init( 50 );
  pm->registerParameter( key, factory.build() );
  pm->registerMemoryDriver( StorageType::INTERNAL_FLASH, driver );

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
  Device_sPtr driver = InternalFLASH_VMD;

  ControlBlockFactory factory;
  factory.setAddress( 0x100 );
  factory.setSize( sizeof( pod ) );
  factory.setStorage( StorageType::INTERNAL_FLASH );
  factory.setUpdateCallback( std::bind(&PMTF::updateProc1, this, _1) );
  
  pm->init( 50 );
  pm->registerParameter( key, factory.build() );
  pm->registerMemoryDriver( StorageType::INTERNAL_FLASH, driver );

  EXPECT_EQ( true, pm->update( key ) );

  pm->read( key, &pod );
  EXPECT_EQ( pod, updateValue1 );
}