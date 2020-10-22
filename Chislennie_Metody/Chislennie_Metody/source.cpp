#include <iomanip>
#include <vector>
#include <sstream>
#include <iostream>

#include "utils.h"

int main() {
	using namespace std;
	bool showMenu = true;
	MathUtils::Matrix matr(0u);
	while (showMenu) {
		cout << "Choose an option: \n";
		cout << "1. Set size. \n";
		cout << "2. Count the determinant.\n";
		cout << "3. Inverse.\n";
		cout << "4. Exit.\n";
		string command;
		cin >> command;
		if (command == "1") {
			cout << "Enter the size: " << endl;
			size_t n = 0;
			cin >> n;
			matr = MathUtils::Matrix(n);
			cout << "Matrix: " << endl;
			cout << matr;
		}
		else if (command == "2") {
			cout << "Determinant of the matrix: ";
			try {
				cout << setprecision(5) << matr.Determinant() << endl;
			}
			catch (const runtime_error& er) {
				cout << "An error has occured: " << er.what() << endl;
			}
		}
		else if (command == "3") {
			cout << "Matrix: " << endl;
			try {
				auto matrInversed = matr.Inverse();
				cout << matrInversed << endl;
			}
			catch (const runtime_error& er) {
				cout << "An error has occured: " << er.what() << endl;
			}
		}
		else {
			showMenu = false;
		}
	}
}