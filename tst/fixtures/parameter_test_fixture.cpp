/********************************************************************************
 *  File Name:
 *    parameter_test_fixture.cpp
 *
 *  Description:
 *    Resources for testing the AeroKernel Parameter Manager
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#include <fixtures/parameter_test_fixture.hpp>

using namespace Chimera::Modules::Memory;

static std::array<uint8_t, InternalSRAM_ByteSize> mock_internal_sram_region;
static std::array<uint8_t, InternalFLASH_ByteSize> mock_internal_flash_region;
static std::array<uint8_t, ExternalFLASH0_ByteSize> mock_external_flash0_region;
static std::array<uint8_t, ExternalFLASH1_ByteSize> mock_external_flash1_region;
static std::array<uint8_t, ExternalFLASH2_ByteSize> mock_external_flash2_region;
static std::array<uint8_t, ExternalSRAM0_ByteSize> mock_external_sram0_region;
static std::array<uint8_t, ExternalSRAM1_ByteSize> mock_external_sram1_region;
static std::array<uint8_t, ExternalSRAM2_ByteSize> mock_external_sram2_region;

VMD_sPtr InternalSRAM_VMD   = std::make_shared<VirtualMemoryDevice>();
VMD_sPtr InternalFLASH_VMD  = std::make_shared<VirtualMemoryDevice>();
VMD_sPtr ExternalFLASH0_VMD = std::make_shared<VirtualMemoryDevice>();
VMD_sPtr ExternalFLASH1_VMD = std::make_shared<VirtualMemoryDevice>();
VMD_sPtr ExternalFLASH2_VMD = std::make_shared<VirtualMemoryDevice>();
VMD_sPtr ExternalSRAM0_VMD  = std::make_shared<VirtualMemoryDevice>();
VMD_sPtr ExternalSRAM1_VMD  = std::make_shared<VirtualMemoryDevice>();
VMD_sPtr ExternalSRAM2_VMD  = std::make_shared<VirtualMemoryDevice>();

void PMTF::reset_test()
{
  InternalSRAM_VMD->initialize( mock_internal_sram_region );
  InternalSRAM_VMD->erase( 0, InternalSRAM_ByteSize );

  InternalFLASH_VMD->initialize( mock_internal_flash_region );
  InternalFLASH_VMD->erase( 0, InternalFLASH_ByteSize );
  
  ExternalFLASH0_VMD->initialize( mock_external_flash0_region );
  ExternalFLASH0_VMD->erase( 0, ExternalFLASH0_ByteSize );
  
  ExternalFLASH1_VMD->initialize( mock_external_flash1_region );
  ExternalFLASH1_VMD->erase( 0, ExternalFLASH1_ByteSize );
  
  ExternalFLASH2_VMD->initialize( mock_external_flash2_region );
  ExternalFLASH2_VMD->erase( 0, ExternalFLASH2_ByteSize );
  
  ExternalSRAM0_VMD->initialize( mock_external_sram0_region );
  ExternalSRAM0_VMD->erase( 0, ExternalSRAM0_ByteSize );
  
  ExternalSRAM1_VMD->initialize( mock_external_sram1_region );
  ExternalSRAM1_VMD->erase( 0, ExternalSRAM1_ByteSize );
  
  ExternalSRAM2_VMD->initialize( mock_external_sram2_region );
  ExternalSRAM2_VMD->erase( 0, ExternalSRAM2_ByteSize );
}

bool PMTF::updateProc1( const std::string_view &key )
{
  return pm->write( key, &updateValue1 );
}
