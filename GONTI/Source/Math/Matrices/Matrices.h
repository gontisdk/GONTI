#ifndef MATRICES_H
#define MATRICES_H

#ifdef __cplusplus
extern "C" {
#endif

	/*=============================================={[ CODE ]}==============================================*/

	#include "../Maths.h"

	#ifdef Matrices_ON

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
		KAPI mat mat_allocate(unsigned int rows, unsigned int cols);
		KAPI mat mat_identity(unsigned int dim);
		KAPI mat mat_zeroMatrix(unsigned int rows, unsigned int cols);
		KAPI mat mat_newMatrix(unsigned int rows, unsigned int cols, unsigned int numVals, ...);
		KAPI mat mat_newMatrixFromArray(unsigned int rows, unsigned int cols, const double* elements);
		KAPI mat mat_newMatrixSafe(unsigned int rows, unsigned int cols, double* row_data[], unsigned int row_count);
		KAPI mat mat_copy(mat m);
		KAPI mat mat_copyPtr(mat* m);
		KAPI mat mat_toRowVec(vec* v);
		KAPI mat mat_toColVec(vec* v);
		KAPI mat mat_scalarDivision(mat m, float k);
		KAPI mat mat_add(mat m1, mat m2);
		KAPI mat mat_multiplicationMat(mat m1, mat m2);
		KAPI mat mat_transpose(mat* m);
		KAPI mat mat_argumentVector(mat* m, vec* v);
		KAPI mat mat_argumentMatrix(mat* m1, mat* m2);
		KAPI mat mat_splice(mat* m, unsigned int exclRow, unsigned int exclCol);
		KAPI mat mat_cofactorMatrix(mat* m);
		KAPI mat mat_adjugate(mat* m);
		KAPI mat mat_inverseMat(mat* m);
		KAPI mat mat_scalarMultiplication(mat m, float k);

		/*VEC*/
		KAPI vec mat_getRow(mat* m, unsigned int row);
		KAPI vec mat_getCol(mat* m, unsigned int col);
		KAPI vec mat_multiplicationVec(mat m, vec v);

		/*VOID*/
		KAPI void mat_print(mat m);
		KAPI void mat_scalarDivisionTo(mat* m, float k);
		KAPI void mat_scalarMultiplicationBy(mat* m, float k);
		KAPI void mat_free(mat* m);
		KAPI void mat_ref(mat* m);
		KAPI void mat_rref(mat* m);

		/*BOOL*/
		KAPI bool mat_addTo(mat* m1, mat m2);
		KAPI bool mat_swapRows(mat* m, unsigned int r1, unsigned int r2);
		KAPI bool mat_addRows(mat* m, unsigned int r1, unsigned int r2);
		KAPI bool mat_multiplyRow(mat* m, unsigned int r1, float k);
		KAPI bool mat_addMultipleRow(mat* m, unsigned int r1, unsigned int r2, float k);
		KAPI bool mat_invertible(mat* m);

		/*FLOAT*/
		KAPI float mat_determinant(mat* m);
		KAPI float mat_determinantExclusion(mat* m);
		KAPI float _mat_determinantExclusion(mat* m, unsigned int row, unsigned int col, unsigned int* skipCols, unsigned int* noSkipCols);
		KAPI float mat_cofactor(mat* m, unsigned int r, unsigned int c);

		/*m_matrix*/

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
		}Matrices;
		extern KAPI Matrices m_matrix;

		/*DEFINES*/

		//#define matrix(rows, cols, ...) mat_newMatrix(rows, cols, __VA_ARGS__)
		#define matrix(rows, cols, ...) mat_newMatrix(rows, cols, NUMARGS(float, __VA_ARGS__), ##__VA_ARGS__)

		#define MAT_ROW(...) __VA_ARGS__
		#define MAT_NEW(rows, cols, ...) mat_newMatrix(rows, cols, __VA_ARGS__)
		#define MAT_NEW_AR(rows, cols, ptr) mat_newMatrixFromArray(rows, cols, ptr)

		#define MAT_ROW_SF(...) (double[]){__VA_ARGS__}
		#define MAT_NEW_SF(rows, cols, ...) mat_newMatrixSafe(rows, cols, (double*[]){__VA_ARGS__}, rows)

	#endif

/*=============================================={[ !CODE ]}==============================================*/

#ifdef __cplusplus
}
#endif

#endif // !MATRICES_H