#include <Algebra/Matrices/Matrix/GtMatrix.h>

#include <GONTI-CORE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <Algebra/Vectors/Vector/GtVector.h>

const GtMat GT_MAT_UNDEFINED = { 0, 0, NULL };

/*
* MAT's
*/
GtMat gontiMatAllocate(GtU32 rows, GtU32 cols) {
	GtMat ret;

	ret.rows = rows;
	ret.cols = cols;

	ret.elements = gt_allocate(rows * sizeof(GtF32*), GT_MEM_TAG_MATRIX);
	for (GtU32 i = 0; i < rows; i++)
	{
		ret.elements[i] = gt_allocate(cols * sizeof(GtF32), GT_MEM_TAG_MATRIX);
	}

	return ret;
}
GtMat gontiMatIdentity(GtU32 dim) {
	GtMat ret = gontiMatAllocate(dim, dim);

	for (GtU32 r = 0; r < dim; r++) {
		for (GtU32 c = 0; c < dim; c++) {
			ret.elements[r][c] = (r == c) ? 1.0f : 0.0f;
		}
	}

	return ret;
}
GtMat gontiMatZeroMatrix(GtU32 rows, GtU32 cols) {
	GtMat ret = gontiMatAllocate(rows, cols);

	for (GtU32 r = 0; r < rows; r++) {
		for (GtU32 c = 0; c < cols; c++) {
			ret.elements[r][c] = 0.0f;
		}
	}

	return ret;
}
GtMat gontiMatNewMatrix(GtU32 rows, GtU32 cols, GtU32 numVals, ...) {
	GtMat ret = gontiMatAllocate(rows, cols);

	va_list list;
    va_start(list, numVals);

    GtU32 i = 0;

    for (GtU32 r = 0; r < rows; r++) {
        for (GtU32 c = 0; c < cols; c++) {
            if (i++ < numVals) {
                ret.elements[r][c] = va_arg(list, GtF64);
            }
            else {
                ret.elements[r][c] = 0.0f;
            }
        }
    }

    va_end(list);
	return ret;
}
GtMat gontiMatNewMatrixFromArray(GtU32 rows, GtU32 cols, const GtF64* elements) {
	GtMat ret = gontiMatAllocate(rows, cols);

	for (GtU32 r = 0; r < rows; r++) {
		for (GtU32 c = 0; c < cols; c++) {
			ret.elements[r][c] = elements[r * cols + c];
		}
	}
	return ret;
}
GtMat gontiMatNewMatrixSafe(GtU32 rows, GtU32 cols, GtF64* row_data[], GtU32 row_count) {
	GtMat ret = gontiMatAllocate(rows, cols);

	GtU32 actual_rows = (row_count < rows) ? row_count : rows;

	for (GtU32 r = 0; r < actual_rows; r++) {
		for (GtU32 c = 0; c < cols; c++) {
			ret.elements[r][c] = row_data[r][c];
		}
	}

	return ret;
}
GtMat gontiMatCopy(GtMat m) {
	return m;
}
GtMat gontiMatCopyPtr(GtMat* m) {
	GtMat ret;
	gt_copyMemory(&ret, m, sizeof(GtMat));
	return ret;
}
GtMat gontiMatToRowVec(GtVec* v) {
	GtMat ret = gontiMatAllocate(1, v->dim);

	for (GtU32 i = 0; i < v->dim; i++) {
		ret.elements[0][i] = v->elements[i];
	}

	return ret;
}
GtMat gontiMatToColVec(GtVec* v) {
	GtMat ret = gontiMatAllocate(v->dim, 1);

	for (GtU32 i = 0; i < v->dim; i++) {
		ret.elements[i][0] = v->elements[i];
	}

	return ret;
}
GtMat gontiMatScalarDivision(GtMat m, GtF32 k) {
	GtMat ret = gontiMatAllocate(m.rows, m.cols);

	for (GtU32 r = 0; r < ret.rows; r++) {
		for (GtU32 c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = m.elements[r][c] + k;
		}
	}

	return ret;
}
GtMat gontiMatAdd(GtMat m1, GtMat m2) {
	if (m1.rows != m2.rows ||
		m1.cols != m2.cols) 
		return GT_MAT_UNDEFINED;

	GtMat ret = gontiMatAllocate(m1.rows, m1.cols);

	for (GtU32 r = 0; r < ret.rows; r++) {
		for (GtU32 c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = m1.elements[r][c] + m2.elements[r][c];
		}
	}

	return ret;
}
GtMat gontiMatMultiplication(GtMat m1, GtMat m2) {
	if (m1.cols != m2.rows) return GT_MAT_UNDEFINED;

	GtVec* m1Rows = gt_allocate(m1.rows * sizeof(GtVec), GT_MEM_TAG_MATH);
	GtVec* m2Cols = gt_allocate(m2.cols * sizeof(GtVec), GT_MEM_TAG_MATH);

	for (GtU32 r = 0; r < m1.rows; r++) {
		m1Rows[r] = gontiMatGetRow(&m1, r + 1);

	}
	for (GtU32 c = 0; c < m2.cols; c++) {
		m2Cols[c] = gontiMatGetCol(&m2, c + 1);
	}

	GtMat ret = gontiMatAllocate(m1.rows, m2.cols);

	for (GtU32 r = 0; r < ret.rows; r++) {
		for (GtU32 c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = gontiVecDot(m1Rows[r], m2Cols[c]);
		}
	}

	for (GtU32 r = 0; r < m1.rows; r++) {
		gontiVecFree(m1Rows + r);
	}
	for (GtU32 c = 0; c < m2.cols; c++) {
		gontiVecFree(m2Cols + c);
	}

	gt_free(m1Rows);
	gt_free(m2Cols);

	return ret;
}
GtMat gontiMatScalarMultiplication(GtMat m, GtF32 k) {
    GtMat ret = gontiMatAllocate(m.rows, m.cols);

    for (GtU32 r = 0; r < ret.rows; r++) {
        for (GtU32 c = 0; c < ret.cols; c++) {
            ret.elements[r][c] = m.elements[r][c] * k;
        }
    }

    return ret;
}
GtMat gontiMatTranspose(GtMat* m) {
	GtMat ret = gontiMatAllocate(m->cols, m->rows);

	for (GtU32 r = 0; r < ret.rows; r++) {
		for (GtU32 c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = m->elements[c][r];
		}
	}

	return ret;
}
GtMat gontiMatArgumentVector(GtMat* m, GtVec* v) {
	if (m->rows != v->dim) return GT_MAT_UNDEFINED;

	GtMat ret = gontiMatAllocate(m->rows, m->cols + 1);

	for (GtU32 r = 0; r < m->rows; r++) {
		GtU32 c = 0;

		for (; c < m->cols; c++) {
			ret.elements[r][c] = m->elements[r][c];
		}

		ret.elements[r][c] = v->elements[r];
	}

	return ret;
}
GtMat gontiMatArgumentMatrix(GtMat* m1, GtMat* m2) {
	if (m1->rows != m2->rows) return GT_MAT_UNDEFINED;

	GtMat ret = gontiMatAllocate(m1->rows, m1->cols + m2->cols);

	for (GtU32 r = 0; r < m1->rows; r++) {
		GtU32 c = 0;

		for (; c < m1->cols; c++) {
			ret.elements[r][c] = m1->elements[r][c];
		}

		for (; c < ret.cols; c++) {
			ret.elements[r][c] = m2->elements[r][c - m1->cols];
		}
	}

	return ret;
}
GtMat gontiMatSplice(GtMat* m, GtU32 exclRow, GtU32 exclCol) {
	exclRow--; exclCol--;

	GtMat ret = gontiMatAllocate(m->rows - 1, m->cols - 1);

	GtU32 offsetRow = 0;
	for (GtU32 r = 0; r < ret.rows; r++) {
		GtU32 offsetCol = 0;

		if (r == exclRow) offsetRow++;

		for (GtU32 c = 0; c < ret.cols; c++) {
			if (c == exclCol) offsetCol++;

			ret.elements[r][c] = m->elements[r + offsetRow][c + offsetCol];
		}
	}

	return ret;
}
GtMat gontiMatCofactorMatrix(GtMat* m) {
	if (m->rows != m->cols || m->rows == 0) return GT_MAT_UNDEFINED;

	GtMat ret = gontiMatAllocate(m->rows, m->cols);

	for (GtU32 r = 0; r < ret.rows; r++) {
		for (GtU32 c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = gontiMatCofactor(m, r + 1, c + 1);
		}
	}

	return ret;
}
GtMat gontiMatAdjugate(GtMat* m) {
	if (m->rows != m->cols || m->rows == 0) return GT_MAT_UNDEFINED;

	GtMat ret = gontiMatAllocate(m->rows, m->cols);

	for (GtU32 r = 0; r < ret.rows; r++) {
		for (GtU32 c = 0; c < ret.cols; c++) {
			ret.elements[r][c] = gontiMatCofactor(m, c + 1, r + 1);
		}
	}

	return ret;
}
GtMat gontiMatInverseMat(GtMat* m) {
	if (m->rows != m->cols || m->rows == 0) return GT_MAT_UNDEFINED;

	GtMat adj = gontiMatAdjugate(m);
	GtF32 det = 0.0f;

	for (GtU32 c = 0; c < m->cols; c++) {
		if (m->elements[0][c] != 0.0f) {
			det += m->elements[0][c] * adj.elements[c][0];
		}
	}

	if (det == 0.0f) return GT_MAT_UNDEFINED;

	GtMat ret = gontiMatScalarMultiplication(adj, 1.0f / det);
	gontiMatFree(&adj);

	return ret;
}

