module Sparse23raw
export PreLapl2, PreLapl3
using SparseArrays


function PreLapl2(size::Int64)
    size2 = size * size

    nc = 5*(size - 2)^2 + 16*(size - 2) + 12
    colptr = Array{Int64}(undef, size2+1)
    rowval = Array{Int64}(undef, nc)
    nzval = Array{Float64}(undef, nc)

    h = 1. / (size - 1)
    h2 = h * h
    cd = -4. / h2
    hd = 1. / h2

    colptr[1] = 1
    count = 1
    for j=1:size
        for i=1:size
            l = (j-1) * size + i

            rowval[count] = l
            nzval[count] = cd
            numc = 1
            count += 1
            
            for i1 = -1:2:1
                if i+i1 > 0 && i+i1 <= size
                    rowval[count] = l + i1
                    nzval[count] = hd
                    numc += 1
                    count += 1
                end
                
                if j+i1 > 0 && j+i1 <= size
                    rowval[count] = l + i1 * size
                    nzval[count] = hd
                    numc += 1
                    count += 1
                end
            end

            colptr[l+1] = colptr[l] + numc
        end
    end

    M = SparseMatrixCSC{Float64, Int64}(size2, size2, colptr, rowval, nzval)
    return M, size2, count-1
end



function PreLapl3(size)
    size2 = size * size
    size3 = size2 * size
    
    nc = 7*(size - 2)^3 + 36*(size - 2)^2 + 60*(size - 2) + 8*4
    colptr = Array{Int64}(undef, size3+1)
    rowval = Array{Int64}(undef, nc)
    nzval = Array{Float64}(undef, nc)
        
    h = 1. / (size - 1)
    h2 = h * h
    cd = -6. / h2
    hd = 1. / h2

    colptr[1] = 1
    count = 1
    for k=1:size
        for j=1:size
            for i=1:size
                l = (k - 1) * size2 + (j - 1) * size + i

                rowval[count] = l
                nzval[count] = cd
                numc = 1
                count += 1
                
                for i1 = -1:2:1
                    if i+i1 > 0 && i+i1 <= size
                        rowval[count] = l + i1
                        nzval[count] = hd
                        numc += 1
                        count += 1
                    end
                    
                    if j+i1 > 0 && j+i1 <= size
                        rowval[count] = l + i1 * size
                        nzval[count] = hd
                        numc += 1
                        count += 1
                    end

                    if k+i1 > 0 && k+i1 <= size
                        rowval[count] = l + i1 * size2
                        nzval[count] = hd
                        numc += 1
                        count += 1
                    end
                end

                colptr[l+1] = colptr[l] + numc
            end
        end
    end

    M = SparseMatrixCSC{Float64, Int64}(size3, size3, colptr, rowval, nzval)
    return M, size^3, count-1
end
end
