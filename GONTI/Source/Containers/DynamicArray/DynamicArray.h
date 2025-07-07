#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../__GONTI_INCLUDES.h"
            
            #ifdef DynamicArray_ON

                #define DARRAY_DEFAULT_CAPACITY 1
                #define DARRAY_RESIZE_FACTOR 2

                #define darrayCreate(type) \
                    _darrayCreate(DARRAY_DEFAULT_CAPACITY, sizeof(type))
                
                #define darrayReserve(type, capacity) \
                    _darrayCreate(capacity, sizeof(type))

                #define darrayDestroy(array) \
                    _darrayDestroy(array);

                #define darrayPush(array, value) { \
                    typeof(value) temp = value; \
                    array = _darrayPush(array, &temp); \
                }

                #define darrayPop(array, valuePtr) \
                    _darrayPop(array, valuePtr)

                #define darrayInsertAt(array, index, value) { \
                    typeof(value) temp = value; \
                    array = _darrayInsertAt(array, index, &temp); \
                }

                #define darrayPopAt(array, index, valuePtr) \
                    _darrayPopAt(array, index, valuePtr)

                #define darrayClear(array) \
                    _darrayFieldSet(array, DARRAY_LENGTH, 0)

                #define darrayCapacity(array) \
                    _darrayFieldGet(array, DARRAY_CAPACITY)

                #define darrayLength(array) \
                    _darrayFieldGet(array, DARRAY_LENGTH)

                #define darrayStride(array) \
                    _darrayFieldGet(array, DARRAY_STRIDE)

                #define darrayLengthSet(array, value) \
                    _darrayFieldSet(array, DARRAY_LENGTH, value)

                enum {
                    DARRAY_CAPACITY,
                    DARRAY_LENGTH,
                    DARRAY_STRIDE,
                    DARRAY_FIELD_LENGTH
                };

                KAPI void* _darrayCreate(u64 length, u64 stride);
                KAPI void _darrayDestroy(void* array);
                KAPI u64 _darrayFieldGet(void* array, u64 field);
                KAPI void _darrayFieldSet(void* array, u64 field, u64 value);
                KAPI void* _darrayResize(void* array);
                KAPI void* _darrayPush(void* array, const void* valuePtr);
                KAPI void _darrayPop(void* array, void* dest);
                KAPI void* _darrayPopAt(void* array, u64 index, void* dest);
                KAPI void* _darrayInsertAt(void* array, u64 index, void* valuePtr);

            #endif

    #ifdef __cplusplus
        }
    #endif

#endif