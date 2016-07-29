/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and further
 * review from Lawrence Livermore National Laboratory.
 */


/*
 * $Id$
 */

/*!
 *******************************************************************************
 * \file FieldTypes.hpp
 *
 * \date Dec 14, 2015
 * \author George Zagaris (zagaris2@llnl.gov)
 *******************************************************************************
 */

#ifndef FIELDTYPES_HPP_
#define FIELDTYPES_HPP_


namespace mint {

enum FieldTypes {
    UNDEFINED_FIELD_TYPE =-1,

    DOUBLE_FIELD_TYPE,
    INTEGER_FIELD_TYPE,

    NUMBER_OF_FIELD_TYPES
};


template< typename FieldType >
struct field_of {
    static const int type = UNDEFINED_FIELD_TYPE;
};

template < >
struct field_of< double > {
    static const int type = DOUBLE_FIELD_TYPE;
};

template < >
struct field_of< int > {
    static const int type = INTEGER_FIELD_TYPE;
};

}


#endif /* FIELDTYPES_HPP_ */
