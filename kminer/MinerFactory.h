/*
 * This file is part of kminer.
 *
Licensed under GPL v3.0
see LICENSE file for a full copy of the GNU General Public License
 *
 * Copyright (c) 2025 ComputerGenieCo
 */

#pragma once

#include <AvailableSolvers.h>

class MinerFactory
{
public:
	MinerFactory(bool use_xenoncat, bool use_cuda_djezo)
		: _use_xenoncat(use_xenoncat), _use_cuda_djezo(use_cuda_djezo) {
	}

	~MinerFactory();

	std::vector<ISolver *> GenerateSolvers(int cpu_threads, int cuda_count, int* cuda_en, int* cuda_b, int* cuda_t);
	void ClearAllSolvers();

private:
	std::vector<ISolver *> _solvers;

	bool _use_xenoncat = true;
	bool _use_cuda_djezo = true;

	ISolver * GenCPUSolver(int use_opt);
#ifdef USE_CPU_XENONCAT
	ISolver * GenCPUSolverXenoncat(int use_opt);
#endif
	ISolver * GenCUDASolver(int dev_id, int blocks, int threadsperblock);
	ISolver * GenOPENCLSolver(int platf_id, int dev_id);

};

