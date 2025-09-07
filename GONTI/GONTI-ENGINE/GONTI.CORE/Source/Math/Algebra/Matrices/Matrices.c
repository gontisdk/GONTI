#include "Matrices.h"

#include "../../../Memory/Memory.h"

const mat MAT_UNDEFINED = { 0, 0, NULL };

/*
* MAT's
*/
mat gontiMatAllocate(unsigned int rows, unsigned int cols) {
	mat ret;

	ret.rows = rows;
	ret.cols = cols;

	ret.elements = k_allocate(rows * sizeof(float*), GONTI_MEMORY_TAG_MATRIX);
	for (unsigned int i = 0; i < rows; i++)
	{
		ret.elements[i] = k_allocate(cols * sizeof(float), GONTI_MEMORY_TAG_MATRIX);
	}

	return ret;
}
mat gontiMatIdentity(unsigned int dim) {
	mat ret = gontiMatAllocate(dim, dim);

	for (unsigned int r = 0; r < dim; r++) {
		for (unsigned int c = 0; c < dim; c++) {
			ret.elements[r][c] = (r == c) ? 1.0f : 0.0f;
		}
	}

	return ret;
}
mat gontiMatZeroMatrix(unsigned int rows, unsigned int cols) {
	mat ret = gontiMatAllocate(rows, cols);

	for (unsigned int r = 0; r < rows; r++) {
		for (unsigned int c = 0; c < cols; c++) {
			ret.elements[r][c] = 0.0f;
		}
	}

	return ret;
}
mat gontiMatNewMatrix(unsigned int rows, unsigned int cols, unsigned int numVals, ...) {
	mat ret = gontiMatAllocate(rows, cols);

	va_list list;
    va_start(list, numVals);

    unsigned int i = 0;

    for (unsigned int r = 0; r < rows; r++) {
        for (unsigned int c = 0; c < cols; c++) {
            if (i++ < numVals) {
                ret.elements[r][c] = va_arg(list, double);
            }
            else {
                ret.elements[r][c] = 0.0f;
            }
        }
    }

    va_end(list);
	return ret;
}
mat gontiMatNewMatrixFromArray(unsigned int rows, unsigned int cols, const double* elements) {
	mat ret = gontiMatAllocate(rows, cols);

	for (unsigned int r = 0; r < rows; r++) {
		for (unsigned int c = 0; c < cols; c++) {
			ret.elements[r][c] = elements[r * cols + c];
		}
	}
	return ret;
}
mat gontiMatNewMatrixSafe(unsigned int rows, unsigned int cols, double* row_data[], unsigned int row_count) {
	mat ret = gontiMatAllocate(rows, cols);

	unsigned int actual_rows = (row_count < rows) ? row_count : rows;

	for (unsigned int r = 0; r < actual_rows; r++) {
		for (unsigned int c = 0; c < cols; c++) {
			ret.elements[r][c] = row_data[r][c];
		}
	}

	return ret;
}
mat gontiMatCopy(mat m) {
	return m;
}
mat gontiMatCopyPtr(mat* m) {
	mat ret;
	k_copyMemory(&ret, m, sizeof(mat));
	return ret;
}
mat gontiMatToRowVec(vec* v) {
	mat ret = gontiMatAllocate(1, v->dim);

	for (unsigned int i = 0; i < v->dim; i++) {
		ret.elements[0][i] = v->elements[i];
	}

	return ret;
}
mat gontiMatToColVec(vec* v) {
	mat ret = gontiMatAllocate(v->dim, 1);

	for (unsigned int i = 0; i < v->dim; i++) {
		ret.elements[i][0] = v->elements[i];
	}

	return ret;
}
mat gontiMatScalarDivision(mat m, float k) {
	mat ret = gontiMatAllocate(m.rows, m.cols);

	for (unsigned int r = 0; r < ret.rows; r++) {
		for (unsigned int c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = m.elements[r][c] + k;
		}
	}

	return ret;
}
mat gontiMatAdd(mat m1, mat m2) {
	if (m1.rows != m2.rows ||
		m1.cols != m2.cols) 
		return MAT_UNDEFINED;

	mat ret = gontiMatAllocate(m1.rows, m1.cols);

	for (unsigned int r = 0; r < ret.rows; r++) {
		for (unsigned int c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = m1.elements[r][c] + m2.elements[r][c];
		}
	}

	return ret;
}
mat gontiMatMultiplicationMat(mat m1, mat m2) {
	if (m1.cols != m2.rows) return MAT_UNDEFINED;

	vec* m1Rows = k_allocate(m1.rows * sizeof(vec), GONTI_MEMORY_TAG_MATH);
	vec* m2Cols = k_allocate(m2.cols * sizeof(vec), GONTI_MEMORY_TAG_MATH);

	for (unsigned int r = 0; r < m1.rows; r++) {
		m1Rows[r] = gontiMatGetRow(&m1, r + 1);

	}
	for (unsigned int c = 0; c < m2.cols; c++) {
		m2Cols[c] = gontiMatGetCol(&m2, c + 1);
	}

	mat ret = gontiMatAllocate(m1.rows, m2.cols);

	for (unsigned int r = 0; r < ret.rows; r++) {
		for (unsigned int c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = gontiVecDot(m1Rows[r], m2Cols[c]);
		}
	}

	for (unsigned int r = 0; r < m1.rows; r++) {
		gontiVecFree(m1Rows + r);
	}
	for (unsigned int c = 0; c < m2.cols; c++) {
		gontiVecFree(m2Cols + c);
	}

	k_free(m1Rows, m1.rows * sizeof(vec), GONTI_MEMORY_TAG_MATH);
	k_free(m2Cols, m2.cols * sizeof(vec), GONTI_MEMORY_TAG_MATH);

	return ret;
}
mat gontiMatScalarMultiplication(mat m, float k) {
    mat ret = gontiMatAllocate(m.rows, m.cols);

    for (unsigned int r = 0; r < ret.rows; r++) {
        for (unsigned int c = 0; c < ret.cols; c++) {
            ret.elements[r][c] = m.elements[r][c] * k;
        }
    }

    return ret;
}
mat gontiMatTranspose(mat* m) {
	mat ret = gontiMatAllocate(m->cols, m->rows);

	for (unsigned int r = 0; r < ret.rows; r++) {
		for (unsigned int c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = m->elements[c][r];
		}
	}

	return ret;
}
mat gontiMatArgumentVector(mat* m, vec* v) {
	if (m->rows != v->dim) return MAT_UNDEFINED;

	mat ret = gontiMatAllocate(m->rows, m->cols + 1);

	for (unsigned int r = 0; r < m->rows; r++) {
		unsigned int c = 0;

		for (; c < m->cols; c++) {
			ret.elements[r][c] = m->elements[r][c];
		}

		ret.elements[r][c] = v->elements[r];
	}

	return ret;
}
mat gontiMatArgumentMatrix(mat* m1, mat* m2) {
	if (m1->rows != m2->rows) return MAT_UNDEFINED;

	mat ret = gontiMatAllocate(m1->rows, m1->cols + m2->cols);

	for (unsigned int r = 0; r < m1->rows; r++) {
		unsigned int c = 0;

		for (; c < m1->cols; c++) {
			ret.elements[r][c] = m1->elements[r][c];
		}

		for (; c < ret.cols; c++) {
			ret.elements[r][c] = m2->elements[r][c - m1->cols];
		}
	}

	return ret;
}
mat gontiMatSplice(mat* m, unsigned int exclRow, unsigned int exclCol) {
	exclRow--; exclCol--;

	mat ret = gontiMatAllocate(m->rows - 1, m->cols - 1);

	unsigned int offsetRow = 0;
	for (unsigned int r = 0; r < ret.rows; r++) {
		unsigned int offsetCol = 0;

		if (r == exclRow) offsetRow++;

		for (unsigned int c = 0; c < ret.cols; c++) {
			if (c == exclCol) offsetCol++;

			ret.elements[r][c] = m->elements[r + offsetRow][c + offsetCol];
		}
	}

	return ret;
}
mat gontiMatCofactorMatrix(mat* m) {
	if (m->rows != m->cols || m->rows == 0) return MAT_UNDEFINED;

	mat ret = gontiMatAllocate(m->rows, m->cols);

	for (unsigned int r = 0; r < ret.rows; r++) {
		for (unsigned int c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = gontiMatCofactor(m, r + 1, c + 1);
		}
	}

	return ret;
}
mat gontiMatAdjugate(mat* m) {
	if (m->rows != m->cols || m->rows == 0) return MAT_UNDEFINED;

	mat ret = gontiMatAllocate(m->rows, m->cols);

	for (unsigned int r = 0; r < ret.rows; r++) {
		for (unsigned int c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = gontiMatCofactor(m, c + 1, r + 1);
		}
	}

	return ret;
}
mat gontiMatInverseMat(mat* m) {
	if (m->rows != m->cols || m->rows == 0) return MAT_UNDEFINED;

	mat adj = gontiMatAdjugate(m);
	float det = 0.0f;

	for (unsigned int c = 0; c < m->cols; c++) {
		if (m->elements[0][c] != 0.0f) {
			det += m->elements[0][c] * adj.elements[c][0];
		}
	}

	if (det == 0.0f) return MAT_UNDEFINED;

	mat ret = gontiMatScalarMultiplication(adj, 1.0f / det);
	gontiMatFree(&adj);

	return ret;
}

