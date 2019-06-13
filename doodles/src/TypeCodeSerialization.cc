/* (c) Copyright, Real-Time Innovations, 2013.  All rights reserved.
 * Author:  Gerardo Pardo-Castellote
 */

#include <stdio.h>
#include <stdlib.h>

#include "TypeCodeSerialization.h"

DDS_UnsignedLong TypeCodeSerialization::get_serialized_Typecode_size(const DDS_TypeCode *typeCode) {
    DDS_ExceptionCode_t ex;
    return typeCode->get_serialized_size(ex);
}

bool TypeCodeSerialization::serialize_TypeCode(
        char *buffer,
        unsigned int bufferLen,
        const DDS_TypeCode *typeCode)
{
    if ( bufferLen < get_serialized_Typecode_size(typeCode) ) {
        printf("TypeCodeSerialization::serialize_TypeCode: output buffer is not large enough. "
                "Needed %d bytes, got %d bytes\n", get_serialized_Typecode_size(typeCode), bufferLen);
        return false;
    }

    RTICdrStream stream;
    RTICdrStream_init(&stream);
    RTICdrStream_set(&stream, buffer, bufferLen);
    // We do not want any limits. Assigning (-1) to an unsigned int
    //   results in the maximum unsigned int
    unsigned int serializedTypecodeMaxSize = -1;
    return RTICdrTypeCode_serialize(
                    NULL,
                    &typeCode->_data, &stream,
                    RTI_TRUE, RTI_CDR_ENCAPSULATION_ID_CDR_NATIVE,
                    RTI_TRUE,  /* Serialize the data on the 2nd parameter (typeCode)  */
                    &serializedTypecodeMaxSize /* Must be a pointer to the length of the stream */);
}

DDS_TypeCode *TypeCodeSerialization::deserialize_TypeCode(
        const char *serializedTypeCode,
        int serializedTypeCodeLen)
{
    char *buffer = (char *)malloc(serializedTypeCodeLen);

    if (!RTICdrTypeCode_initialize_stream((struct RTICdrTypeCode *)buffer, serializedTypeCodeLen) ) {
         printf("TypeCodeSerialization::deserialize_TypeCode: type code buffer size "
                 " is not large enough\n");
         return NULL;
    }

    RTICdrTypeCode_copy_stream(
            (struct RTICdrTypeCode *)buffer,
            (struct RTICdrTypeCode *)serializedTypeCode);

    RTICdrTypeCode *cdrTypeCode = (struct RTICdrTypeCode *)buffer;
    DDS_TypeCode *typeCode = (DDS_TypeCode *)cdrTypeCode;

    return typeCode;
}



