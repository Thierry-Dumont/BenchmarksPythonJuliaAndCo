#include "main1d-gpu-kernels.hpp"


__device__ inline double flux(const double V1, const double V2, const double tol) {
    if(fabs(V1) < tol)
        return 0.;
    else
        return V2 * V2 / V1 + 0.5 * 9.81 * V1 * V1;
}


__global__ void scheme_LaxFriedrich(double* V1, double* V2, double* Vold1, double* Vold2, double* lambdas, double dt, double dx, double tol, unsigned int nx) {
    cg::thread_block cta = cg::this_thread_block();
    extern __shared__ double shared_data[];
    
    const unsigned int gid = blockIdx.x * blockDim.x + threadIdx.x;
    const unsigned int lid = threadIdx.x + 1;

    const double Cx = dt / dx;

    double* sVold1 = shared_data;
    double* sVold2 = &shared_data[blockDim.x + 2];
    double* sLmb = &shared_data[2 * (blockDim.x + 2)];

    if(gid < nx) {
        sVold1[lid] = Vold1[gid];
        sVold2[lid] = Vold2[gid];
        sLmb[lid] = lambdas[gid];

        if(lid == 1 && blockIdx.x > 0) {
            sVold1[0] = Vold1[gid - 1];
            sVold2[0] = Vold2[gid - 1];
            sLmb[0] = lambdas[gid - 1];
        }

        if(lid == blockDim.x && blockIdx.x < gridDim.x - 1) {
            sVold1[lid + 1] = Vold1[gid + 1];
            sVold2[lid + 1] = Vold2[gid + 1];
            sLmb[lid + 1] = lambdas[gid + 1];
        }
    }

    cg::sync(cta);


    if(gid > 0 && gid < nx-1) {
        double f_hu_m = flux(sVold1[lid-1], sVold2[lid-1], tol);
        double f_hu_p = flux(sVold1[lid+1], sVold2[lid+1], tol);
        double mlm = fmax(sLmb[lid-1], sLmb[lid]);
        double mlp = fmax(sLmb[lid], sLmb[lid+1]);

        V1[gid] += 0.5 * Cx * ((sVold2[lid-1] - sVold2[lid+1]) - mlm * (sVold1[lid] - sVold1[lid-1]) + mlp * (sVold1[lid+1] - sVold1[lid]));
        V2[gid] += 0.5 * Cx * ((f_hu_m - f_hu_p) - mlm * (sVold2[lid] - sVold2[lid-1]) + mlp * (sVold2[lid+1] - sVold2[lid]));
    }
    else if(gid == 0) {
        double f_hu_m = flux(sVold1[1], sVold2[1], tol);
        double f_hu_p = flux(sVold1[2], sVold2[2], tol);
        double ml = fmax(sLmb[1], sLmb[2]);

        V1[0] -= 0.5 * Cx * ((sVold2[2] + sVold2[1]) - ml * (sVold1[2] - sVold1[1]));
        V2[0] += Cx * (f_hu_m - sLmb[1] * sVold2[1]) - 0.5 * Cx * ((f_hu_p + f_hu_m) - ml * (sVold2[2] - sVold2[1]));
    }
    else if(gid == nx - 1) {
        double f_hu_m = flux(sVold1[lid-1], sVold2[lid-1], tol);
        double f_hu_p = flux(sVold1[lid], sVold2[lid], tol);
        double ml = fmax(sLmb[lid-1], sLmb[lid]);

        V1[nx-1] += 0.5 * Cx * ((sVold2[lid] + sVold2[lid-1]) - ml * (sVold1[lid] - sVold1[lid-1]));
        V2[nx-1] += 0.5 * Cx * ((f_hu_p + f_hu_m) - ml * (sVold2[lid] - sVold2[lid-1])) - Cx * (f_hu_p + sLmb[lid] * sVold2[lid]);
    }
}


__global__ void update_eigenvalues(double* lambdas, double* V1, double* V2, double tol, unsigned int nx) {
    const unsigned int index = blockIdx.x * blockDim.x + threadIdx.x;

    if(index < nx) {
        if(fabs(V1[index]) < tol)
            lambdas[index] = 0.;
        else
            lambdas[index] = fabs(V2[index] / V1[index]) + sqrt(9.81 * V1[index]);
    }
}




__global__ void reduce_max(double *in, double *out, std::size_t N) {
    cg::thread_block cta = cg::this_thread_block();
    extern __shared__ double shared_data[];

    // perform first level of reduction,
    // reading from global memory, writing to shared memory
    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x * blockDim.x * 2 + threadIdx.x;

    double mm = i < N ? in[i] : -1.0;     // because in this problem, the values of in are > 0
    if(i + blockDim.x < N)
        mm = fmax(mm, in[i + blockDim.x]);
    
    shared_data[tid] = mm;
    cg::sync(cta);

    // do reduction in shared mem
    if((blockDim.x >= 512) && (tid < 256)) {
        shared_data[tid] = mm = fmax(mm, shared_data[tid + 256]);
    }
    cg::sync(cta);

    if((blockDim.x >= 256) && (tid < 128)){
        shared_data[tid] = mm = fmax(mm, shared_data[tid + 128]);
    }
    cg::sync(cta);

    if((blockDim.x >= 128) && (tid <  64)){
       shared_data[tid] = mm = fmax(mm, shared_data[tid +  64]);
    }
    cg::sync(cta);

    cg::thread_block_tile<32> tile32 = cg::tiled_partition<32>(cta);  // warp size
    if(cta.thread_rank() < 32) {
        // Fetch final intermediate max from 2nd warp
        if(blockDim.x >=  64) {
            mm = fmax(mm, shared_data[tid + 32]);
        }
        
        // Reduce final warp using shuffle
        for(int offset = tile32.size()/2; offset > 0; offset /= 2) {
            mm = fmax(mm, tile32.shfl_down(mm, offset));
        }
    }

    // write result for this block to global mem
    if(cta.thread_rank() == 0) {
        out[blockIdx.x] = mm;
    }
}