/*
* VEC's
*/
vec gontiMatGetRow(mat* m, unsigned int row) {
	row--;
	if (row >= m->rows) return VEC_UNDEFINED;

	vec ret = gontiVecAllocate(m->cols);

	for (unsigned int i = 0; i < ret.dim; i++) {
		ret.elements[i] = m->elements[row][i];
	}

	return ret;
}
vec gontiMatGetCol(mat* m, unsigned int col) {
	col--;
	if (col >= m->cols) return VEC_UNDEFINED;

	vec ret = gontiVecAllocate(m->rows);

	for (unsigned int i = 0; i < ret.dim; i++) {
		ret.elements[i] = m->elements[i][col];
	}

	return ret;
}
vec gontiMatMultiplicationVec(mat m, vec v) {
	if (m.cols != v.dim) return VEC_UNDEFINED;

	vec ret = gontiVecAllocate(m.rows);

	for (unsigned int r = 0; r < ret.dim; r++) {
		ret.elements[r] = gontiVecDot(v, gontiMatGetRow(&m, r + 1));
	}

	return ret;
}

/*
* VOID's
*/
void gontiMatPrint(mat m) {
	printf("\n");

	for (unsigned int r = 0; r < m.rows; r++) {
		printf("\n");
		printf("|");

		for (unsigned int c = 0; c < m.cols; c++) {
			printf(" %f", m.elements[r][c]);
		}

		printf(" |");
	}
	printf("\n");
}
void gontiMatScalarDivisionTo(mat* m, float k) {
	for (unsigned int r = 0; r < m->rows; r++) {
		for (unsigned int c = 0; c < m->cols; c++) {
			m->elements[r][c] += k;
		}
	}
}
void gontiMatScalarMultiplicationBy(mat* m, float k) {
    for (unsigned int r = 0; r < m->rows; r++) {
        for (unsigned int c = 0; c < m->cols; c++) {
            m->elements[r][c] *= k;
        }
    }
}
void gontiMatFree(mat* m) {
	for (unsigned int r = 0; r < m->rows; r++) {
		k_free(m->elements[r], m->cols * sizeof(float), GONTI_MEMORY_TAG_MATRIX);
	}

	k_free(m->elements, m->rows * sizeof(float*), GONTI_MEMORY_TAG_MATRIX);

	m->elements = NULL;
	m->rows = 0;
	m->cols = 0;
}
void gontiMatRef(mat* m) {
	unsigned int currentRow = 0;

	for (unsigned int c = 0; c < m->cols; c++) {
		unsigned int r = currentRow;

		if (r >= m->rows) break;

		for (; r < m->rows; r++) {
			if (m->elements[r][c] != 0.0f) break;
		}

		if (r == m->rows) continue;

		gontiMatSwapRows(m, currentRow, r);

		float factor = 1 / m->elements[currentRow][c];

		for (unsigned int col = c; col < m->cols; col++) {
			m->elements[currentRow][col] *= factor;
		}

		for (r = currentRow + 1; r < m->rows; r++) {
			gontiMatAddMultipleRow(m, r, currentRow, -1 * m->elements[r][c]);
		}

		currentRow++;
	}
}
void gontiMatRref(mat* m) {
	unsigned int currentRow = 0;

	for (unsigned int c = 0; c < m->cols; c++) {
		unsigned int r = currentRow;

		if (r >= m->rows) break;

		for (; r < m->rows; r++) {
			if (m->elements[r][c] != 0.0f) break;
		}

		if (r == m->rows) continue;

		gontiMatSwapRows(m, currentRow, r);

		float factor = 1 / m->elements[currentRow][c];

		for (unsigned int col = c; col < m->cols; col++) {
			m->elements[currentRow][col] *= factor;
		}

		for (r = 0; r < m->rows; r++) {
			if (r == currentRow) continue;

			gontiMatAddMultipleRow(m, r, currentRow, -1 * m->elements[r][c]);
		}

		currentRow++;
	}
}

