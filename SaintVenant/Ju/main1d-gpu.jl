# perf or animation ?
const INFO = true
const record_animation = false

using Printf

using CUDAdrv
using CUDAnative
using CuArrays
CuArrays.allowscalar(false)

using Statistics
#using Makie
using DelimitedFiles


include("main1d-gpu-kernels.jl")


# the type for the domain
struct Domain
    x_start::Float64
    x_end::Float64
end


# Initial solution
function InitSolution!(V::Array{Float64, 2}, domain::Domain)
    nx = size(V, 2)

    hx = (domain.x_end - domain.x_start) / nx
    xc = 0.5  * (domain.x_start + domain.x_end) + nx/8 * hx
    
    for i=1:nx
        x = domain.x_start + (i-0.5) * hx
        if abs(x - xc) < 0.2
            V[1, i] = 1.
        else
            V[1, i] = 0.
        end

        V[2, i] = 0.
    end
end



function compute_max(mx::Array{Float64}, mx_d::CuArray{Float64}, x_d::CuArray{Float64}, nthreads, nblocks, smem)
    @cuda threads=nthreads blocks=nblocks shmem=smem reduce_max(x_d, mx_d, smem)
    copyto!(mx, 1, mx_d, 1, nblocks)
    return maximum(mx)
end



# initialization
const domain = Domain(0., 1.)
const Nx = 32768

# GPU parameters
const num_gpu_threads = 256
const num_gpu_blocks = ifelse(Nx % num_gpu_threads == 0, div(Nx, num_gpu_threads), div(Nx, num_gpu_threads) + 1)

const nblocks_max = div(num_gpu_blocks, 2) + rem(num_gpu_blocks, 2)
const smem_max = ifelse(num_gpu_threads <= 32, 64 * sizeof(Float64), num_gpu_threads * sizeof(Float64))
const smem_scheme = 3 * (num_gpu_threads + 2) * sizeof(Float64)

println("number of GPU blocks = ", num_gpu_blocks)
println("number of GPU threads per block = ", num_gpu_threads)

# loop in time
const T = 1e-3
const dx = (domain.x_end - domain.x_start) / Nx

# tolerance for dry region
const tol = eps(Float64)




let t=0
    # only V is store on cpu
    V = Array{Float64, 2}(undef, 2, Nx)
    InitSolution!(V, domain)

    # no need for Vold and lambdas to be on cpu
    V_d = CuArray{Float64, 1}(undef, 2*Nx)
    Vold_d = CuArray{Float64, 1}(undef, 2*Nx)
    
    lambdas_d = CuArray{Float64, 1}(undef, Nx)
    lmax_d = CuArray{Float64, 1}(undef, nblocks_max)
    lmax = Array{Float64, 1}(undef, nblocks_max)

    copyto!(V_d, 1, V, 1, length(V))

    # create kernels (we don't use @cuda macro because there is no need to convert the type
    # of the arguments before each call. We always use the same arguments)
    ker_args_eig = cudaconvert.([lambdas_d, V_d, tol])
    ker_tt_eig = Tuple{Core.Typeof.(ker_args_eig)...}
    ker_eig = cufunction(kernel_UpdateEigenvalues!, ker_tt_eig)
    
    ker_args_scheme = cudaconvert.([V_d, Vold_d, lambdas_d, 0., tol])
    ker_tt_scheme = Tuple{Core.Typeof.(ker_args_scheme)...}
    ker_scheme = cufunction(kernel_LaxFriedrich!, ker_tt_scheme)
    
    function update_to_time(tframe::Float64)
        while(t < tframe)
            # update the eigenvalues (first time iteration = initialization)
            #CuArrays.@sync begin
            @cuda threads=num_gpu_threads blocks=num_gpu_blocks kernel_UpdateEigenvalues!(lambdas_d, V_d, tol)
            #ker_eig(ker_args_eig...; threads=num_gpu_threads, blocks=num_gpu_blocks)
            #CUDAdrv.cudacall(ker_eig.fun, ker_tt_eig, ker_args_eig...; threads=num_gpu_threads, blocks=num_gpu_blocks)
            #end
            
            # compute time step (reduction operation on gpu because we are using CuArrays)
            #Cmax = compute_max(lmax_d, lambdas_d)
            Cmax = compute_max(lmax, lmax_d, lambdas_d, num_gpu_threads, nblocks_max, smem_max)
            dt = min(0.5 * dx / Cmax, tframe - t)
                        
            # copy of V to Vold on gpu (first time iteration = initialization)
            copyto!(Vold_d, 1, V_d, 1, 2*Nx)
                        
            # performs one time step
            #CuArrays.@sync begin
            #@cuda threads=num_gpu_threads blocks=num_gpu_blocks shmem=smem_scheme kernel_LaxFriedrich!(V_d, Vold_d, lambdas_d, dt / dx, tol)
            ker_args_scheme[4] = dt / dx
            ker_scheme(ker_args_scheme...; threads=num_gpu_threads, blocks=num_gpu_blocks, shmem=smem_scheme)
            #CUDAdrv.cudacall(ker_scheme.fun, ker_tt_scheme, ker_args_scheme..., threads=num_gpu_threads, blocks=num_gpu_blocks, shmem=smem_scheme)
            #end

            t += dt
            INFO && @printf("\t%.5f / %.5f\n", t, tframe)
        end
    end    

    print("Warming up... ")
    update_to_time(eps(Float64))
    println("Done")
    
    t = 0
    InitSolution!(V, domain)
    copyto!(V_d, 1, V, 1, length(V))

    println("Initial time t = ", t)
    CUDAdrv.@profile update_to_time(T)
    #CuArrays.@time update_to_time(T)
    println("End of simulation, t = ", t)

    
    copyto!(V, 1, V_d, 1, length(V))
    println("mean(h) = ", mean(V[1, :]))
    writedlm("sol-gpu", V[1, :], "\n")
    
    #scene = lines(range(0, stop=1, length=Nx), V[1, :])
    #Makie.save("plot_h_gpu.png", scene)
end
