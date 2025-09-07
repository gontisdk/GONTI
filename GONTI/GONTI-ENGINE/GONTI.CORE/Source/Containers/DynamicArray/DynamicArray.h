#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../Defines/Defines.h"

            #define DARRAY_DEFAULT_CAPACITY 1
            #define DARRAY_RESIZE_FACTOR 2

            #define darrayCreate(type) \
                _gontiDarrayCreate(DARRAY_DEFAULT_CAPACITY, sizeof(type))
                
                #define darrayReserve(type, capacity) \
                    _gontiDarrayCreate(capacity, sizeof(type))

            #define darrayDestroy(array) \
                _gontiDarrayDestroy(array);

            #define darrayPush(array, value) { \
                typeof(value) temp = value; \
                array = _gontiDarrayPush(array, &temp); \
            }

            #define darrayPop(array, valuePtr) \
                _gontiDarrayPop(array, valuePtr)

            #define darrayInsertAt(array, index, value) { \
                typeof(value) temp = value; \
                array = _gontiDarrayInsertAt(array, index, &temp); \
            }

            #define darrayPopAt(array, index, valuePtr) \
                _gontiDarrayPopAt(array, index, valuePtr)

            #define darrayClear(array) \
                _gontiDarrayFieldSet(array, DARRAY_LENGTH, 0)

            #define darrayCapacity(array) \
                _gontiDarrayFieldGet(array, DARRAY_CAPACITY)

            #define darrayLength(array) \
                _gontiDarrayFieldGet(array, DARRAY_LENGTH)

            #define darrayStride(array) \
                _gontiDarrayFieldGet(array, DARRAY_STRIDE)

            #define darrayLengthSet(array, value) \
                _gontiDarrayFieldSet(array, DARRAY_LENGTH, value)

            enum {
                DARRAY_CAPACITY,
                DARRAY_LENGTH,
                DARRAY_STRIDE,
                DARRAY_FIELD_LENGTH
            };

            KAPI void* _gontiDarrayCreate(u64 length, u64 stride);
            KAPI void _gontiDarrayDestroy(void* array);
            KAPI u64 _gontiDarrayFieldGet(void* array, u64 field);
            KAPI void _gontiDarrayFieldSet(void* array, u64 field, u64 value);
            KAPI void* _gontiDarrayResize(void* array);
            KAPI void* _gontiDarrayPush(void* array, const void* valuePtr);
            KAPI void _gontiDarrayPop(void* array, void* dest);
            KAPI void* _gontiDarrayPopAt(void* array, u64 index, void* dest);
            KAPI void* _gontiDarrayInsertAt(void* array, u64 index, void* valuePtr);

    #ifdef __cplusplus
        }
    #endif

#endif