/*
* BOOL's
*/
bool gontiMatAddTo(mat* m1, mat m2) {
	if (m1->rows != m2.rows ||
		m1->cols != m2.cols) {
		return false;
	}

	for (unsigned int r = 0; r < m1->rows; r++) {
		for (unsigned int c = 0; c < m1->cols; c++) {
			m1->elements[r][c] += m2.elements[r][c];
		}
	}

	return true;
}
bool gontiMatSwapRows(mat* m, unsigned int r1, unsigned int r2) {
	if (r1 >= m->rows || r2 >= m->rows || r1 == r2) return false;

	float* temp = m->elements[r1];
	m->elements[r1] = m->elements[r2];
	m->elements[r2] = temp;

	return true;
}
bool gontiMatAddRows(mat* m, unsigned int r1, unsigned int r2) {
	if (r1 >= m->rows || r2 >= m->rows || r1 == r2) return false;

	for (unsigned int c = 0; c < m->cols; c++) {
		m->elements[r1][c] += m->elements[r2][c];
	}

	return true;
}
bool gontiMatMultiplyRow(mat* m, unsigned int r1, float k) {
	if (r1 >= m->rows || k == 0.0f) return false;

	for (unsigned int c = 0; c < m->cols; c++) {
		m->elements[r1][c] *= k;
	}

	return true;
}
bool gontiMatAddMultipleRow(mat* m, unsigned int r1, unsigned int r2, float k) {
	if (r1 >= m->rows || r2 >= m->rows || r1 == r2 || k == 0.0f) return false;

	for (unsigned int c = 0; c < m->cols; c++) {
		m->elements[r1][c] += m->elements[r2][c] * k;
	}

	return true;
}
bool gontiMatInvertible(mat* m) {
	if (m->rows != m->cols || m->rows == 0) return false;
	return gontiMatDeterminant(m) != 0.0f;
}

