#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <cassert>
#include <exception>

namespace MathUtils {
	class Column {
	public:
		explicit Column(std::vector<double> values);
		explicit Column(size_t size, size_t index);

		double& operator[](size_t n);
	private:
		std::vector<double> values_;
	};

	class Matrix {
	public:
		explicit Matrix(size_t n);
		explicit Matrix(std::vector<Column> columns);

		Column SolveSLAE();
		double Determinant();
		Matrix Inverse();
		void AddColumn(Column column);

		friend std::ostream& operator<<(std::ostream& stream, const Matrix& m);
	private:
		std::vector<std::vector<double>> matrix;

		void DivideRowByFirstElem(size_t index);
		void EraseLeftAndUpperRows();
		void SubtractRow(size_t index);
		Column CountRootColumn();
		void SwapByLeadingElem(size_t index);
	};
}

#endif
