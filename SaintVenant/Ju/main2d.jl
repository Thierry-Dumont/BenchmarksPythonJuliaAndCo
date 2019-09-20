# Shallow water equations using a Lax-Friedrich finite volume scheme.
# flat bottom. Equations :
# h_t + div(hU) = 0                        (1)
# (hU)_t + div(h UxU + 1/2 gh^2 Id_2) = 0  (2)
#
# Setting V = (h, hu, hv), where U = (u, v), equations write :
# V_t + F(V)_x + G(V)_y = 0
#
# Where :
# F(V) = (hu, hu^2 + 1/2 gh^2, huv)
# G(V) = (hv, huv, hv^2 + 1/2 gh^2)
#
# Lax-Friedrich for 1D equation v_t + F(v)_x = 0
# v^{n+1}_i = 1/2 (v^n_{i-1} + v^n_{i+1}) - dt / (2 dx) * (F(v^n_{i+1}) - F(v^n_{i-1}))
#
# For our problem, in 2D, with a cartesian mesh :
# V^{n+1}_{i, j} = 1/2 (V^n_{i-1, j} + V^n_{i+1, j} + V^n_{i, j-1} + V^n_{i, j+1})
#                  - dt / (2 dx) * (F(V^n_{i+1, j}) - F(V^n_{i-1, j}))
#                  - dt / (2 dy) * (G(V^n_{i, j+1}) - G(V^n_{i, j-1}))



# perf or animation ?
const INFO = false
const record_animation = false



using Printf
using StaticArrays
if record_animation
    using Makie
end

using Profile


# the type for the domain
struct Domain
    x_start::Float64
    x_end::Float64
    y_start::Float64
    y_end::Float64
end



# Initial solution
function InitSolution!(V::Array{Float64, 3}, domain::Domain)
    ny = size(V, 2)
    nx = size(V, 3)

    hx = (domain.x_end - domain.x_start) / nx
    hy = (domain.y_end - domain.y_start) / ny
    
    xc = 0.5  * (domain.x_start + domain.x_end) + nx/8 * hx
    yc = 0.5  * (domain.y_start + domain.y_end) - ny/8 * hy
    
    for i=1:nx
        x = domain.x_start + (i-0.5) * hx
        
        for j=1:ny
            y = domain.y_start + (j-0.5) * hy

            if (x - xc)^2 + (y - yc)^2 < 0.2^2
                V[1, j, i] = 2
            else
                V[1, j, i] = 1
            end

            V[2, j, i] = 0
            V[3, j, i] = 0
        end
    end
end


# Model
function F!(f, V::Array{Float64, 3}, j::Int64, i::Int64)
    f[1] = V[2, j, i]
    if isapprox(V[1, j, i], 0., atol=eps(Float64), rtol=0)
        f[2] = 0
        f[3] = 0
    else
        f[2] = V[2, j, i]^2 / V[1, j, i] + 0.5 * 9.81 * V[1, j, i]^2
        f[3] = V[2, j, i] * V[3, j, i] / V[1, j, i]
    end
end

function G!(g, V::Array{Float64, 3}, j::Int64, i::Int64)
    g[1] = V[3, j, i]
    if isapprox(V[1, j, i], 0., atol=eps(Float64), rtol=0)
        g[2] = 0
        g[3] = 0
    else
        g[2] = V[2, j, i] * V[3, j, i] / V[1, j, i]
        g[3] = V[3, j, i]^2 / V[1, j, i] + 0.5 * 9.81 * V[1, j, i]^2
    end
end

        


