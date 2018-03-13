// wrapSLIC.cpp
// This is generated code, do not edit
//
// Copyright (c) 2017-2018, Lawrence Livermore National Security, LLC.
//
// Produced at the Lawrence Livermore National Laboratory
//
// LLNL-CODE-741217
//
// All rights reserved.
//
// This file is part of Axom.
//
// For details about use and distribution, please read axom/LICENSE.
//
// wrapSLIC.cpp
#include "wrapSLIC.h"
#include <cstring>
#include <string>
#include "shroudrt.hpp"
#include "slic/slic.hpp"

namespace axom
{
namespace slic
{

// splicer begin CXX_definitions
// splicer end CXX_definitions

extern "C" {

// splicer begin C_definitions
// splicer end C_definitions

void SLIC_initialize()
{
// splicer begin function.initialize
  initialize();
  return;
// splicer end function.initialize
}

bool SLIC_is_initialized()
{
// splicer begin function.is_initialized
  bool SH_rv = isInitialized();
  return SH_rv;
// splicer end function.is_initialized
}

void SLIC_finalize()
{
// splicer begin function.finalize
  finalize();
  return;
// splicer end function.finalize
}

void SLIC_create_logger(const char* name, char imask)
{
// splicer begin function.create_logger
  const std::string SH_name(name);
  createLogger(SH_name, imask);
  return;
// splicer end function.create_logger
}

void SLIC_create_logger_bufferify(const char* name, int Lname, char imask)
{
// splicer begin function.create_logger_bufferify
  const std::string SH_name(name, Lname);
  createLogger(SH_name, imask);
  return;
// splicer end function.create_logger_bufferify
}

bool SLIC_activate_logger(const char* name)
{
// splicer begin function.activate_logger
  const std::string SH_name(name);
  bool SH_rv = activateLogger(SH_name);
  return SH_rv;
// splicer end function.activate_logger
}

bool SLIC_activate_logger_bufferify(const char* name, int Lname)
{
// splicer begin function.activate_logger_bufferify
  const std::string SH_name(name, Lname);
  bool SH_rv = activateLogger(SH_name);
  return SH_rv;
// splicer end function.activate_logger_bufferify
}

void SLIC_get_active_logger_name_bufferify(char* name, int Nname)
{
// splicer begin function.get_active_logger_name_bufferify
  std::string SH_rv = getActiveLoggerName();
  if (SH_rv.empty())
  {
    std::memset(name, ' ', Nname);
  }
  else
  {
    shroud_FccCopy(name, Nname, SH_rv.c_str());
  }
  return;
// splicer end function.get_active_logger_name_bufferify
}

void SLIC_set_logging_msg_level(int level)
{
// splicer begin function.set_logging_msg_level
  setLoggingMsgLevel(static_cast< message::Level >(level));
  return;
// splicer end function.set_logging_msg_level
}

void SLIC_set_abort_on_error(bool status)
{
// splicer begin function.set_abort_on_error
  setAbortOnError(status);
  return;
// splicer end function.set_abort_on_error
}

void SLIC_enable_abort_on_error()
{
// splicer begin function.enable_abort_on_error
  enableAbortOnError();
  return;
// splicer end function.enable_abort_on_error
}

void SLIC_disable_abort_on_error()
{
// splicer begin function.disable_abort_on_error
  disableAbortOnError();
  return;
// splicer end function.disable_abort_on_error
}

bool SLIC_is_abort_on_errors_enabled()
{
// splicer begin function.is_abort_on_errors_enabled
  bool SH_rv = isAbortOnErrorsEnabled();
  return SH_rv;
// splicer end function.is_abort_on_errors_enabled
}

void SLIC_set_abort_on_warning(bool status)
{
// splicer begin function.set_abort_on_warning
  setAbortOnWarning(status);
  return;
// splicer end function.set_abort_on_warning
}

void SLIC_enable_abort_on_warning()
{
// splicer begin function.enable_abort_on_warning
  enableAbortOnWarning();
  return;
// splicer end function.enable_abort_on_warning
}

void SLIC_disable_abort_on_warning()
{
// splicer begin function.disable_abort_on_warning
  disableAbortOnWarning();
  return;
// splicer end function.disable_abort_on_warning
}

bool SLIC_is_abort_on_warnings_enabled()
{
// splicer begin function.is_abort_on_warnings_enabled
  bool SH_rv = isAbortOnWarningsEnabled();
  return SH_rv;
// splicer end function.is_abort_on_warnings_enabled
}

void SLIC_log_message(int level, const char* message, const char* fileName,
                      int line, bool filter)
{
// splicer begin function.log_message
  const std::string SH_message(message);
  const std::string SH_fileName(fileName);
  logMessage(static_cast< message::Level >(level), SH_message, SH_fileName,
             line, filter);
  return;
// splicer end function.log_message
}

void SLIC_log_message_bufferify(int level, const char* message, int Lmessage,
                                const char* fileName, int LfileName, int line,
                                bool filter)
{
// splicer begin function.log_message_bufferify
  const std::string SH_message(message, Lmessage);
  const std::string SH_fileName(fileName, LfileName);
  logMessage(static_cast< message::Level >(level), SH_message, SH_fileName,
             line, filter);
  return;
// splicer end function.log_message_bufferify
}

}  // extern "C"

}  // namespace slic
}  // namespace axom
