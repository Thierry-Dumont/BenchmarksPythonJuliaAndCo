#pragma once

#include <stdio.h>
#include <cooperative_groups.h>

namespace cg = cooperative_groups;


__global__ void scheme_LaxFriedrich(double* V1, double* V2, double* Vold1, double* Vold2, double* lambdas, double dt, double dx, double tol, unsigned int nx);

__global__ void update_eigenvalues(double* lambdas, double* V1, double* V2, double tol, unsigned int nx);


__global__ void reduce_max(double *in, double *out, std::size_t N);
