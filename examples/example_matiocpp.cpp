/***************************************************************************
 * Copyright (c) 2016 <Salva Ardid>                                        *
 * Copyright (c) 2014 <Lars Johannesen> All rights reserved.               *
 *                                                                         *
 * This file is part of MATIOCPP                                           *
 *                                                                         *
 * MATIOCPP is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * MATIOCPP is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with MATIOCPP.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                         *
 * compile with: g++ example.cpp -o example -larmadillo -lmatio -std=c++11 *
 ***************************************************************************/
#include <armadillo>
#include <matiocpp/matiocpp.hpp>

using namespace std;
using namespace matiocpp;
using namespace arma;

int main(int argc, char **argv) {

	// Create vector sequence
	vec ex_vector = zeros<vec>(10);
	for(size_t i = 0; i < 10; ++i) ex_vector(i) = i;
    ex_vector.print("ex_vector =");

	// Create random matrix
	mat ex_matrix = randu<mat>(5,5);
	ex_matrix.print("ex_matrix =");

    // Create int matrix
	imat ex_imatrix = randi<imat>(2, 3, distr_param(0,10));
	ex_imatrix.print("ex_imatrix =");

    // string
	string ex_string1("hello");
    cout << "ex_string1 = " << ex_string1 << endl;

	// Create cell array 10x1
	vector<size_t> dims(2);
	dims[0] = 10;
	dims[1] = 1;

	Cell ex_cell1(dims, true); // fill with empty matrices
	ex_cell1.set(0,ex_string1); // Set first element = string
	ex_cell1.set(1,ex_vector); // Set second element = vector sequence
	ex_cell1.set(2,ex_matrix); // Set third element = matrix

	Cell ex_cell2(dims, true); // Second cell array
	ex_cell2.set(0,ex_string1); // Set first element = string

    ex_cell1.set(3,ex_cell2); // Copy second cell into first

	// Create struct array (1x1)
	dims[0] = 1;
	dims[1] = 1;
	vector<string> fields = {"test","asd"};
	Struct ex_struct(dims, fields);
	ex_struct.set("test",0,ex_vector); // Set first test field to vector sequence
	ex_struct.set("asd",0,ex_cell1); // Set first asd field to first cell

	// Create second struct array (1x)
	Struct ex_struct2(dims, fields);
	ex_struct2.set("test",0,ex_string1); // Set test = string
	ex_struct2.set("asd",0,ex_vector); // Set asd = vector sequence

    // ex_cell2.set(1,ex_struct2); // Set cell2 1 = struct 2

	// Create saver
	Saver matFileNameW("test.mat");
	// Write each element
	cout << "status: " << matFileNameW.save("vector",ex_vector) << endl;
	cout << "status: " << matFileNameW.save("matrix",ex_matrix) << endl;
	cout << "status: " << matFileNameW.save("string1",ex_string1) << endl;
	cout << "status: " << matFileNameW.save("cell1",ex_cell1) << endl;
	cout << "status: " << matFileNameW.save("struct",ex_struct2) << endl;
	cout << "status: " << matFileNameW.save("cell2",ex_cell2) << endl;
    cout << "status: " << matFileNameW.save("imatrix",ex_imatrix) << endl;

	// Create loader
	Loader matFileNameR("test.mat");
	// Read each element
	vec ex_vector1 = matFileNameR.load("vector");
    ex_vector1.print("ex_vector1 =");

	mat ex_matrix1 = matFileNameR.load("matrix");
    ex_matrix1.print("ex_matrix1");

    mat ex_matrix2 = ex_matrix1*ex_matrix1;
    ex_matrix2.print("ex_matrix2 =");

    Cell ex_cell3 = matFileNameR.load("cell1");
    string ex_string2a = ex_cell3.get(0);
    cout << "ex_string2a = " << ex_string2a << endl;
    vec ex_vector2 = ex_cell3.get(1);
    ex_vector2.print("ex_vector2 =");

    Struct ex_struct3 = matFileNameR.load("struct");
	vec ex_vector3 = ex_struct3.get("asd",0);
	ex_vector3.print("ex_vector3 =");

	string ex_string2 = matFileNameR.load("string1");
    cout << "ex_string2 = " << ex_string2 << endl;

    imat ex_imatrix2 = matFileNameR.load("imatrix");
    ex_imatrix2.print("ex_imatrix2 =");

	return EXIT_SUCCESS;
}