/*
* FLOAT
*/
float gontiMatDeterminant(mat* m) {
	if (m->rows != m->cols || m->rows == 0) return 0.0f;
	if (m->rows == 1) return m->elements[0][0];
	
	// char cofactorSign = 1;
	// float ret = 0.0f;
	
	// for (unsigned int c = 0; c < m->cols; c++) {
	// 	mat minor = gontiMatSplice(m, 1, c + 1);
	// 	ret += cofactorSign * m->elements[0][c] * gontiMatDeterminant(&minor);
	// 	cofactorSign = -cofactorSign;
	// }

	float ret = 0.0f;

	for (unsigned int c = 0; c < m->cols; c++) {
		if (m->elements[0][c] != 0.0f) {
			ret += m->elements[0][c] * gontiMatCofactor(m, 1, c + 1);
		}
	}
	
	return ret;
}
float gontiMatDeterminantExclusion(mat* m) {
	if (m->rows != m->cols || m->rows == 0) return 0.0f;

    unsigned int *skipCols = k_allocate(m->cols * sizeof(unsigned int), GONTI_MEMORY_TAG_MATH);
    unsigned int noSkipCols = 0;

    float ret = _gontiMatDeterminantExclusion(m, 1, 0, skipCols, &noSkipCols);

    k_free(skipCols, m->cols * sizeof(unsigned int), GONTI_MEMORY_TAG_MATH);

	return ret;
}
float _gontiMatDeterminantExclusion(mat* m, unsigned int row, unsigned int col, unsigned int* skipCols, unsigned int* noSkipCols) {
	skipCols[*noSkipCols] = col;
	(*noSkipCols)++;

	if (row == m->rows) {
		unsigned int c = m->cols;

		if (*noSkipCols != 0) {
			if (skipCols[*noSkipCols - 1] != 0) {
				while (gonti_math.containsUint(skipCols, *noSkipCols, c)) {
					c--;
				}
				(*noSkipCols)--;
			}
		}

		return m->elements[row - 1][c - 1];
	}

	float ret = 0.0f;
	char cofactorSign = 1;

	for (unsigned int c = 1; c <= m->cols; c++) {
		if (gonti_math.containsUint(skipCols, *noSkipCols, c)) continue;

		float res = 0.0f;
		if (m->elements[row - 1][c - 1] != 0.0f) 
			res = cofactorSign * m->elements[row - 1][c - 1] * _gontiMatDeterminantExclusion(m, row + 1, c, skipCols, noSkipCols);

		ret += res;
		cofactorSign = -cofactorSign;
	}

	(*noSkipCols)--;

	return ret;
}
float gontiMatCofactor(mat* m, unsigned int r, unsigned int c) {
	if (m->rows != m->cols || m->rows == 0 
	|| r > m->rows || c > m->cols) return 0.0f;

	char sign = ((r + c) & 1) == 0 ? 1 : -1;
	mat spliceMat = gontiMatSplice(m, r, c);

	float ret = sign * gontiMatDeterminant(&spliceMat);
	gontiMatFree(&spliceMat);

	return ret;
}

