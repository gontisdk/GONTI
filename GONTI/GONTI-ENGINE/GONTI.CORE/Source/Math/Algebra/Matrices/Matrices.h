#ifndef MATRICES_H
#define MATRICES_H

#ifdef __cplusplus
extern "C" {
#endif

	/*=============================================={[ CODE ]}==============================================*/

	#include "../../Maths.h"

	#include "stdarg.h"
	#include "stdio.h"
	#include "stdlib.h"
	#include "string.h"
	#include "math.h"


	// Files
	#include "../Vectors/Vectors.h"

	/*type mat*/

	typedef struct {
		unsigned int rows;
		unsigned int cols;
		float** elements;
	}mat;

	extern KAPI const mat MAT_UNDEFINED;

	/*MAT*/
	KAPI mat gontiMatAllocate(unsigned int rows, unsigned int cols);
	KAPI mat gontiMatIdentity(unsigned int dim);
	KAPI mat gontiMatZeroMatrix(unsigned int rows, unsigned int cols);
	KAPI mat gontiMatNewMatrix(unsigned int rows, unsigned int cols, unsigned int numVals, ...);
	KAPI mat gontiMatNewMatrixFromArray(unsigned int rows, unsigned int cols, const double* elements);
	KAPI mat gontiMatNewMatrixSafe(unsigned int rows, unsigned int cols, double* row_data[], unsigned int row_count);
	KAPI mat gontiMatCopy(mat m);
	KAPI mat gontiMatCopyPtr(mat* m);
	KAPI mat gontiMatToRowVec(vec* v);
	KAPI mat gontiMatToColVec(vec* v);
	KAPI mat gontiMatScalarDivision(mat m, float k);
	KAPI mat gontiMatAdd(mat m1, mat m2);
	KAPI mat gontiMatMultiplicationMat(mat m1, mat m2);
	KAPI mat gontiMatTranspose(mat* m);
	KAPI mat gontiMatArgumentVector(mat* m, vec* v);
	KAPI mat gontiMatArgumentMatrix(mat* m1, mat* m2);
	KAPI mat gontiMatSplice(mat* m, unsigned int exclRow, unsigned int exclCol);
	KAPI mat gontiMatCofactorMatrix(mat* m);
	KAPI mat gontiMatAdjugate(mat* m);
	KAPI mat gontiMatInverseMat(mat* m);
	KAPI mat gontiMatScalarMultiplication(mat m, float k);

	/*VEC*/
	KAPI vec gontiMatGetRow(mat* m, unsigned int row);
	KAPI vec gontiMatGetCol(mat* m, unsigned int col);
	KAPI vec gontiMatMultiplicationVec(mat m, vec v);

	/*VOID*/
	KAPI void gontiMatPrint(mat m);
	KAPI void gontiMatScalarDivisionTo(mat* m, float k);
	KAPI void gontiMatScalarMultiplicationBy(mat* m, float k);
	KAPI void gontiMatFree(mat* m);
	KAPI void gontiMatRef(mat* m);
	KAPI void gontiMatRref(mat* m);

	/*BOOL*/
	KAPI bool gontiMatAddTo(mat* m1, mat m2);
	KAPI bool gontiMatSwapRows(mat* m, unsigned int r1, unsigned int r2);
	KAPI bool gontiMatAddRows(mat* m, unsigned int r1, unsigned int r2);
	KAPI bool gontiMatMultiplyRow(mat* m, unsigned int r1, float k);
	KAPI bool gontiMatAddMultipleRow(mat* m, unsigned int r1, unsigned int r2, float k);
	KAPI bool gontiMatInvertible(mat* m);

	/*FLOAT*/
	KAPI float gontiMatDeterminant(mat* m);
	KAPI float gontiMatDeterminantExclusion(mat* m);
	KAPI float _gontiMatDeterminantExclusion(mat* m, unsigned int row, unsigned int col, unsigned int* skipCols, unsigned int* noSkipCols);
	KAPI float gontiMatCofactor(mat* m, unsigned int r, unsigned int c);

	/*gonti_matrix*/

	typedef struct {
		/*MAT*/
		mat(*allocate)(unsigned int rows, unsigned int cols);
		mat(*identity)(unsigned int dim);
		mat(*zeroMatrix)(unsigned int rows, unsigned int cols);
		mat(*newMatrix)(unsigned int rows, unsigned int cols, unsigned int numVals, ...);
		mat(*newMatrixFromArray)(unsigned int rows, unsigned int cols, const double* elements);
		mat(*newMatrixSafe)(unsigned int rows, unsigned int cols, double* row_data[], unsigned int row_count);
		mat(*copy)(mat m);
		mat(*copyPtr)(mat* m);
		mat(*toRowVec)(vec* v);
		mat(*toColVec)(vec* v);
		mat(*scalarDivision)(mat m, float k);
		mat(*add)(mat m1, mat m2);
		mat(*multiplicationMat)(mat m1, mat m2);
		mat(*transpose)(mat* m);
		mat(*argumentVector)(mat* m, vec* v);
		mat(*argumentMatrix)(mat* m1, mat* m2); 
		mat(*splice)(mat* m, unsigned int exclRow, unsigned int exclCol);
		mat(*cofactorMatrix)(mat* m);
		mat(*adjugate)(mat* m);
		mat(*inverseMat)(mat* m);
		mat(*scalarMultiplication)(mat m, float k);

		/*VEC*/
		vec(*getRow)(mat* m, unsigned int row);
		vec(*getCol)(mat* m, unsigned int col);
		vec(*multiplicationVec)(mat m, vec v);

		/*VOID*/
		void(*print)(mat m);
		void(*scalarDivisionTo)(mat* m, float k);
		void(*free)(mat* m);
		void(*ref)(mat* m);
		void(*rref)(mat* m);
		void(*scalarMultiplicationBy)(mat* m, float k);

		/*BOOL*/
		bool(*addTo)(mat* m1, mat m2);
		bool(*swapRows)(mat* m, unsigned int r1, unsigned int r2);
		bool(*addRows)(mat* m, unsigned int r1, unsigned int r2);
		bool(*multiplyRow)(mat* m, unsigned int r1, float k);
		bool(*addMultipleRow)(mat* m, unsigned int r1, unsigned int r2, float k);
		bool(*invertible)(mat* m);

		/*FLOAT*/
		float(*determinant)(mat* m);
		float(*determinantExclusion)(mat* m);
		float(*cofactor)(mat* m, unsigned int r, unsigned int c);
	}gontiMatrices;
	extern KAPI gontiMatrices gonti_matrix;

	/*DEFINES*/

	//#define matrix(rows, cols, ...) gontiMatNewMatrix(rows, cols, __VA_ARGS__)
	#define matrix(rows, cols, ...) gontiMatNewMatrix(rows, cols, NUMARGS(float, __VA_ARGS__), ##__VA_ARGS__)

	#define MAT_ROW(...) __VA_ARGS__
	#define MAT_NEW(rows, cols, ...) gontiMatNewMatrix(rows, cols, __VA_ARGS__)
	#define MAT_NEW_AR(rows, cols, ptr) gontiMatNewMatrixFromArray(rows, cols, ptr)

	#define MAT_ROW_SF(...) (double[]){__VA_ARGS__}
	#define MAT_NEW_SF(rows, cols, ...) gontiMatNewMatrixSafe(rows, cols, (double*[]){__VA_ARGS__}, rows)

/*=============================================={[ !CODE ]}==============================================*/

#ifdef __cplusplus
}
#endif

#endif // !MATRICES_H