# Scheme
function LaxFriedrich!(V::Array{Float64, 3}, Vold::Array{Float64, 3}, lambdas::Array{Float64, 2}, dt, dx, dy)
    ny = size(V, 2)
    nx = size(V, 3)
    
    Cx = dt / dx
    Cy = dt / dy
    
    flux1 = @MVector [0., 0., 0.]
    flux2 = @MVector [0., 0., 0.]

    # moving along x axis
    for j=1:ny
        F!(flux1, Vold, j, 1)

        # left boundary
        for k=2:3
            V[k, j, 1] += Cx * (flux1[k] - lambdas[j, 1] * Vold[k, j, 1])
        end
        
        for i=1:nx-1
            F!(flux2, Vold, j, i+1)
            lambda = max(lambdas[j, i], lambdas[j, i+1])
            
            for k=1:3
                flux = 0.5 * Cx * ((flux2[k] + flux1[k]) - lambda * (Vold[k, j, i+1] - Vold[k, j, i]))
                V[k, j, i] -= flux
                V[k, j, i+1] += flux
            end
            flux1, flux2 = flux2, flux1
        end

        # right boundary
        for k=2:3
            V[k, j, nx] -= Cx * (flux1[k] + lambdas[j, nx] * Vold[k, j, nx])
        end
    end

    # moving along y-axis
    for i=1:nx
        G!(flux1, Vold, 1, i)

        # bottom boundary
        for k=2:3
            V[k, 1, i] += Cy * (flux1[k] - lambdas[1, i] * Vold[k, 1, i])
        end
        
        for j=1:ny-1
            G!(flux2, Vold, j+1, i)
            lambda = max(lambdas[j, i], lambdas[j+1, i])
            
            for k=1:3
                flux = 0.5 * Cy * ((flux2[k] + flux1[k]) - lambda * (Vold[k, j+1, i] - Vold[k, j, i]))
                V[k, j, i] -= flux
                V[k, j+1, i] += flux
            end
            flux1, flux2 = flux2, flux1
        end

        # top boundary
        for k=2:3
            V[k, ny, i] -= Cy * (flux1[k] + lambdas[ny, i] * Vold[k, ny, i])
        end
    end
end

# Computes time step satisfying the CFL condition
function UpdateCFL!(lambdas::Array{Float64, 2}, V::Array{Float64, 3}, dx::Float64, dy::Float64)
    nx = size(V, 3)
    ny = size(V, 2)
    
    M = 0
    for i=1:nx
        for j=1:ny
            if V[1, j, i] > 0
                lambdas[j, i] = sqrt(V[2, j, i]^2 + V[3, j, i]^2) / V[1, j, i] + sqrt(9.81 * V[1, j, i])
                M = max(M, lambdas[j, i])
            else
                lambdas[j, i] = 0.
            end
        end
    end

    return 0.5 * min(dx / M, dy / M)
end



# initialization
const domain = Domain(0., 1., 0., 1.)
const Nx = 512
const Ny = 512


# loop in time
const T = 2.0
const dx = (domain.x_end - domain.x_start) / Nx
const dy = (domain.y_end - domain.y_start) / Ny


let t=0
    V = Array{Float64, 3}(undef, 3, Nx, Ny)
    InitSolution!(V, domain)
    
    Vold = Array{Float64, 3}(undef, 3, Nx, Ny)
    lambdas = Array{Float64, 2}(undef, Nx, Ny)

    function update_to_time(tframe::Float64)
        while(t < tframe)
            dt = min(UpdateCFL!(lambdas, V, dx, dy), tframe - t)
            @. Vold = V
            LaxFriedrich!(V, Vold, lambdas, dt, dx, dy)
            t += dt
            INFO && @printf("\t%.5f / %.5f\n", t, tframe)
        end
    end

    if record_animation
        nframe_per_second = 1000
        nframe = convert(Int64, ceil(nframe_per_second * T)) + 1

        scene = Scene(resolution = (1920, 1080))
        x = range(domain.x_start, stop=domain.x_end, length=Nx)
        y = range(domain.y_start, stop=domain.y_end, length=Ny)

        surf = surface!(scene, x, y, view(V, 1, :, :))[end]
        record(scene, "./height2d.mp4", range(t, stop=T, length=nframe), framerate=30) do tframe
            println(tframe, " / ", T)
            update_to_time(tframe)
            surf[3] = view(V, 1, :, :)
        end
    else
        println("Initial time t = ", t)
        Profile.clear()
        @profile update_to_time(T)
        println("End of simulation, t = ", t)
    end
end

Profile.print(maxdepth=10)