/*
* gonti_matrix
*/
gontiMatrices gonti_matrix = {
	/*MAT*/
	.allocate = gontiMatAllocate,
	.identity = gontiMatIdentity,
	.zeroMatrix = gontiMatZeroMatrix,
	.newMatrix = gontiMatNewMatrix,
	.newMatrixFromArray = gontiMatNewMatrixFromArray,
	.newMatrixSafe = gontiMatNewMatrixSafe,
	.copy = gontiMatCopy,
	.copyPtr = gontiMatCopyPtr,
	.toRowVec = gontiMatToRowVec,
	.toColVec = gontiMatToColVec,
	.scalarDivision = gontiMatScalarDivision,
	.add = gontiMatAdd,
	.multiplicationMat = gontiMatMultiplicationMat,
	.transpose = gontiMatTranspose,
	.argumentVector = gontiMatArgumentVector,
	.argumentMatrix = gontiMatArgumentMatrix,
	.splice = gontiMatSplice,
	.cofactorMatrix = gontiMatCofactorMatrix,
	.adjugate = gontiMatAdjugate,
	.inverseMat = gontiMatInverseMat,
	.scalarMultiplication = gontiMatScalarMultiplication,

	/*VEC*/
	.getRow = gontiMatGetRow,
	.getCol = gontiMatGetCol,
	.multiplicationVec = gontiMatMultiplicationVec,

	/*VOID*/
	.print = gontiMatPrint,
	.scalarDivisionTo = gontiMatScalarDivisionTo,
	.free = gontiMatFree,
	.ref = gontiMatRef,
	.rref = gontiMatRref,
	.scalarMultiplicationBy = gontiMatScalarMultiplicationBy,

	/*BOOL*/
	.addTo = gontiMatAddTo,
	.swapRows = gontiMatSwapRows,
	.addRows = gontiMatAddRows,
	.multiplyRow = gontiMatMultiplyRow,
	.addMultipleRow = gontiMatAddMultipleRow,
	.invertible = gontiMatInvertible,

	/*FLOAT*/
	.determinant = gontiMatDeterminant,
	.determinantExclusion = gontiMatDeterminantExclusion,
	.cofactor = gontiMatCofactor
};
