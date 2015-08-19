/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and
 * further review from Lawrence Livermore National Laboratory.
 */

/*!
 *******************************************************************************
 * \file Utility.cpp
 * \author Chris White (white238@llnl.gov)
 *
 * \brief This file contains the implementation of utility functions.
 *******************************************************************************
 */

#include "lumberjack/Utility.hpp"

#ifndef ENABLE_CXX11
#include <sstream>
#endif

namespace asctoolkit {
namespace lumberjack {

std::string intToString(int intValue)
{
    std::string stringValue = "";
#ifdef ENABLE_CXX11
    stringValue += std::to_string(intValue);
#else
    std::ostringstream ss;
    ss << intValue;
    stringValue += ss.str();
#endif
    return stringValue;
}

int stringToInt(const std::string& stringValue)
{
    int intValue = 0;
#ifdef ENABLE_CXX11
    intValue = stoi(stringValue);
#else
    std::istringstream(stringValue) >> intValue;
#endif
    return intValue;
}

} // end namespace lumberjack
} // end namespace asctoolkit
