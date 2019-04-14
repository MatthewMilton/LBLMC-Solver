/*

Copyright (C) 2018-2019 Matthew Milton

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

#ifndef SYSTEMSOLVERGENERATOR_HPP
#define SYSTEMSOLVERGENERATOR_HPP

#include <vector>
#include <string>
#include "LBLMC/DataTypes.hpp"

namespace LBLMC
{

class SystemSolverGenerator
{
private:
	const NumType* A; ///< the inverted conductance matrix ( A = G^-1 of Gx=b )
	unsigned int dimension; ///< number of solutions in the system Gx=b
	unsigned int num_components; ///< number of components in system to contribute to vector b of Gx=b
	NumType zero_bound; ///< range from zero when determining whether Aij*bi=xi is close to zero to be ignored; defaults to 1e-12.

public:
	/**
	 * parameter constructor
	 * @param A the inverted conductance matrix ( A = G^-1 of Gx=b )
	 * @param dimension number of solutions in the system Gx=b
	 * @param num_components number of components in system to contribute to vector b of Gx=b
	 * @param zero_bound range from zero when determining whether Aij*bi=xi is close to zero to be ignored; defaults to 1e-12.
	 */
	SystemSolverGenerator(const NumType* A, unsigned int dimension, unsigned int num_components, NumType zero_bound = 1.0e-12);
	SystemSolverGenerator(const SystemSolverGenerator& base);

	void reset(const NumType* A, unsigned int dimension, unsigned int num_components, NumType zero_bound = 1.0e-12);
	void reset(const SystemSolverGenerator& base);

	const char* generateSystemSolver(std::string& buffer, const char* solver_name = "solveSystem",
			const char* A_name = "mat_name", const char* b_func_name = "aggregateSources");

	int generateSystemSolverAndExportC(const char* dir, const char* filename, const char* solver_name = "solveSystem",
			const char* A_name = "mat_name", const char* b_func_name = "aggregateSources");
};

} //namespace LBLMC

#endif //SYSTEMSOLVERGENERATOR_HPP
