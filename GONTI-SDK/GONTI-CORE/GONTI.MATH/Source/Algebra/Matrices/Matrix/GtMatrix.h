#ifndef GtMatrixH
#define GtMatrixH

#ifdef __cplusplus
	extern "C" {
#endif

		#include <GtMath.h>

		#include <stdarg.h>
		#include <stdio.h>
		#include <stdlib.h>
		#include <string.h>
		#include <math.h>

		// Files
		#include <Algebra/Matrices/GtMatTypes.inl>
		#include <Algebra/Vectors/GtVecTypes.inl>

		extern GTAPI const GtMat GT_MAT_UNDEFINED;

		/*MAT*/
		GTAPI GtMat gontiMatAllocate(GtU32 rows, GtU32 cols);
		GTAPI GtMat gontiMatIdentity(GtU32 dim);
		GTAPI GtMat gontiMatZeroMatrix(GtU32 rows, GtU32 cols);
		GTAPI GtMat gontiMatNewMatrix(GtU32 rows, GtU32 cols, GtU32 numVals, ...);
		GTAPI GtMat gontiMatNewMatrixFromArray(GtU32 rows, GtU32 cols, const GtF64* elements);
		GTAPI GtMat gontiMatNewMatrixSafe(GtU32 rows, GtU32 cols, GtF64* row_data[], GtU32 row_count);
		GTAPI GtMat gontiMatCopy(GtMat m);
		GTAPI GtMat gontiMatCopyPtr(GtMat* m);
		GTAPI GtMat gontiMatToRowVec(GtVec* v);
		GTAPI GtMat gontiMatToColVec(GtVec* v);
		GTAPI GtMat gontiMatScalarDivision(GtMat m, GtF32 k);
		GTAPI GtMat gontiMatAdd(GtMat m1, GtMat m2);
		GTAPI GtMat gontiMatMultiplication(GtMat m1, GtMat m2);
		GTAPI GtMat gontiMatTranspose(GtMat* m);
		GTAPI GtMat gontiMatArgumentVector(GtMat* m, GtVec* v);
		GTAPI GtMat gontiMatArgumentMatrix(GtMat* m1, GtMat* m2);
		GTAPI GtMat gontiMatSplice(GtMat* m, GtU32 exclRow, GtU32 exclCol);
		GTAPI GtMat gontiMatCofactorMatrix(GtMat* m);
		GTAPI GtMat gontiMatAdjugate(GtMat* m);
		GTAPI GtMat gontiMatInverseMat(GtMat* m);
		GTAPI GtMat gontiMatScalarMultiplication(GtMat m, GtF32 k);

		/*VEC*/
		GTAPI GtVec gontiMatGetRow(GtMat* m, GtU32 row);
		GTAPI GtVec gontiMatGetCol(GtMat* m, GtU32 col);
		GTAPI GtVec gontiMatMultiplicationVec(GtMat m, GtVec v);

		/*VOID*/
		GTAPI void gontiMatPrint(GtMat m);
		GTAPI void gontiMatScalarDivisionTo(GtMat* m, GtF32 k);
		GTAPI void gontiMatScalarMultiplicationBy(GtMat* m, GtF32 k);
		GTAPI void gontiMatFree(GtMat* m);
		GTAPI void gontiMatRef(GtMat* m);
		GTAPI void gontiMatRref(GtMat* m);

		/*BOOL*/
		GTAPI GtB8 gontiMatAddTo(GtMat* m1, GtMat m2);
		GTAPI GtB8 gontiMatSwapRows(GtMat* m, GtU32 r1, GtU32 r2);
		GTAPI GtB8 gontiMatAddRows(GtMat* m, GtU32 r1, GtU32 r2);
		GTAPI GtB8 gontiMatMultiplyRow(GtMat* m, GtU32 r1, GtF32 k);
		GTAPI GtB8 gontiMatAddMultipleRow(GtMat* m, GtU32 r1, GtU32 r2, GtF32 k);
		GTAPI GtB8 gontiMatInvertible(GtMat* m);

		/*FLOAT*/
		GTAPI GtF32 gontiMatDeterminant(GtMat* m);
		GTAPI GtF32 gontiMatDeterminantExclusion(GtMat* m);
		GTAPI GtF32 _gontiMatDeterminantExclusion(GtMat* m, GtU32 row, GtU32 col, GtU32* skipCols, GtU32* noSkipCols);
		GTAPI GtF32 gontiMatCofactor(GtMat* m, GtU32 r, GtU32 c);

		/*DEFINES*/

		//#define GtMatrix(rows, cols, ...) gontiMatNewMatrix(rows, cols, __VA_ARGS__)
		#define GtMatrix(rows, cols, ...) gontiMatNewMatrix(rows, cols, GT_NUMARGS(GtF32, __VA_ARGS__), ##__VA_ARGS__)

		#define GT_MAT_ROW(...) __VA_ARGS__
		#define GT_MAT_NEW(rows, cols, ...) gontiMatNewMatrix(rows, cols, __VA_ARGS__)
		#define GT_MAT_NEW_AR(rows, cols, ptr) gontiMatNewMatrixFromArray(rows, cols, ptr)

		#define GT_MAT_ROW_SF(...) (GtF64[]){__VA_ARGS__}
		#define GT_MAT_NEW_SF(rows, cols, ...) gontiMatNewMatrixSafe(rows, cols, (GtF64*[]){__VA_ARGS__}, rows)

#ifdef __cplusplus
	}
#endif

#endif