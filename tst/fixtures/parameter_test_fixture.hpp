/********************************************************************************
 *  File Name:
 *    parameter_test_fixture.hpp
 *
 *  Description:
 *    Resources for testing the AeroKernel Parameter Manager
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef PARAMETER_MANAGER_TEST_FIXTURE_HPP
#define PARAMETER_MANAGER_TEST_FIXTURE_HPP

#include <gtest/gtest.h>
#include <AeroKernel/parameter.hpp>
#include <Chimera/modules/memory/device.hpp>

class PMTF : public ::testing::Test
{
public:
  static constexpr uint8_t updateValue1 = 0x33;
  bool updateProc1(const std::string_view &key);

protected:
  virtual void SetUp() override
  {
    pm = new AeroKernel::Parameter::Manager();
    reset_test();
  }

  virtual void TearDown() override
  {
    delete pm;
  }

  void reset_test();

  AeroKernel::Parameter::Manager *pm = nullptr;
};

/*------------------------------------------------
Virtual Memory Region Resources
------------------------------------------------*/
static constexpr size_t KB = 1024;
static constexpr size_t MB = 1024 * 1024;

static constexpr size_t InternalSRAM_ByteSize   = 4 * KB;
static constexpr size_t InternalFLASH_ByteSize  = 4 * KB;
static constexpr size_t ExternalFLASH0_ByteSize = 1 * MB;
static constexpr size_t ExternalFLASH1_ByteSize = 512 * KB;
static constexpr size_t ExternalFLASH2_ByteSize = 64 * KB;
static constexpr size_t ExternalSRAM0_ByteSize  = 512 * KB;
static constexpr size_t ExternalSRAM1_ByteSize  = 64 * KB;
static constexpr size_t ExternalSRAM2_ByteSize  = 8 * KB;

extern Chimera::Modules::Memory::VMD_sPtr InternalSRAM_VMD;
extern Chimera::Modules::Memory::VMD_sPtr InternalFLASH_VMD;
extern Chimera::Modules::Memory::VMD_sPtr ExternalFLASH0_VMD;
extern Chimera::Modules::Memory::VMD_sPtr ExternalFLASH1_VMD;
extern Chimera::Modules::Memory::VMD_sPtr ExternalFLASH2_VMD;
extern Chimera::Modules::Memory::VMD_sPtr ExternalSRAM0_VMD;
extern Chimera::Modules::Memory::VMD_sPtr ExternalSRAM1_VMD;
extern Chimera::Modules::Memory::VMD_sPtr ExternalSRAM2_VMD;

#endif /* !PARAMETER_MANAGER_TEST_FIXTURE_HPP */