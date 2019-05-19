/********************************************************************************
 *  File Name:
 *    mb85rs64v_tests_stm32f4.cpp
 *
 *  Description:
 *    On device embedded tests for the MB85RS64V FRAM chip. Targeted for the STM32F4
 *    Nucleo development board.
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>

/* Boost Includes */
#include <boost/circular_buffer.hpp>

/* Test Driver Includes */
#include "TinyEmbeddedTest.h"

/* Chimera Includes */
#include "chimeraPort.hpp"
#include <Chimera/gpio.hpp>
#include <Chimera/spi.hpp>

/* Parameter Manager Includes */
#include <AeroKernel/parameter.hpp>
#include "mb85rs64v.hpp"

using namespace AeroKernel::Parameter;

static Chimera::SPI::SPIClass_sPtr spi_glbl;
static FRAM::Fujitsu::MB85RS64V_sPtr fram;
static Chimera::SPI::Setup setup;
static AeroKernel::Parameter::Manager mgr;

static constexpr size_t numParams = 10;
static bool driverAllocated = false;

struct DataType
{
  float a1;
  uint32_t b1;
  uint64_t c1;
  uint8_t d1;
};

static void
    reset_test()
{
  using namespace Chimera::Hardware;
  using namespace Chimera::SPI;
  using namespace Chimera::GPIO;

  if ( !driverAllocated )
  {
    spi_glbl = std::make_shared<Chimera::SPI::SPIClass>();
    fram = std::make_shared<FRAM::Fujitsu::MB85RS64V>();
    mgr.init( numParams );
    driverAllocated = true;
  }

  memset( &setup, 0, sizeof( Chimera::SPI::Setup ) );

  /*------------------------------------------------
  This should be the first group of four pins on CN7  
  for the STM32F4 Nucleo dev board.
  ------------------------------------------------*/
  setup.MOSI.port      = Port::PORTC;
  setup.MOSI.pin       = 12;
  setup.MOSI.alternate = GPIO_AF6_SPI3;

  setup.MISO.port      = Port::PORTC;
  setup.MISO.pin       = 11;
  setup.MISO.alternate = GPIO_AF6_SPI3;

  setup.SCK.port      = Port::PORTC;
  setup.SCK.pin       = 10;
  setup.SCK.alternate = GPIO_AF6_SPI3;

  setup.CS.port = Port::PORTD;
  setup.CS.pin  = 2;

  setup.channel = 3;
  setup.clockFrequency = 8000000;
  
  /*------------------------------------------------
  Initialize the FRAM sink
  ------------------------------------------------*/
  fram->attachSPI( spi_glbl );
  fram->initialize( &setup, Chimera::Hardware::SubPeripheralMode::BLOCKING );
  Chimera::Modules::Memory::Device_sPtr extFlash0 = fram;

  /*------------------------------------------------
  Initialize the parameter manager 
  ------------------------------------------------*/
  mgr.registerMemoryDriver( Location::EXTERNAL_FLASH0, extFlash0 );
}

TEST_GROUP( ParameterManager_FRAMSink ){};

TEST( ParameterManager_FRAMSink, parameterOperations )
{
  reset_test();

  std::string_view key = "yolo";

  ParamCtrlBlk cbWrite;

  cbWrite.address = 0x1234;
  cbWrite.config  = 5678;
  cbWrite.size    = 10;
  cbWrite.update  = nullptr;

  CHECK( mgr.registerParameter( key, cbWrite ) );
  CHECK( mgr.isRegistered( key ) );
  CHECK( mgr.unregisterParameter( key ) );
}

TEST( ParameterManager_FRAMSink, readWrite )
{
  reset_test();

  std::string_view key = "testStructure";
  DataType testParam;
  testParam.a1 = 3.14159f;
  testParam.b1 = 0x12345678;
  testParam.c1 = 0xFF00FF00FF00FF00;
  testParam.d1 = 0xAA;

  DataType testParamCopy;
  memset( &testParamCopy, 0, sizeof( DataType ) );

  ParamCtrlBlk testCB;

  testCB.address = 0x0000;
  testCB.config  = Location::EXTERNAL_FLASH0;
  testCB.size    = sizeof(DataType);
  testCB.update  = nullptr;

  CHECK( mgr.registerParameter( key, testCB ) );
  CHECK( mgr.write( key, &testParam ) );
  CHECK( mgr.read( key, &testParamCopy ) );
  CHECK( memcmp( &testParam, &testParamCopy, sizeof( DataType ) ) == 0 );
}