#include <Containers/DynamicArray/GtDArray.h>

#include <Memory/GtMemory.h>
#include <Logging/GtLogger.h>

void* _gontiDarrayCreate(GtU64 length, GtU64 stride) {
    GtU64 headerSize = GT_DARRAY_FIELD_LENGTH * sizeof(GtU64);
    GtU64 arraySize = length * stride;
    GtU64* newArray = gt_allocate(headerSize + arraySize, GT_MEM_TAG_DARRAY);

    gt_setMemory(newArray, 0, headerSize + arraySize);

    newArray[GT_DARRAY_CAPACITY] = length;
    newArray[GT_DARRAY_LENGTH] = 0;
    newArray[GT_DARRAY_STRIDE] = stride;

    return (void*)(newArray + GT_DARRAY_FIELD_LENGTH);
}
void _gontiDarrayDestroy(void* array) {
    GtU64* header = (GtU64*)array - GT_DARRAY_FIELD_LENGTH;
    //GtU64 headerSize = GT_DARRAY_FIELD_LENGTH * sizeof(GtU64);
    //GtU64 arraySize = header[GT_DARRAY_CAPACITY] * header[GT_DARRAY_STRIDE];
    //GtU64 totalSize = headerSize + arraySize;

    gt_free(header);
}
GtU64 _gontiDarrayFieldGet(void* array, GtU64 field) {
    GtU64* header = (GtU64*)array - GT_DARRAY_FIELD_LENGTH;

    return header[field];
}
void _gontiDarrayFieldSet(void* array, GtU64 field, GtU64 value) {
    GtU64* header = (GtU64*)array - GT_DARRAY_FIELD_LENGTH;
    header[field] = value;
}
void* _gontiDarrayResize(void* array) {
    GtU64 length = gontiDarrayLength(array);
    GtU64 stride = gontiDarrayStride(array);

    void* new = _gontiDarrayCreate((GT_DARRAY_RESIZE_FACTOR * gontiDarrayCapacity(array)), stride);
    gt_copyMemory(new, array, length * stride);

    _gontiDarrayFieldSet(new, GT_DARRAY_LENGTH, length);
    _gontiDarrayDestroy(array);
    return new;
}
void* _gontiDarrayPush(void* array, const void* valuePtr) {
    GtU64 length = gontiDarrayLength(array);
    GtU64 stride = gontiDarrayStride(array);

    if (length >= gontiDarrayCapacity(array)) {
        array = _gontiDarrayResize(array);
    }

    GtU64 addr = (GtU64)array;
    addr += length * stride;

    gt_copyMemory((void*)addr, valuePtr, stride);
    _gontiDarrayFieldSet(array, GT_DARRAY_LENGTH, length + 1);

    return array;
}
void _gontiDarrayPop(void* array, void* dest) {
    GtU64 length = gontiDarrayLength(array);
    GtU64 stride = gontiDarrayStride(array);

    GtU64 addr = (GtU64)array;
    addr += ((length - 1) * stride);

    gt_copyMemory(dest, (void*)addr, stride);
    _gontiDarrayFieldSet(array, GT_DARRAY_LENGTH, length - 1);
}
void* _gontiDarrayPopAt(void* array, GtU64 index, void* dest) {
    GtU64 length = gontiDarrayLength(array);
    GtU64 stride = gontiDarrayStride(array);

    if (index >= length) {
        GTERROR("Index outside the bounds of this array! Length: %i, Index %i", length, index);
        return array;
    }

    GtU64 addr = (GtU64)array;
    gt_copyMemory(dest, (void*)(addr + (index * stride)), stride);

    if (index != length - 1) {
        gt_copyMemory(
            (void*)(addr + (index * stride)),
            (void*)(addr + ((index + 1) * stride)),
            stride * (length - index - 1)
        );
    }

    _gontiDarrayFieldSet(array, GT_DARRAY_LENGTH, length - 1);

    return array;
}
void* _gontiDarrayInsertAt(void* array, GtU64 index, void* valuePtr) {
    GtU64 length = gontiDarrayLength(array);
    GtU64 stride = gontiDarrayStride(array);

    if (index > length) {
        GTERROR("Index outside the bounds of this array! Length: %i, Index %i", length, index);
        return array;
    }

    if (length >= gontiDarrayCapacity(array)) {
        array = _gontiDarrayResize(array);
    }

    GtU64 addr = (GtU64)array;

    if (index < length) {
        gt_copyMemory(
            (void*)(addr + ((index + 1) * stride)),
            (void*)(addr + (index * stride)),
            stride * (length - index - 1)
        );
    }

    gt_copyMemory((void*)(addr + (index * stride)), valuePtr, stride);
    _gontiDarrayFieldSet(array, GT_DARRAY_LENGTH, length + 1);

    return array;
}