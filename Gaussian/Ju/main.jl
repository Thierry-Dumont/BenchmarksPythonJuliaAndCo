push!(LOAD_PATH, "./")
using Rando
#= 
feed a Matrix with random data
=#
function RandomFeedMatrix(M,R::RandoData)
    n,m=size(M)
    @inbounds for i = 1:n
        @inbounds for j = 1:m
            M[i,j]=fv(R)
        end
    end
end


#= factor a matrix od size (n,n+1)
   size is not checked 
=#
function factorMatrix!(M::Array{Float64, 2})
    n, m = size(M)
    for line = 1:n
        # find pivot
         @inbounds(cmax= line-1+indmax(abs.(M[line:n,line])))
        # exchange rows if necessary
        if cmax != line
            @simd for j = line:m
                @inbounds M[line, j], M[cmax, j] = M[cmax, j], M[line, j]
            end
        end

        pivot = M[line, line]
        for k = line+1:m
            @inbounds tmp = M[line, k] / pivot
            @simd for j = line+1:n
                @inbounds M[j, k] -= M[j, line] * tmp
            end
        end
        
        M[line+1:n, line] .= 0.
    end

end



function doall(n::Int64)
    Ro = RandoData()
    M = Array{Float64}(n, n+1)
    RandomFeedMatrix(M, Ro)
    

    
    t1 = time_ns()
    factorMatrix!(M)
    t2 = time_ns() - t1

end

# main starts here.

MatrixSize = 2048
liminf = 2
D = Dict{Integer, Real}()
println("start")

#Profile.clear()

while MatrixSize > liminf
    T = doall(MatrixSize)
    println(MatrixSize)
    D[MatrixSize] = 1.e-9 * T
    MatrixSize = convert(Int64, MatrixSize/2)
end



# Results:
S = sort(collect(zip(keys(D), values(D))), rev=true)

open("RunningOn"*gethostname(),"w") do f
    for k in S
        write(f,string(k[1])," ",string(k[2]),"\n")
    end
end


#Profile.print()
