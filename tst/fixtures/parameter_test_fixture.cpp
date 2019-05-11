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

VMD_sPtr InternalSRAM_VMD = std::make_shared<VirtualMemoryDevice>();
VMD_sPtr InternalFLASH_VMD = std::make_shared<VirtualMemoryDevice>();
VMD_sPtr ExternalFLASH0_VMD = std::make_shared<VirtualMemoryDevice>();
VMD_sPtr ExternalFLASH1_VMD = std::make_shared<VirtualMemoryDevice>();
VMD_sPtr ExternalFLASH2_VMD = std::make_shared<VirtualMemoryDevice>();
VMD_sPtr ExternalSRAM0_VMD = std::make_shared<VirtualMemoryDevice>();
VMD_sPtr ExternalSRAM1_VMD = std::make_shared<VirtualMemoryDevice>();
VMD_sPtr ExternalSRAM2_VMD = std::make_shared<VirtualMemoryDevice>();

void PMTF::reset_test()
{
  InternalSRAM_VMD->erase( 0, InternalSRAM_ByteSize );
}
