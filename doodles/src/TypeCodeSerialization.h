/* (c) Copyright, Real-Time Innovations, 2013.  All rights reserved.
 * Author:  Gerardo Pardo-Castellote
 */

#ifndef TYPECODESERIALIZATION_H_
#define TYPECODESERIALIZATION_H_

#include "ndds/ndds_cpp.h"

class TypeCodeSerialization {
public:
    static DDS_UnsignedLong get_serialized_Typecode_size(const DDS_TypeCode *typeCode);
    static bool serialize_TypeCode(char *buffer, unsigned int bufferLen, const DDS_TypeCode *typeCode);
    static DDS_TypeCode *deserialize_TypeCode(const char *serializedTypeCode, int serializedTypeCodeLen);
};

#endif /* TYPECODESERIALIZATION_H_ */
