/*
 * This file is part of kminer.
 *
Licensed under GPL v3.0
see LICENSE file for a full copy of the GNU General Public License
 *
 * Copyright (c) 2025 ComputerGenieCo
 */

#pragma once

#include "Solver.h"
#include "SolverStub.h"


#ifdef USE_CPU_XENONCAT
#include "../cpu_xenoncat/cpu_xenoncat.hpp"
#else
CREATE_SOLVER_STUB(cpu_xenoncat, "cpu_xenoncat_STUB")
#endif
#ifdef USE_CUDA_DJEZO
#include "../cuda_djezo/cuda_djezo.hpp"
#else
CREATE_SOLVER_STUB(cuda_djezo, "cuda_djezo_STUB")
#endif
// OpenCL solvers are fropped replace with new OS solvers
#ifdef USE_OCL_XMP
#include "../ocl_xpm/ocl_xmp.hpp"
#else
CREATE_SOLVER_STUB(ocl_xmp, "ocl_xmp_STUB")
#endif
#ifdef USE_OCL_SILENTARMY
#include "../ocl_silentarmy/ocl_silentarmy.hpp"
#else
CREATE_SOLVER_STUB(ocl_silentarmy, "ocl_silentarmy_STUB")
#endif

//namespace AvailableSolvers
//{
//} // AvailableSolvers

// CPU solvers
class CPUSolverXenoncat : public Solver<cpu_xenoncat> {
public:
	CPUSolverXenoncat(int use_opt) : Solver<cpu_xenoncat>(new cpu_xenoncat(), SolverType::CPU) {
		_context->use_opt = use_opt;
	}
	virtual ~CPUSolverXenoncat() {}
};
// TODO remove platform id for cuda solvers
// CUDA solvers
class CUDASolverDjezo : public Solver<cuda_djezo> {
public:
	CUDASolverDjezo(int dev_id, int blocks, int threadsperblock) : Solver<cuda_djezo>(new cuda_djezo(0, dev_id), SolverType::CUDA) {
		if (blocks > 0) {
			_context->blocks = blocks;
		}
		if (threadsperblock > 0) {
			_context->threadsperblock = threadsperblock;
		}
	}
	virtual ~CUDASolverDjezo() {}
};
// OpenCL solvers
class OPENCLSolverSilentarmy : public Solver<ocl_silentarmy> {
public:
	OPENCLSolverSilentarmy(int platf_id, int dev_id) : Solver<ocl_silentarmy>(new ocl_silentarmy(platf_id, dev_id), SolverType::OPENCL) {
	}
	virtual ~OPENCLSolverSilentarmy() {}
};
class OPENCLSolverXMP : public Solver<ocl_xmp> {
public:
	OPENCLSolverXMP(int platf_id, int dev_id) : Solver<ocl_xmp>(new ocl_xmp(platf_id, dev_id), SolverType::OPENCL) {
	}
	virtual ~OPENCLSolverXMP() {}
};

