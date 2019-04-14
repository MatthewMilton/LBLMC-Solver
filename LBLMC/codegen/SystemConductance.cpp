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

#include "SystemConductance.hpp"

#include <vector>
#include <fstream>
#include <iomanip>
#include <stdexcept>

namespace LBLMC
{

SystemConductance::SystemConductance(unsigned int dimension):
	matrix(MatrixRMXd::Zero(dimension,dimension)), dimension(dimension)
{
	//do nothing else
}

SystemConductance::SystemConductance(unsigned int dimension, MatrixRMXd* base):
		matrix(*base), dimension(dimension)
{
	//do nothing else
}

SystemConductance::SystemConductance(const SystemConductance& base) :
		matrix(base.matrix), dimension(base.dimension)
{
	//do nothing else
}

void SystemConductance::reset(unsigned int dimension, MatrixRMXd* base)
{
	if(base == 0)
	{
		this->dimension = dimension;
		this->matrix.setZero();
		return;
	}

	this->dimension = dimension;
	this->matrix = *base;
}

void SystemConductance::reset(const SystemConductance& base)
{
	dimension = base.dimension;
	matrix = base.matrix;
}

double* SystemConductance::asPointer()
{
	return matrix.data();
}

double* SystemConductance::asArray()
{
	return matrix.data();
}
//std::vector<NumType>& SystemConductance::asVector()
//{
//	return matrix;
//}

MatrixRMXd& SystemConductance::asEigen3Matrix()
{
	return matrix;
}

unsigned int SystemConductance::getDimension()
{
	return dimension;
}

void SystemConductance::invertSelf()
{
	if(!matrix.fullPivLu().isInvertible())
	{
		throw std::runtime_error("SystemConductance: cannot invert conductance matrix as it is singular");
	}

	matrix = matrix.fullPivLu().inverse();
}

const char* SystemConductance::spy(std::string& buffer)
{
	buffer.clear();

	for(unsigned int r = 0; r < dimension; r++)
	{
		for(unsigned int c = 0; c < dimension; c++)
		{
			if( matrix(r,c)  == 0.0 )
				buffer += ". ";
			else
				buffer += "X ";
		}
		buffer += '\n';
	}

	return buffer.c_str();
}

int SystemConductance::exportSpy(const char* filename)
{
	std::fstream file;

	try
	{
		file.open(filename, std::fstream::out | std::fstream::trunc);
	}
	catch(...)
	{
		return -1;
	}

	for(unsigned int r = 0; r < dimension; r++)
	{
		for(unsigned int c = 0; c < dimension; c++)
		{
			if( matrix(r,c)  == 0.0 )
				file << '.';
			else
				file << 'X';
		}
		file << '\n';
	}
	file << std::flush;

	file.close();

	return 0;
}

int SystemConductance::exportAsASCIIMatlab(const char* filename)
{
	std::fstream file;

	try
	{
		file.open(filename, std::fstream::out | std::fstream::trunc);
	}
	catch(...)
	{
		return -1;
	}

	file << std::setprecision(16);
	file << std::fixed;
	file << std::scientific;

	for(unsigned int r = 0; r < dimension; r++)
	{
		for(unsigned int c = 0; c < dimension; c++)
		{
			file << "   " << matrix(r,c);
		}
		file << "\n";
	}
	file << std::flush;

	file.close();

	return 0;

}

int SystemConductance::exportAsCSV(const char* filename)
{
	std::fstream file;

	try
	{
		file.open(filename, std::fstream::out | std::fstream::trunc);
	}
	catch(...)
	{
		return -1;
	}

	file << std::setprecision(16);
	file << std::fixed;
	file << std::scientific;

	for(unsigned int r = 0; r < dimension; r++)
	{
		file << matrix(r,0);

		for(unsigned int c = 1; c < dimension; c++)
		{
			file << ", " << matrix(r,c);
		}
		file << "\n";
	}
	file << std::flush;

	file.close();

	return 0;
}

int SystemConductance::exportAsCHeader(const char* filename, const char* mat_name)
{
	std::fstream file;

	std::string fname = filename;
	fname += ".hpp";

	try
	{
		file.open(fname.c_str(), std::fstream::out | std::fstream::trunc);
	}
	catch(...)
	{
		return -1;
	}

	file << std::setprecision(16);
	file << std::fixed;
	file << std::scientific;

	file <<
			"/**\n"
			" *\n"
			" * LBLMC Vivado HLS Simulation Engine for FPGA Designs\n"
			" *\n"
			" * Auto-generated by SystemConductance Object\n"
			" *\n"
			" * NOTE: For this header, do not include outside the system solver to avoid linkage/compilation issues\n"
			" *\n"
			" */\n\n";

	file << "#ifndef " << mat_name << "_HPP" << "\n";
	file << "#define " << mat_name << "_HPP" << "\n";

	file << "\n#include \"LBLMC/DataTypes.hpp\"\n\n";

	file << "const LBLMC::NumType " << mat_name << "[" << dimension << "][" << dimension << "] =\n{";

	for(unsigned int r = 0; r < dimension; r++)
	{
		file << "{" << matrix(r,0);

		for(unsigned int c = 1; c < dimension; c++)
		{
			file << "," << matrix(r,c);
		}
		file << "}";

		if(r != dimension-1) file << ",";

		file << "\n";
	}

	file << "};\n";

	file << "\n#endif";

	file << std::flush;

	file.close();

	return 0;
}

int SystemConductance::importFromASCIIMatlab(const char* filename)
{
	std::fstream file;

	try
	{
		file.open(filename, std::fstream::in);
		if(file.fail()) return -1;
	}
	catch(...)
	{
		return -1;
	}

	reset(dimension);

	//in a lazy way, we are not checking the matlab file for proper formatting here, so beware!

	for(unsigned int i = 0; i < dimension*dimension; i++)
	{
		file >> (matrix.data())[i];
	}

	file.close();

	return 0;
}

} //namespace LBLMC
