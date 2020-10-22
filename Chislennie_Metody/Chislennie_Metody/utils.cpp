#include "utils.h"

namespace MathUtils {
	Matrix::Matrix(size_t n) {
		matrix.resize(n);
		for (auto& i : matrix) {
			i.resize(n);
		}

		for (size_t i = 1; i <= matrix.size(); i++) {
			for (size_t j = 1; j <= matrix.size(); j++) {
				matrix[i - 1][j - 1] = (i == j) ? (matrix.size()) / (2. * i) : (2. * i) / (((double)i + j) * ((double)i + j));
			}
		}
	}

	Matrix::Matrix(std::vector<Column> columns) {
		matrix.resize(columns.size());
		for (auto& row : matrix) {
			row.resize(columns.size());
		}

		for (size_t i = 0; i < columns.size(); i++) {
			for (size_t j = 0; j < columns.size(); j++) {
				matrix[i][j] = columns[j][i];
			}
		}
	}

	double Matrix::Determinant() {
		Matrix copy = *this;
		double toReturn = 1.;
		while (copy.matrix.size() != 0)
		{
			toReturn *= copy.matrix[0][0];
			copy.DivideRowByFirstElem(0u);
			copy.SubtractRow(0u);
			copy.EraseLeftAndUpperRows();
		}
		return toReturn;
	}

	Column Matrix::CountRootColumn() {
		std::vector<double> roots;
		size_t height = matrix.size();
		size_t width = matrix[0].size();
		roots.resize(height);
		roots[0] = matrix[height - 1][width - 1];
		for (size_t i = 1; i < roots.size(); i++) {
			roots[i] = matrix[height - 1 - i][width - 1];
			for (size_t j = 1; j <= i; j++) {
				roots[i] -= matrix[height - 1 - i][width - 1 - j] * roots[j - 1];
			}
		}
		std::reverse(roots.begin(), roots.end());
		return Column(move(roots));
	}

	Column Matrix::SolveSLAE() {
		Matrix copy = *this;
		for (size_t i = 0; i < matrix.size(); i++) {
			copy.DivideRowByFirstElem(i);
			copy.SubtractRow(i);
		}
		return copy.CountRootColumn();;
	}

	Matrix Matrix::Inverse() {
		std::vector<Column> columns;
		size_t mSize = matrix.size();
		columns.reserve(mSize);
		for (size_t i = 0; i < mSize; i++) {
			Matrix copy = *this;
			copy.AddColumn(Column(mSize, i));
			columns.push_back(copy.SolveSLAE());
		}
		return Matrix(move(columns));
	}

	void Matrix::EraseLeftAndUpperRows() {
		matrix.erase(matrix.begin());
		for (auto& row : matrix) {
			row.erase(row.begin());
		}
	}

	void Matrix::SubtractRow(size_t index) {
		auto& row = matrix[index];
		for (size_t i = index + 1; i < matrix.size(); i++) {
			auto temp = matrix[i][index];
			for (size_t j = 0; j < matrix[0].size(); j++) {
				matrix[i][j] -= row[j] * temp;
			}
		}
	}

	void Matrix::AddColumn(Column column) {
		for (size_t i = 0; i < matrix.size(); i++) {
			matrix[i].push_back(column[i]);
		}
	}

	void Matrix::SwapByLeadingElem(size_t index) {
		double max_elem = std::numeric_limits<double>::min();
		size_t max_index = -1;
		for (size_t i = index; index < matrix.size(); i++) {
			if (matrix[i][index] != 0 && matrix[i][index] > max_elem) {
				max_index = i;
				max_elem = matrix[i][index];
			}
		}
		if (max_index != -1) {
			std::swap(matrix[max_index], matrix[index]);
		}
		else {
			throw std::runtime_error("The matrix has multiple solutions");
		}
	}

	void Matrix::DivideRowByFirstElem(size_t index) {
		auto temp = matrix[index][index];
		if (temp == 0.) {
			SwapByLeadingElem(index);
		}
		for (size_t j = index; j < matrix[index].size(); j++) {
			matrix[index][j] /= temp;
		}
	}

	Column::Column(std::vector<double> values) : values_(move(values)) {}

	Column::Column(size_t size, size_t index) {
		values_.resize(size);
		values_[index] = 1;
	}

	double& Column::operator[](size_t n) {
		return values_[n];
	}

	std::ostream& operator<<(std::ostream& stream, const Matrix& m) {
		for (size_t i = 0; i < m.matrix.size(); i++) {
			for (size_t j = 0; j < m.matrix[0].size(); j++) {
				stream << "|";
				stream << std::setw(7) << std::setprecision(3) << std::fixed << std::right << m.matrix[i][j];
			}
			stream << "|";
			stream << std::endl;
		}
		return stream;
	}
}