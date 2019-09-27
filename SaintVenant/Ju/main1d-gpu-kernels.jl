# this file in only included

# Model
function device_F(V1::Float64, V2::Float64, atol)
    if CUDAnative.abs(V1) < atol
        return 0.
    else
        return V2 * V2 / V1 + 0.5 * 9.81 * V1 * V1
    end
end





# Scheme
function kernel_LaxFriedrich!(V, Vold, lambdas, Cx, atol)
    nx = length(lambdas)
    
    # index and stride of the cell that this gpu core will update
    gid = (blockIdx().x - 1) * blockDim().x + threadIdx().x
    lid = threadIdx().x

    sdata = CUDAnative.@cuDynamicSharedMem(Float64, 3 * (blockDim().x + 2))
    sVold = view(sdata, 1:2*(blockDim().x+2))
    sLmb = view(sdata, 2*(blockDim().x+2)+1:3*(blockDim().x+2))

    if gid <= nx
        sVold[2 * lid + 1] = Vold[2 * gid - 1]
        sVold[2 * lid + 2] = Vold[2 * gid]
        sLmb[lid + 1] = lambdas[gid]
        if lid == 1 && blockIdx().x > 1
            sVold[1] = Vold[2 * gid - 3]
            sVold[2] = Vold[2 * gid - 2]
            sLmb[1] = lambdas[gid - 1]
        end
        
        if lid == blockDim().x && blockIdx().x < gridDim().x
            sVold[2 * lid + 3] = Vold[2 * gid + 1]
            sVold[2 * lid + 4] = Vold[2 * gid + 2]
            sLmb[lid + 2] = lambdas[gid + 1]
        end
    end
    
    CUDAnative.sync_threads()
    
    if gid > 1 && gid < nx
        # mid-cell
        f_hu_m = device_F(sVold[2*lid-1], sVold[2*lid], atol)
        f_hu_p = device_F(sVold[2*lid+3], sVold[2*lid+4], atol)
        
        mlm = CUDAnative.max(sLmb[lid], sLmb[lid+1])
        mlp = CUDAnative.max(sLmb[lid+1], sLmb[lid+2])
        
        V[2*gid-1] += 0.5 * Cx * ((sVold[2*lid] - sVold[2*lid+4]) - mlm * (sVold[2*lid+1] - sVold[2*lid-1]) + mlp * (sVold[2*lid+3] - sVold[2*lid+1]))
        V[2*gid] += 0.5 * Cx * ((f_hu_m - f_hu_p) - mlm * (sVold[2*lid+2] - sVold[2*lid]) + mlp * (sVold[2*lid+4] - sVold[2*lid+2]))
    elseif gid==1
        # first cell
        f_hu_m = device_F(sVold[3], sVold[4], atol)
        f_hu_p = device_F(sVold[5], sVold[6], atol)
        ml = CUDAnative.max(sLmb[2], sLmb[3])
        
        V[1] -= 0.5 * Cx * ((sVold[6] + sVold[4]) - ml * (sVold[5] - sVold[3]))
        V[2] += Cx * (f_hu_m - sLmb[2] * sVold[4]) - 0.5 * Cx * ((f_hu_p + f_hu_m) - ml * (sVold[6] - sVold[4]))
    elseif gid==nx
        # last cell
        f_hu_m = device_F(sVold[2*lid-1], sVold[2*lid], atol)
        f_hu_p = device_F(sVold[2*lid+1], sVold[2*lid+2], atol)
        
        ml = CUDAnative.max(sLmb[lid], sLmb[lid+1])
        
        V[2*nx-1] += 0.5 * Cx * ((sVold[2*lid+2] + sVold[2*lid]) - ml * (sVold[2*lid+1] - sVold[2*lid-1]))
        V[2*nx] += 0.5 * Cx * ((f_hu_p + f_hu_m) - ml * (sVold[2*lid+2] - sVold[2*lid])) - Cx * (f_hu_p + sLmb[lid+1] * sVold[2*lid+2])
    end

    return nothing
end



# Update the eigenvalues
function kernel_UpdateEigenvalues!(lambdas, V, atol)
    nx = length(lambdas)
    
    # index and stride of the cell that this gpu core will update
    index = (blockIdx().x - 1) * blockDim().x + threadIdx().x

    if index <= nx
        if CUDAnative.abs(V[2*index-1]) < atol
            lambdas[index] = 0.
        else
            lambdas[index] = CUDAnative.abs(V[2*index] / V[2*index-1]) + CUDAnative.sqrt(9.81 * V[2*index-1])
        end
    end

    return nothing
end


# compute max of x for each block and put the values in mx
function reduce_max(x, mx, smem)
    nx = length(x)
    sdata = CUDAnative.@cuDynamicSharedMem(Float64, smem)

    tid = threadIdx().x
    i = (blockIdx().x - 1) * blockDim().x * 2 + threadIdx().x

    if i <= nx
        sdata[tid] = x[i]
        if i + blockDim().x < nx
            sdata[tid] = CUDAnative.max(sdata[tid], x[i + blockDim().x])
        end
    else
        sdata[tid] = -1.0
    end
    CUDAnative.sync_threads()

    if blockDim().x >= 512 && tid <= 256
        sdata[tid] = CUDAnative.max(sdata[tid], sdata[tid + 256])
    end
    CUDAnative.sync_threads()

    if blockDim().x >= 256 && tid <= 128
        sdata[tid] = CUDAnative.max(sdata[tid], sdata[tid + 128])
    end
    CUDAnative.sync_threads()
    
    if blockDim().x >= 128 && tid <= 64
        sdata[tid] = CUDAnative.max(sdata[tid], sdata[tid + 64])
    end
    CUDAnative.sync_threads()

    if tid <= 32
        if blockDim().x >= 64
            sdata[tid] = CUDAnative.max(sdata[tid], sdata[tid + 32])
        end

        offset = 16
        while offset >= 1
            sdata[tid] = CUDAnative.max(sdata[tid], CUDAnative.shfl_down(sdata[tid], offset))
            offset = CUDAnative.div(offset, 2)
        end
    end

    if tid == 1
        mx[blockIdx().x] = sdata[tid]
    end

    return nothing
end
