#ifndef GtDArrayH
#define GtDArrayH

    #ifdef __cplusplus
        extern "C"  {
    #endif

            #include <Defines/GtDefines.inl>

            #define GT_DARRAY_DEFAULT_CAPACITY 1
            #define GT_DARRAY_RESIZE_FACTOR 2

            #define gontiDarrayCreate(type) \
                _gontiDarrayCreate(GT_DARRAY_DEFAULT_CAPACITY, sizeof(type))
                
            #define gontiDarrayReserve(type, capacity) \
                _gontiDarrayCreate(capacity, sizeof(type))

            #define gontiDarrayDestroy(array) \
                _gontiDarrayDestroy(array);

            #define gontiDarrayPush(array, value) { \
                typeof(value) temp = value; \
                array = _gontiDarrayPush(array, &temp); \
            }

            #define gontiDarrayPop(array, valuePtr) \
                _gontiDarrayPop(array, valuePtr)

            #define gontiDarrayInsertAt(array, index, value) { \
                typeof(value) temp = value; \
                array = _gontiDarrayInsertAt(array, index, &temp); \
            }

            #define gontiDarrayPopAt(array, index, valuePtr) \
                _gontiDarrayPopAt(array, index, valuePtr)

            #define gontiDarrayClear(array) \
                _gontiDarrayFieldSet(array, GT_DARRAY_LENGTH, 0)

            #define gontiDarrayCapacity(array) \
                _gontiDarrayFieldGet(array, GT_DARRAY_CAPACITY)

            #define gontiDarrayLength(array) \
                _gontiDarrayFieldGet(array, GT_DARRAY_LENGTH)

            #define gontiDarrayStride(array) \
                _gontiDarrayFieldGet(array, GT_DARRAY_STRIDE)

            #define gontiDarrayLengthSet(array, value) \
                _gontiDarrayFieldSet(array, GT_DARRAY_LENGTH, value)

            enum {
                GT_DARRAY_CAPACITY,
                GT_DARRAY_LENGTH,
                GT_DARRAY_STRIDE,
                GT_DARRAY_FIELD_LENGTH
            };

            GTAPI void* _gontiDarrayCreate(GtU64 length, GtU64 stride);
            GTAPI void _gontiDarrayDestroy(void* array);
            GTAPI GtU64 _gontiDarrayFieldGet(void* array, GtU64 field);
            GTAPI void _gontiDarrayFieldSet(void* array, GtU64 field, GtU64 value);
            GTAPI void* _gontiDarrayResize(void* array);
            GTAPI void* _gontiDarrayPush(void* array, const void* valuePtr);
            GTAPI void _gontiDarrayPop(void* array, void* dest);
            GTAPI void* _gontiDarrayPopAt(void* array, GtU64 index, void* dest);
            GTAPI void* _gontiDarrayInsertAt(void* array, GtU64 index, void* valuePtr);

    #ifdef __cplusplus
        }
    #endif

#endif