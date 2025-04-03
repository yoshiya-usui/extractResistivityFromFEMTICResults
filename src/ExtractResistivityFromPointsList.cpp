//--------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2021 Yoshiya Usui
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//--------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <complex>
#include <iomanip>
#include <math.h>
#include <assert.h>
#include <algorithm>
#include <stdio.h>
#include <string.h>

#include "MeshDataTetraElement.h"
#include "MeshDataBrickElement.h"
#include "MeshDataNonConformingHexaElement.h"
#include "ResistivityBlock.h"

enum ElementType{
	TETRA = 0,
	BRICK,
	NONCONFORMING_HEXA
};

struct Coord3D{
	double X;
	double Y;
	double Z;
};

ResistivityBlock m_ResistivityBlock;

void run(const int elementType, const int interationNumber, const std::string& pointList);
void readPointList(const std::string& pointList, std::vector<Coord3D>& points);
void makeCutawayNonconformingHexa(const int interationNumber, std::ofstream& ofs, const std::vector<Coord3D>& points);

int main( int argc, char* argv[] ){
	if( argc < 4 ){
		std::cerr << "You must specify element type, interation number and input-file name !!" << std::endl;
		exit(1);
	}
	const int elementType = atoi(argv[1]);
	switch (elementType)
	{
		case NONCONFORMING_HEXA:
			std::cout << "Element type: non-conforming hexahedral element" << std::endl;
			break;
		case TETRA:
			// go through
		case BRICK:
			// go through
		default:
			std::cerr << "Unsupported element type " << elementType << std::endl;
			exit(1);
			break;
	}
	const int interationNumber = atoi(argv[2]);
	std::cout << "Iteration number: " << interationNumber << std::endl;
	const std::string inputFileName = argv[3];
	std::cout << "Input file name: " << inputFileName << std::endl;
	run(elementType, interationNumber, inputFileName);
	return 0;
}

void run( const int elementType, const int interationNumber, const std::string& pointList ){

	std::ostringstream ofile;
	ofile << "resistivity.txt";
	std::ofstream ofs(ofile.str().c_str(), std::ios::out);
	if (ofs.fail()) {
		std::cerr << "File open error : " << ofile.str() << " !!" << std::endl;
		exit(1);
	}

	std::vector<Coord3D> points;
	readPointList(pointList, points);
	switch (elementType){
		//case TETRA:
		//	makeCutaway();
		//	break;
		//case BRICK:
		//	makeCutawayBrick();
		//	break;
		case NONCONFORMING_HEXA:
			makeCutawayNonconformingHexa(interationNumber, ofs, points);
			break;
		default:
			std::cerr << "Unknown plane type !!" << std::endl;
			exit(1);
			break;
	}

	ofs.close();

}

void readPointList( const std::string& pointList, std::vector<Coord3D>& points){

	std::ifstream ifs(pointList.c_str(), std::ios::in );
	if( ifs.fail() ){
		std::cerr << "File open error : " << pointList.c_str() << " !!" << std::endl;
		exit(1);
	}

	std::string line;
	while (std::getline(ifs, line))
	{
		std::istringstream iss(line);
		Coord3D point = { 0.0,0.0,0.0 };
		iss >> point.X >> point.Y >> point.Z;
		point.X *= CommonParameters::convKilometerToMeter;
		point.Y *= CommonParameters::convKilometerToMeter;
		point.Z *= CommonParameters::convKilometerToMeter;
		points.push_back(point);
	}

	ifs.close();

}

void makeCutawayNonconformingHexa(const int interationNumber, std::ofstream& ofs, const std::vector<Coord3D>& points ){

	MeshDataNonConformingHexaElement m_meshDataNonConformingHexaElement;

	m_meshDataNonConformingHexaElement.inputMeshData();
	m_ResistivityBlock.inputResisitivityBlock(interationNumber);

	const int numElem = m_meshDataNonConformingHexaElement.getNumElemTotal();
	
	for (std::vector<Coord3D>::const_iterator itr = points.begin(); itr != points.end(); ++itr) {
		ofs << std::setw(12) << std::scientific << std::setprecision(3) << itr->X / CommonParameters::convKilometerToMeter;
		ofs << std::setw(12) << std::scientific << std::setprecision(3) << itr->Y / CommonParameters::convKilometerToMeter;
		ofs << std::setw(12) << std::scientific << std::setprecision(3) << itr->Z / CommonParameters::convKilometerToMeter;
		bool found(false);
		for (int iElem = 0; iElem < numElem; ++iElem) {
			if (m_meshDataNonConformingHexaElement.isLocatedInTheElement(itr->X, itr->Y, itr->Z, iElem)) {
				found = true;
				const int blockID = m_ResistivityBlock.getBlockIDFromElemID(iElem);
				const double resistivity = m_ResistivityBlock.getResistivityValuesFromBlockID(blockID);
				ofs << std::setw(12) << std::scientific << std::setprecision(3) << resistivity << std::endl;
				break;
			}
		}
		if (!found) {
			std::cerr << "Point (" << itr->X << ", " << itr->Y << ", " << itr->Z << ") cannot be found !!" << std::endl;
			exit(1);
		}
	}

}
