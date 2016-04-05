/***************************************************************************
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
#include <cstdlib>
#include <cstdio>
#include <matiocpp/matiocpp.hpp>

int main(int argc, char **argv) {
	using namespace std;
	using namespace matiocpp;
	using namespace arma;

	// Create vector sequence
	vec vv = zeros<vec>(10);
	for(size_t i = 0; i < 10; ++i) vv(i) = i;

	// Create random matrix
	mat vvv = randu<mat>(5,5);
	vvv.print("VVV=");
	string v("hello");

	// Create cell array 10x1
	vector<size_t> dims(2);
	dims[0] = 10;
	dims[1] = 1;

	Cell vc(dims, true); // fill with empty matrices
	vc.set(0,v); // Set first element = string
	vc.set(1,vv); // Set second element = vector sequence
	vc.set(2,vvv); // Set third element = matrix

	Cell vc2(dims, true); // Second cell array
	vc2.set(0,v); // Copy first cell into second
	vc.set(3, vc2); // Set first from second

	// Create struct array (1x1)
	dims[0] = 1;
	dims[1] = 1;
	vector<string> fields = {"test","asd"};
	Struct ss(dims, fields);
	ss.set("test",0,vv); // Set first test field to vector sequence
	ss.set("asd",0,vc); // Set first asd field to first cell

	// Create second struct array (1x)
	Struct sss(dims, fields);
	sss.set("test",0,v); // Set test = string
	sss.set("asd",0,vv); // Set asd = vector sequence
	vc2.set(1,sss); // Set cell2 1 = struct 2

	// Create writer
	Writer mw("test.mat");
	// Write each element
	cout << "res:" << mw.write("vec",vv) << endl;
	cout << "res:" << mw.write("mat",vvv) << endl;
	cout << "res:" << mw.write("string",v) << endl;
	cout << "res:" << mw.write("cell1",vc) << endl;
	cout << "res:" << mw.write("str",ss) << endl;
	cout << "res:" << mw.write("cell2",vc2) << endl;

	// Create reader
	Reader mv("test.mat");
	// Read each element
	vec xx = mv.read("vec");
	mat xxx = mv.read("mat");
	Cell cc = mv.read("cell1");
	Struct ss2 = mv.read("str");
	vec xx2 = cc.get(1);
	vec xx3 = ss2.get("test",0);

	xx.print("XX=");
	xx2.print("XX2=");
	xx3.print("XX2=");
	xxx.print("XXX=");
	string str = mv.read("string");
	cout << str << endl;

	mat xxxd = mv.read("vec");

	return EXIT_SUCCESS;
}
