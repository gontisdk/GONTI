#include "Matrices.h"

#ifdef Matrices_ON

#include "../../Core/MemorySubsystem/MemorySubsystem.h"

const mat MAT_UNDEFINED = { 0, 0, NULL };

/*
* MAT's
*/
mat mat_allocate(unsigned int rows, unsigned int cols) {
	mat ret;

	ret.rows = rows;
	ret.cols = cols;

	ret.elements = k_allocate(rows * sizeof(float*), MEMORY_TAG_MATRIX);
	for (unsigned int i = 0; i < rows; i++)
	{
		ret.elements[i] = k_allocate(cols * sizeof(float), MEMORY_TAG_MATRIX);
	}

	return ret;
}
mat mat_identity(unsigned int dim) {
	mat ret = mat_allocate(dim, dim);

	for (unsigned int r = 0; r < dim; r++) {
		for (unsigned int c = 0; c < dim; c++) {
			ret.elements[r][c] = (r == c) ? 1.0f : 0.0f;
		}
	}

	return ret;
}
mat mat_zeroMatrix(unsigned int rows, unsigned int cols) {
	mat ret = mat_allocate(rows, cols);

	for (unsigned int r = 0; r < rows; r++) {
		for (unsigned int c = 0; c < cols; c++) {
			ret.elements[r][c] = 0.0f;
		}
	}

	return ret;
}
mat mat_newMatrix(unsigned int rows, unsigned int cols, unsigned int numVals, ...) {
	mat ret = mat_allocate(rows, cols);

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
mat mat_newMatrixFromArray(unsigned int rows, unsigned int cols, const double* elements) {
	mat ret = mat_allocate(rows, cols);

	for (unsigned int r = 0; r < rows; r++) {
		for (unsigned int c = 0; c < cols; c++) {
			ret.elements[r][c] = elements[r * cols + c];
		}
	}
	return ret;
}
mat mat_newMatrixSafe(unsigned int rows, unsigned int cols, double* row_data[], unsigned int row_count) {
	mat ret = mat_allocate(rows, cols);

	unsigned int actual_rows = (row_count < rows) ? row_count : rows;

	for (unsigned int r = 0; r < actual_rows; r++) {
		for (unsigned int c = 0; c < cols; c++) {
			ret.elements[r][c] = row_data[r][c];
		}
	}

	return ret;
}
mat mat_copy(mat m) {
	return m;
}
mat mat_copyPtr(mat* m) {
	mat ret;
	k_copyMemory(&ret, m, sizeof(mat));
	return ret;
}
mat mat_toRowVec(vec* v) {
	mat ret = mat_allocate(1, v->dim);

	for (unsigned int i = 0; i < v->dim; i++) {
		ret.elements[0][i] = v->elements[i];
	}

	return ret;
}
mat mat_toColVec(vec* v) {
	mat ret = mat_allocate(v->dim, 1);

	for (unsigned int i = 0; i < v->dim; i++) {
		ret.elements[i][0] = v->elements[i];
	}

	return ret;
}
mat mat_scalarDivision(mat m, float k) {
	mat ret = mat_allocate(m.rows, m.cols);

	for (unsigned int r = 0; r < ret.rows; r++) {
		for (unsigned int c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = m.elements[r][c] + k;
		}
	}

	return ret;
}
mat mat_add(mat m1, mat m2) {
	if (m1.rows != m2.rows ||
		m1.cols != m2.cols) 
		return MAT_UNDEFINED;

	mat ret = mat_allocate(m1.rows, m1.cols);

	for (unsigned int r = 0; r < ret.rows; r++) {
		for (unsigned int c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = m1.elements[r][c] + m2.elements[r][c];
		}
	}

	return ret;
}
mat mat_multiplicationMat(mat m1, mat m2) {
	if (m1.cols != m2.rows) return MAT_UNDEFINED;

	vec* m1Rows = k_allocate(m1.rows * sizeof(vec), MEMORY_TAG_MATH);
	vec* m2Cols = k_allocate(m2.cols * sizeof(vec), MEMORY_TAG_MATH);

	for (unsigned int r = 0; r < m1.rows; r++) {
		m1Rows[r] = mat_getRow(&m1, r + 1);

	}
	for (unsigned int c = 0; c < m2.cols; c++) {
		m2Cols[c] = mat_getCol(&m2, c + 1);
	}

	mat ret = mat_allocate(m1.rows, m2.cols);

	for (unsigned int r = 0; r < ret.rows; r++) {
		for (unsigned int c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = vec_dot(m1Rows[r], m2Cols[c]);
		}
	}

	for (unsigned int r = 0; r < m1.rows; r++) {
		vec_free(m1Rows + r);
	}
	for (unsigned int c = 0; c < m2.cols; c++) {
		vec_free(m2Cols + c);
	}

	k_free(m1Rows, m1.rows * sizeof(vec), MEMORY_TAG_MATH);
	k_free(m2Cols, m2.cols * sizeof(vec), MEMORY_TAG_MATH);

	return ret;
}
mat mat_scalarMultiplication(mat m, float k) {
    mat ret = mat_allocate(m.rows, m.cols);

    for (unsigned int r = 0; r < ret.rows; r++) {
        for (unsigned int c = 0; c < ret.cols; c++) {
            ret.elements[r][c] = m.elements[r][c] * k;
        }
    }

    return ret;
}
mat mat_transpose(mat* m) {
	mat ret = mat_allocate(m->cols, m->rows);

	for (unsigned int r = 0; r < ret.rows; r++) {
		for (unsigned int c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = m->elements[c][r];
		}
	}

	return ret;
}
mat mat_argumentVector(mat* m, vec* v) {
	if (m->rows != v->dim) return MAT_UNDEFINED;

	mat ret = mat_allocate(m->rows, m->cols + 1);

	for (unsigned int r = 0; r < m->rows; r++) {
		unsigned int c = 0;

		for (; c < m->cols; c++) {
			ret.elements[r][c] = m->elements[r][c];
		}

		ret.elements[r][c] = v->elements[r];
	}

	return ret;
}
mat mat_argumentMatrix(mat* m1, mat* m2) {
	if (m1->rows != m2->rows) return MAT_UNDEFINED;

	mat ret = mat_allocate(m1->rows, m1->cols + m2->cols);

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
mat mat_splice(mat* m, unsigned int exclRow, unsigned int exclCol) {
	exclRow--; exclCol--;

	mat ret = mat_allocate(m->rows - 1, m->cols - 1);

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
mat mat_cofactorMatrix(mat* m) {
	if (m->rows != m->cols || m->rows == 0) return MAT_UNDEFINED;

	mat ret = mat_allocate(m->rows, m->cols);

	for (unsigned int r = 0; r < ret.rows; r++) {
		for (unsigned int c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = mat_cofactor(m, r + 1, c + 1);
		}
	}

	return ret;
}
mat mat_adjugate(mat* m) {
	if (m->rows != m->cols || m->rows == 0) return MAT_UNDEFINED;

	mat ret = mat_allocate(m->rows, m->cols);

	for (unsigned int r = 0; r < ret.rows; r++) {
		for (unsigned int c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = mat_cofactor(m, c + 1, r + 1);
		}
	}

	return ret;
}
mat mat_inverseMat(mat* m) {
	if (m->rows != m->cols || m->rows == 0) return MAT_UNDEFINED;

	mat adj = mat_adjugate(m);
	float det = 0.0f;

	for (unsigned int c = 0; c < m->cols; c++) {
		if (m->elements[0][c] != 0.0f) {
			det += m->elements[0][c] * adj.elements[c][0];
		}
	}

	if (det == 0.0f) return MAT_UNDEFINED;

	mat ret = mat_scalarMultiplication(adj, 1.0f / det);
	mat_free(&adj);

	return ret;
}

/*
* VEC's
*/
vec mat_getRow(mat* m, unsigned int row) {
	row--;
	if (row >= m->rows) return VEC_UNDEFINED;

	vec ret = vec_allocate(m->cols);

	for (unsigned int i = 0; i < ret.dim; i++) {
		ret.elements[i] = m->elements[row][i];
	}

	return ret;
}
vec mat_getCol(mat* m, unsigned int col) {
	col--;
	if (col >= m->cols) return VEC_UNDEFINED;

	vec ret = vec_allocate(m->rows);

	for (unsigned int i = 0; i < ret.dim; i++) {
		ret.elements[i] = m->elements[i][col];
	}

	return ret;
}
vec mat_multiplicationVec(mat m, vec v) {
	if (m.cols != v.dim) return VEC_UNDEFINED;

	vec ret = vec_allocate(m.rows);

	for (unsigned int r = 0; r < ret.dim; r++) {
		ret.elements[r] = vec_dot(v, mat_getRow(&m, r + 1));
	}

	return ret;
}

/*
* VOID's
*/
void mat_print(mat m) {
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
void mat_scalarDivisionTo(mat* m, float k) {
	for (unsigned int r = 0; r < m->rows; r++) {
		for (unsigned int c = 0; c < m->cols; c++) {
			m->elements[r][c] += k;
		}
	}
}
void mat_scalarMultiplicationBy(mat* m, float k) {
    for (unsigned int r = 0; r < m->rows; r++) {
        for (unsigned int c = 0; c < m->cols; c++) {
            m->elements[r][c] *= k;
        }
    }
}
void mat_free(mat* m) {
	for (unsigned int r = 0; r < m->rows; r++) {
		k_free(m->elements[r], m->cols * sizeof(float), MEMORY_TAG_MATRIX);
	}

	k_free(m->elements, m->rows * sizeof(float*), MEMORY_TAG_MATRIX);

	m->elements = NULL;
	m->rows = 0;
	m->cols = 0;
}
void mat_ref(mat* m) {
	unsigned int currentRow = 0;

	for (unsigned int c = 0; c < m->cols; c++) {
		unsigned int r = currentRow;

		if (r >= m->rows) break;

		for (; r < m->rows; r++) {
			if (m->elements[r][c] != 0.0f) break;
		}

		if (r == m->rows) continue;

		mat_swapRows(m, currentRow, r);

		float factor = 1 / m->elements[currentRow][c];

		for (unsigned int col = c; col < m->cols; col++) {
			m->elements[currentRow][col] *= factor;
		}

		for (r = currentRow + 1; r < m->rows; r++) {
			mat_addMultipleRow(m, r, currentRow, -1 * m->elements[r][c]);
		}

		currentRow++;
	}
}
void mat_rref(mat* m) {
	unsigned int currentRow = 0;

	for (unsigned int c = 0; c < m->cols; c++) {
		unsigned int r = currentRow;

		if (r >= m->rows) break;

		for (; r < m->rows; r++) {
			if (m->elements[r][c] != 0.0f) break;
		}

		if (r == m->rows) continue;

		mat_swapRows(m, currentRow, r);

		float factor = 1 / m->elements[currentRow][c];

		for (unsigned int col = c; col < m->cols; col++) {
			m->elements[currentRow][col] *= factor;
		}

		for (r = 0; r < m->rows; r++) {
			if (r == currentRow) continue;

			mat_addMultipleRow(m, r, currentRow, -1 * m->elements[r][c]);
		}

		currentRow++;
	}
}

/*
* BOOL's
*/
bool mat_addTo(mat* m1, mat m2) {
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
bool mat_swapRows(mat* m, unsigned int r1, unsigned int r2) {
	if (r1 >= m->rows || r2 >= m->rows || r1 == r2) return false;

	float* temp = m->elements[r1];
	m->elements[r1] = m->elements[r2];
	m->elements[r2] = temp;

	return true;
}
bool mat_addRows(mat* m, unsigned int r1, unsigned int r2) {
	if (r1 >= m->rows || r2 >= m->rows || r1 == r2) return false;

	for (unsigned int c = 0; c < m->cols; c++) {
		m->elements[r1][c] += m->elements[r2][c];
	}

	return true;
}
bool mat_multiplyRow(mat* m, unsigned int r1, float k) {
	if (r1 >= m->rows || k == 0.0f) return false;

	for (unsigned int c = 0; c < m->cols; c++) {
		m->elements[r1][c] *= k;
	}

	return true;
}
bool mat_addMultipleRow(mat* m, unsigned int r1, unsigned int r2, float k) {
	if (r1 >= m->rows || r2 >= m->rows || r1 == r2 || k == 0.0f) return false;

	for (unsigned int c = 0; c < m->cols; c++) {
		m->elements[r1][c] += m->elements[r2][c] * k;
	}

	return true;
}
bool mat_invertible(mat* m) {
	if (m->rows != m->cols || m->rows == 0) return false;
	return mat_determinant(m) != 0.0f;
}

/*
* FLOAT
*/
float mat_determinant(mat* m) {
	if (m->rows != m->cols || m->rows == 0) return 0.0f;
	if (m->rows == 1) return m->elements[0][0];
	
	// char cofactorSign = 1;
	// float ret = 0.0f;
	
	// for (unsigned int c = 0; c < m->cols; c++) {
	// 	mat minor = mat_splice(m, 1, c + 1);
	// 	ret += cofactorSign * m->elements[0][c] * mat_determinant(&minor);
	// 	cofactorSign = -cofactorSign;
	// }

	float ret = 0.0f;

	for (unsigned int c = 0; c < m->cols; c++) {
		if (m->elements[0][c] != 0.0f) {
			ret += m->elements[0][c] * mat_cofactor(m, 1, c + 1);
		}
	}
	
	return ret;
}
float mat_determinantExclusion(mat* m) {
	if (m->rows != m->cols || m->rows == 0) return 0.0f;

    unsigned int *skipCols = k_allocate(m->cols * sizeof(unsigned int), MEMORY_TAG_MATH);
    unsigned int noSkipCols = 0;

    float ret = _mat_determinantExclusion(m, 1, 0, skipCols, &noSkipCols);

    k_free(skipCols, m->cols * sizeof(unsigned int), MEMORY_TAG_MATH);

	return ret;
}
float _mat_determinantExclusion(mat* m, unsigned int row, unsigned int col, unsigned int* skipCols, unsigned int* noSkipCols) {
	skipCols[*noSkipCols] = col;
	(*noSkipCols)++;

	if (row == m->rows) {
		unsigned int c = m->cols;

		if (*noSkipCols != 0) {
			if (skipCols[*noSkipCols - 1] != 0) {
				while (m_math.containsUint(skipCols, *noSkipCols, c)) {
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
		if (m_math.containsUint(skipCols, *noSkipCols, c)) continue;

		float res = 0.0f;
		if (m->elements[row - 1][c - 1] != 0.0f) 
			res = cofactorSign * m->elements[row - 1][c - 1] * _mat_determinantExclusion(m, row + 1, c, skipCols, noSkipCols);

		ret += res;
		cofactorSign = -cofactorSign;
	}

	(*noSkipCols)--;

	return ret;
}
float mat_cofactor(mat* m, unsigned int r, unsigned int c) {
	if (m->rows != m->cols || m->rows == 0 
	|| r > m->rows || c > m->cols) return 0.0f;

	char sign = ((r + c) & 1) == 0 ? 1 : -1;
	mat spliceMat = mat_splice(m, r, c);

	float ret = sign * mat_determinant(&spliceMat);
	mat_free(&spliceMat);

	return ret;
}

/*
* m_matrix
*/
Matrices m_matrix = {
	/*MAT*/
	.allocate = mat_allocate,
	.identity = mat_identity,
	.zeroMatrix = mat_zeroMatrix,
	.newMatrix = mat_newMatrix,
	.newMatrixFromArray = mat_newMatrixFromArray,
	.newMatrixSafe = mat_newMatrixSafe,
	.copy = mat_copy,
	.copyPtr = mat_copyPtr,
	.toRowVec = mat_toRowVec,
	.toColVec = mat_toColVec,
	.scalarDivision = mat_scalarDivision,
	.add = mat_add,
	.multiplicationMat = mat_multiplicationMat,
	.transpose = mat_transpose,
	.argumentVector = mat_argumentVector,
	.argumentMatrix = mat_argumentMatrix,
	.splice = mat_splice,
	.cofactorMatrix = mat_cofactorMatrix,
	.adjugate = mat_adjugate,
	.inverseMat = mat_inverseMat,
	.scalarMultiplication = mat_scalarMultiplication,

	/*VEC*/
	.getRow = mat_getRow,
	.getCol = mat_getCol,
	.multiplicationVec = mat_multiplicationVec,

	/*VOID*/
	.print = mat_print,
	.scalarDivisionTo = mat_scalarDivisionTo,
	.free = mat_free,
	.ref = mat_ref,
	.rref = mat_rref,
	.scalarMultiplicationBy = mat_scalarMultiplicationBy,

	/*BOOL*/
	.addTo = mat_addTo,
	.swapRows = mat_swapRows,
	.addRows = mat_addRows,
	.multiplyRow = mat_multiplyRow,
	.addMultipleRow = mat_addMultipleRow,
	.invertible = mat_invertible,

	/*FLOAT*/
	.determinant = mat_determinant,
	.determinantExclusion = mat_determinantExclusion,
	.cofactor = mat_cofactor
};

#endif