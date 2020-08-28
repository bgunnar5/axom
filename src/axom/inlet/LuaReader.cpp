// Copyright (c) 2017-2020, Lawrence Livermore National Security, LLC and
// other Axom Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)

/*!
 *******************************************************************************
 * \file LuaReader.cpp
 *
 * \brief This file contains the class implementation of the LuaReader.
 *******************************************************************************
 */

# include <fstream>

#include "axom/inlet/LuaReader.hpp"

#include "axom/core/utilities/FileUtilities.hpp"
#include "axom/core/utilities/StringUtilities.hpp"
#include "axom/inlet/inlet_utils.hpp"

#include "fmt/fmt.hpp"
#include "axom/slic.hpp"

#define SOL_ALL_SAFETIES_ON 1

namespace axom
{
namespace inlet
{

bool LuaReader::parseFile(const std::string& filePath)
{
  if (!axom::utilities::filesystem::pathExists(filePath))
  {
    SLIC_WARNING(fmt::format("Inlet: Given Lua input deck does not exist: {0}",
                             filePath));
    return false;
  }

  auto script = m_lua.script_file(filePath);
  if (!script.valid()) {
    SLIC_WARNING(fmt::format("Inlet: Given Lua input deck does not exist: {0}",
                             filePath));
  }
  return script.valid();
}


bool LuaReader::parseString(const std::string& luaString)
{
  if (luaString.empty())
  {
    SLIC_WARNING("Inlet: Given an empty Lua string to parse.");
    return false;
  }
  m_lua.script(luaString);
  return true;
}

// TODO allow alternate delimiter at sidre level
#define SCOPE_DELIMITER '/'

bool LuaReader::getBool(const std::string& id, bool& value)
{
  return getValue(id, value);
}


bool LuaReader::getDouble(const std::string& id, double& value)
{
  return getValue(id, value);
}


bool LuaReader::getInt(const std::string& id, int& value)
{
  return getValue(id, value);
}


bool LuaReader::getString(const std::string& id, std::string& value)
{
  return getValue(id, value);
}

template <typename T>
bool LuaReader::getValue(const std::string& id, T& value) {
  std::vector<std::string> tokens;
  axom::utilities::string::split(tokens, id, SCOPE_DELIMITER);
  if (tokens.size() == 1 && m_lua[id].valid()) {
    value = m_lua[id];
    return true;
  }
  
  if (!m_lua[tokens[0]].valid()) {
    return false;
  }
  sol::table t = m_lua[tokens[0]];
  for (size_t i = 1; i < tokens.size()-1; i++) {
    if (t[tokens[i]].valid()) {
      t = t[tokens[i]];
    } else {
      return false;
    }
  }
  if (t[tokens.back()].valid()) {
    value = t[tokens.back()];
    return true;
  }
  
  return false;
}

} // end namespace inlet
} // end namespace axom
