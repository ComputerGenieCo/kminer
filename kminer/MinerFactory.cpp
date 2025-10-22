/*
 * This file is part of kminer.
 *
Licensed under GPL v3.0
see LICENSE file for a full copy of the GNU General Public License
 *
 * Copyright (c) 2025 ComputerGenieCo
 */

#include "MinerFactory.h"

#include <thread>
#include <boost/log/trivial.hpp>

extern int use_avx;
extern int use_avx2;



MinerFactory::~MinerFactory()
{
	ClearAllSolvers();
}

std::vector<ISolver *> MinerFactory::GenerateSolvers(int cpu_threads, int cuda_count, int* cuda_en, int* cuda_b, int* cuda_t) {
	std::vector<ISolver *> solversPointers;

	for (int i = 0; i < cuda_count; ++i) {
		ISolver* solver = GenCUDASolver(cuda_en[i], cuda_b[i], cuda_t[i]);
		if (solver != nullptr) {
			solversPointers.push_back(solver);
		}
	}

	bool hasGpus = solversPointers.size() > 0;
	if (cpu_threads < 0) {
		cpu_threads = std::thread::hardware_concurrency();
		if (cpu_threads < 1) cpu_threads = 1;
		else if (hasGpus) --cpu_threads; // decrease number of threads if there are GPU workers
	}

#ifdef USE_CPU_XENONCAT
	for (int i = 0; i < cpu_threads; ++i)
	{
		solversPointers.push_back(GenCPUSolverXenoncat(use_avx2));
	}
#endif

	return solversPointers;
}

void MinerFactory::ClearAllSolvers() {
	for (ISolver * ds : _solvers) {
		if (ds != nullptr) {
			delete ds;
		}
	}
	_solvers.clear();
}

ISolver * MinerFactory::GenCPUSolver([[maybe_unused]] int use_opt) {
    // TODO fix dynamic linking on Linux
#ifdef    USE_CPU_XENONCAT
	_solvers.push_back(new CPUSolverXenoncat(use_opt));
	return _solvers.back();
#endif
	// If no CPU solver is enabled, return nullptr
	return nullptr;
}

#ifdef USE_CPU_XENONCAT
ISolver * MinerFactory::GenCPUSolverXenoncat([[maybe_unused]] int use_opt) {
    _solvers.push_back(new CPUSolverXenoncat(use_opt));
    return _solvers.back();
}
#endif

ISolver * MinerFactory::GenCUDASolver([[maybe_unused]] int dev_id, [[maybe_unused]] int blocks, [[maybe_unused]] int threadsperblock) {
#ifdef USE_CUDA_DJEZO
	// Validate device ID before creating solver
	int device_count = cuda_djezo::getcount();
	if (dev_id >= device_count) {
		BOOST_LOG_TRIVIAL(error) << "CUDA device " << dev_id << " not found. Available devices: 0-" << (device_count - 1);
		return nullptr;
	}
	_solvers.push_back(new CUDASolverDjezo(dev_id, blocks, threadsperblock));
	return _solvers.back();
#endif
	// If no CUDA solver is enabled, return nullptr
	return nullptr;
}