/*
* VEC's
*/
GtVec gontiMatGetRow(GtMat* m, GtU32 row) {
	row--;
	if (row >= m->rows) return VEC_UNDEFINED;

	GtVec ret = gontiVecAllocate(m->cols);

	for (GtU32 i = 0; i < ret.dim; i++) {
		ret.elements[i] = m->elements[row][i];
	}

	return ret;
}
GtVec gontiMatGetCol(GtMat* m, GtU32 col) {
	col--;
	if (col >= m->cols) return VEC_UNDEFINED;

	GtVec ret = gontiVecAllocate(m->rows);

	for (GtU32 i = 0; i < ret.dim; i++) {
		ret.elements[i] = m->elements[i][col];
	}

	return ret;
}
GtVec gontiMatMultiplicationVec(GtMat m, GtVec v) {
	if (m.cols != v.dim) return VEC_UNDEFINED;

	GtVec ret = gontiVecAllocate(m.rows);

	for (GtU32 r = 0; r < ret.dim; r++) {
		ret.elements[r] = gontiVecDot(v, gontiMatGetRow(&m, r + 1));
	}

	return ret;
}

/*
* VOID's
*/
void gontiMatPrint(GtMat m) {
	printf("\n");

	for (GtU32 r = 0; r < m.rows; r++) {
		printf("\n");
		printf("|");

		for (GtU32 c = 0; c < m.cols; c++) {
			printf(" %f", m.elements[r][c]);
		}

		printf(" |");
	}
	printf("\n");
}
void gontiMatScalarDivisionTo(GtMat* m, GtF32 k) {
	for (GtU32 r = 0; r < m->rows; r++) {
		for (GtU32 c = 0; c < m->cols; c++) {
			m->elements[r][c] += k;
		}
	}
}
void gontiMatScalarMultiplicationBy(GtMat* m, GtF32 k) {
    for (GtU32 r = 0; r < m->rows; r++) {
        for (GtU32 c = 0; c < m->cols; c++) {
            m->elements[r][c] *= k;
        }
    }
}
void gontiMatFree(GtMat* m) {
	for (GtU32 r = 0; r < m->rows; r++) {
		gt_free(m->elements[r]);
	}

	gt_free(m->elements);

	m->elements = NULL;
	m->rows = 0;
	m->cols = 0;
}
void gontiMatRef(GtMat* m) {
	GtU32 currentRow = 0;

	for (GtU32 c = 0; c < m->cols; c++) {
		GtU32 r = currentRow;

		if (r >= m->rows) break;

		for (; r < m->rows; r++) {
			if (m->elements[r][c] != 0.0f) break;
		}

		if (r == m->rows) continue;

		gontiMatSwapRows(m, currentRow, r);

		GtF32 factor = 1 / m->elements[currentRow][c];

		for (GtU32 col = c; col < m->cols; col++) {
			m->elements[currentRow][col] *= factor;
		}

		for (r = currentRow + 1; r < m->rows; r++) {
			gontiMatAddMultipleRow(m, r, currentRow, -1 * m->elements[r][c]);
		}

		currentRow++;
	}
}
void gontiMatRref(GtMat* m) {
	GtU32 currentRow = 0;

	for (GtU32 c = 0; c < m->cols; c++) {
		GtU32 r = currentRow;

		if (r >= m->rows) break;

		for (; r < m->rows; r++) {
			if (m->elements[r][c] != 0.0f) break;
		}

		if (r == m->rows) continue;

		gontiMatSwapRows(m, currentRow, r);

		GtF32 factor = 1 / m->elements[currentRow][c];

		for (GtU32 col = c; col < m->cols; col++) {
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
GtB8 gontiMatAddTo(GtMat* m1, GtMat m2) {
	if (m1->rows != m2.rows ||
		m1->cols != m2.cols) {
		return GtFalse;
	}

	for (GtU32 r = 0; r < m1->rows; r++) {
		for (GtU32 c = 0; c < m1->cols; c++) {
			m1->elements[r][c] += m2.elements[r][c];
		}
	}

	return GtTrue;
}
GtB8 gontiMatSwapRows(GtMat* m, GtU32 r1, GtU32 r2) {
	if (r1 >= m->rows || r2 >= m->rows || r1 == r2) return GtFalse;

	GtF32* temp = m->elements[r1];
	m->elements[r1] = m->elements[r2];
	m->elements[r2] = temp;

	return GtTrue;
}
GtB8 gontiMatAddRows(GtMat* m, GtU32 r1, GtU32 r2) {
	if (r1 >= m->rows || r2 >= m->rows || r1 == r2) return GtFalse;

	for (GtU32 c = 0; c < m->cols; c++) {
		m->elements[r1][c] += m->elements[r2][c];
	}

	return GtTrue;
}
GtB8 gontiMatMultiplyRow(GtMat* m, GtU32 r1, GtF32 k) {
	if (r1 >= m->rows || k == 0.0f) return GtFalse;

	for (GtU32 c = 0; c < m->cols; c++) {
		m->elements[r1][c] *= k;
	}

	return GtTrue;
}
GtB8 gontiMatAddMultipleRow(GtMat* m, GtU32 r1, GtU32 r2, GtF32 k) {
	if (r1 >= m->rows || r2 >= m->rows || r1 == r2 || k == 0.0f) return GtFalse;

	for (GtU32 c = 0; c < m->cols; c++) {
		m->elements[r1][c] += m->elements[r2][c] * k;
	}

	return GtTrue;
}
GtB8 gontiMatInvertible(GtMat* m) {
	if (m->rows != m->cols || m->rows == 0) return GtFalse;
	return gontiMatDeterminant(m) != 0.0f;
}

/*
* FLOAT
*/
GtF32 gontiMatDeterminant(GtMat* m) {
	if (m->rows != m->cols || m->rows == 0) return 0.0f;
	if (m->rows == 1) return m->elements[0][0];
	
	// char cofactorSign = 1;
	// GtF32 ret = 0.0f;
	
	// for (GtU32 c = 0; c < m->cols; c++) {
	// 	GtMat minor = gontiMatSplice(m, 1, c + 1);
	// 	ret += cofactorSign * m->elements[0][c] * gontiMatDeterminant(&minor);
	// 	cofactorSign = -cofactorSign;
	// }

	GtF32 ret = 0.0f;

	for (GtU32 c = 0; c < m->cols; c++) {
		if (m->elements[0][c] != 0.0f) {
			ret += m->elements[0][c] * gontiMatCofactor(m, 1, c + 1);
		}
	}
	
	return ret;
}
GtF32 gontiMatDeterminantExclusion(GtMat* m) {
	if (m->rows != m->cols || m->rows == 0) return 0.0f;

    GtU32 *skipCols = gt_allocate(m->cols * sizeof(GtU32), GT_MEM_TAG_MATH);
    GtU32 noSkipCols = 0;

    GtF32 ret = _gontiMatDeterminantExclusion(m, 1, 0, skipCols, &noSkipCols);

    gt_free(skipCols);

	return ret;
}
GtF32 _gontiMatDeterminantExclusion(GtMat* m, GtU32 row, GtU32 col, GtU32* skipCols, GtU32* noSkipCols) {
	skipCols[*noSkipCols] = col;
	(*noSkipCols)++;

	if (row == m->rows) {
		GtU32 c = m->cols;

		if (*noSkipCols != 0) {
			if (skipCols[*noSkipCols - 1] != 0) {
				while (gontiMathContainsUint(skipCols, *noSkipCols, c)) {
					c--;
				}
				(*noSkipCols)--;
			}
		}

		return m->elements[row - 1][c - 1];
	}

	GtF32 ret = 0.0f;
	char cofactorSign = 1;

	for (GtU32 c = 1; c <= m->cols; c++) {
		if (gontiMathContainsUint(skipCols, *noSkipCols, c)) continue;

		GtF32 res = 0.0f;
		if (m->elements[row - 1][c - 1] != 0.0f) 
			res = cofactorSign * m->elements[row - 1][c - 1] * _gontiMatDeterminantExclusion(m, row + 1, c, skipCols, noSkipCols);

		ret += res;
		cofactorSign = -cofactorSign;
	}

	(*noSkipCols)--;

	return ret;
}
GtF32 gontiMatCofactor(GtMat* m, GtU32 r, GtU32 c) {
	if (m->rows != m->cols || m->rows == 0 
	|| r > m->rows || c > m->cols) return 0.0f;

	char sign = ((r + c) & 1) == 0 ? 1 : -1;
	GtMat spliceMat = gontiMatSplice(m, r, c);

	GtF32 ret = sign * gontiMatDeterminant(&spliceMat);
	gontiMatFree(&spliceMat);

	return ret;
}
