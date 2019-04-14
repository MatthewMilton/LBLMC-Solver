/*

Copyright (C) 2017-2019 Matthew Milton

This file is part of the LB-LMC Solver C++ Library.

LB-LMC Solver C++ Library is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LB-LMC Solver C++ Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LB-LMC Solver C++ Library.  If not, see <https://www.gnu.org/licenses/>.

*/

#ifndef SYSTEMCONDUCTANCE_HPP
#define SYSTEMCONDUCTANCE_HPP

#include <vector>
#include <string>
#include "LBLMC/DataTypes.hpp"
#include "TBDataTypes.hpp"

namespace LBLMC
{

/**
 * @brief encapsulates the square conductance matrix for a system model in LB-LMC
 *
 * This class is intended for use of computing the conductance matrix of a system in
 * offline simulation only.  The conductance matrix that is managed by this class
 * can be used in math tools to develop the resistance matrix that is to be used
 * in LB-LMC FPGA simulation.
 *
 * This class is used to compute conductance matrix by passing its matrix to LB-LMC
 * components to stamp in their conductances.  After the conductance matrix is stamped,
 * the matrix can be exported by this class to file or memory to be processed for
 * development of a LB-LMC simulation engine that is RTL synthesizable.
 *
 * @note This class is NOT intended for RTL Synthesis.
 *
 * @todo May rename this class to be Matrix or SquareMatrix since it could be used for
 * other things besides conductance matrices
 *
 */
class SystemConductance
{
private:
	MatrixRMXd matrix;
	unsigned int dimension;

public:

	/**
	 * parameter constructor
	 * @param dimension non-zero dimension of square conductance matrix (length or width)
	 */
	SystemConductance(unsigned int dimension);

	/**
	 * initialization constructor
	 *
	 * Initalizes conductance matrix to that given.  This constructor expects the dimension to match up
	 * to get given matrix
	 *
	 * @param dimension non-zero dimension of square conductance matrix (length or width)
	 * @param base matrix to initialize conductance matrix to
	 */
	SystemConductance(unsigned int dimension, MatrixRMXd* base);

	/**
	 * copy constructor
	 * @param base conductance matrix to copy from
	 */
	SystemConductance(const SystemConductance& base);

	/**
	 * resets conductance matrix to given matrix and dimensions
	 *
	 * If base's pointer is equal to zero, this method clears the matrix to all ZERO values.
	 *
	 * @param dimension non-zero dimension of square conductance matrix (length or width)
	 * @param base conductance matrix to copy from
	 */
	void reset(unsigned int dimension, MatrixRMXd* base = 0);

	/**
	 * resets conductance matrix to that of given SystemConductance
	 * @param base
	 */
	void reset(const SystemConductance& base);

	/**
	 * returns conductance matrix as a pointer
	 * @return pointer to conductance matrix data
	 */
	double* asPointer();

	/**
	 * returns conductance matrix as an array
	 *
	 * same as asPointer()
	 *
	 * @see asPointer()
	 *
	 * @return return conductance matrix as an array
	 */
	double* asArray();

	/**
	 * returns conductance matrix as a vector reference
	 *
	 * @return conductance matrix as vector
	 */
	//std::vector<NumType>& asVector();

	/**
	 * returns the conductance matrix as a Eigen3 Matrix Type
	 * @return
	 */
	MatrixRMXd& asEigen3Matrix();

	/**
	 * gets dimension of square conductance matrix
	 * @return dimension of matrix
	 */
	unsigned int getDimension();

	/**
	 * inverts the conductance matrix and stores the result into itself
	 * @throws std::runtime_error if matrix is singular (non-invertible)
	 */
	void invertSelf();

	/**
	 * generates a sparsity pattern of conductance matrix and stores into given string buffer
	 *
	 * In the SPY, 'X' is a non-zero value, '.' is a zero value
	 *
	 * @param buffer string buffer to store sparsity pattern
	 * @return given string buffer as a char array
	 */
	const char* spy(std::string& buffer);

	/**
	 * generates a sparsity pattern of conductance matrix and stores into a text file given by filename
	 *
	 * In the SPY, 'X' is a non-zero value, '.' is a zero value
	 *
	 * @param filename filename of the text file to store sparsity pattern
	 * @return 0 if successful, -1 if fails to open/write to file
	 */
	int exportSpy(const char* filename);

	/**
	 * exports the conductance matrix to a MATLAB ASCII double text file
	 *
	 * The exported file can be opened in MATLAB/Octave using load 'file.txt' command
	 * MATLAB/Octave expects ASCII matrix files to be saved as text file, not as .MAT.
	 * .MAT files are binary and interpreted as such by MATLAB/Octave.
	 *
	 * @param filename filename of the text file to store matrix
	 * @return 0 if successful, -1 if fails to open/write to file
	 */
	int exportAsASCIIMatlab(const char* filename);

	/**
	 * exports the conductance matrix to a Comma Separated Value (CSV) text file
	 *
	 * The exported file can be opened by many tools that support CSV, including:
	 * MATLAB / Octave (dlmread, csvread)
	 * Excel
	 * Kst
	 * Gnuplot
	 * and many more.
	 *
	 * @param filename filename of the text file to store matrix
	 * @return 0 if successful, -1 if fails to open/write to file
	 */
	int exportAsCSV(const char* filename);

	/**
	 * exports the conductance matrix to a C/C++ Header file containing a constant 2D array
	 *
	 * The exported header file can be used in projects based on the LB-LMC codebase
	 *
	 * @param filename filename of the header file to store matrix as array, without file extension; the actual filename will be "<filename>.hpp"
	 * @param mat_name C/C++ compatible name for the matrix array in header file; with no spaces
	 * @return 0 if successful, -1 if fails to open/write to file
	 */
	int exportAsCHeader(const char* filename, const char* mat_name);

	/**
	 * imports a matrix from a given MATLAB ASCII text file into the conductance matrix
	 *
	 * The imported MATLAB ASCII file must have been generated with MATLAB command: save file.txt matrix -ascii -double
	 *
	 * In this version, the imported matrix from the file is expected to be square and same dimension as this object.
	 * Also, this version does not check the file for proper formatting, so beware!
	 *
	 * @param filename filename of the matlab ASCII text file from which to import the matrix
	 * @return 0 if successful, -1 if fails to open/read the file
	 */
	int importFromASCIIMatlab(const char* filename);

};

typedef SystemConductance SystemResistance;	///< lazy alias of SystemConductance for Inverted Conductance matrix A = G^-1
typedef SystemConductance SquareMatrix;	///< lazy alias of SystemConductance for general square matrices

} //namespace LBLMC

#endif //SYSTEMCONDUCTANCE_HPP
