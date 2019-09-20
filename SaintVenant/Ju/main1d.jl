# perf or animation ?
const INFO = false
const record_animation = false


using Printf
using StaticArrays
#using Makie
using Statistics
using DelimitedFiles




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
            V[1, i] = 1
        else
            V[1, i] = 0
        end

        V[2, i] = 0
    end
end


# Model
function F!(f, V::Array{Float64, 2}, i::Int64)
    f[1] = V[2, i]
    if isapprox(V[1, i], 0., atol=eps(Float64), rtol=0)
        f[2] = 0
    else
        f[2] = V[2, i]^2 / V[1, i] + 0.5 * 9.81 * V[1, i]^2
    end
end


# Scheme
function LaxFriedrich!(V::Array{Float64, 2}, Vold::Array{Float64, 2}, lambdas::Array{Float64, 1}, dt, dx)
    nx = size(V, 2)
    Cx = dt / dx
    
    flux1 = @MVector [0., 0.]
    flux2 = @MVector [0., 0.]


    F!(flux1, Vold, 1)

    # left boundary
    V[2, 1] += Cx * (flux1[2] - lambdas[1] * Vold[2, 1])

    # interior edges
    for i=1:nx-1
        F!(flux2, Vold, i+1)
        lambda = max(lambdas[i], lambdas[i+1])
        
        for k=1:2
            flux = 0.5 * Cx * ((flux2[k] + flux1[k]) - lambda * (Vold[k, i+1] - Vold[k, i]))
            V[k, i] -= flux
            V[k, i+1] += flux
        end
        flux1, flux2 = flux2, flux1
    end

    # right boundary
    V[2, nx] -= Cx * (flux1[2] + lambdas[nx] * Vold[2, nx])
end

# Computes time step satisfying the CFL condition
function UpdateCFL!(lambdas::Array{Float64, 1}, V::Array{Float64, 2}, dx::Float64)
    nx = size(V, 2)
    
    M = 0
    for i=1:nx
        if isapprox(V[1, i], 0., atol=eps(Float64), rtol=0)
            lambdas[i] = 0.
        else
            lambdas[i] = abs(V[2, i] / V[1, i]) + sqrt(9.81 * V[1, i])
            M = max(M, lambdas[i])
        end
    end
   
    return 0.5 * dx / M
end







# initialization
const domain = Domain(0., 1.)
const Nx = 16384


# loop in time
const T = 2.0
const dx = (domain.x_end - domain.x_start) / Nx

let t=0
    V = Array{Float64, 2}(undef, 2, Nx)
    InitSolution!(V, domain)
    
    Vold = Array{Float64, 2}(undef, 2, Nx)
    lambdas = Array{Float64, 1}(undef, Nx)

    function update_to_time(tframe::Float64)
        while(t < tframe)
            dt = min(UpdateCFL!(lambdas, V, dx), tframe - t)
            @. Vold = V
            LaxFriedrich!(V, Vold, lambdas, dt, dx)
            t += dt
            INFO && @printf("\t%.5f / %.5f\n", t, tframe)
        end
    end

    if record_animation
        nframe_per_second = 1000
        nframe = convert(Int64, ceil(nframe_per_second * T)) + 1

        scene = Scene(resolution = (1920, 1080))
        x = range(domain.x_start, stop=domain.x_end, length=Nx)

        plt = lines!(scene, x, view(V, 1, :), color=:blue, linewidth=2)[end]
        record(scene, "./height1d.mp4", range(t, stop=T, length=nframe), framerate=30) do tframe
            println(tframe, " / ", T)
            update_to_time(tframe)
            plt[2] = view(V, 1, :)
        end
    else
        print("Warming up... ")
        update_to_time(eps(Float64))
        println("Done")

        t = 0
        InitSolution!(V, domain)

        println("Initial time t = ", t)
        @time update_to_time(T)
        println("End of simulation, t = ", t)
        
        println("mean(h) = ", mean(V[1, :]))
        writedlm("sol-cpu", V[1, :], "\n")
    end
end
