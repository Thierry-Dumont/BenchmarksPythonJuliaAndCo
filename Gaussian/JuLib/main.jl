#=
Coefficients for a simple, but reproductible random number generator.
=#
mutable struct RandoData
    seed
    a
    c
    m
    RandoData()=new(123456789,1103515245,12345,4294967296)
end
#=
return a random float in[0,vmax]
=#
function fv(R::RandoData,vmax=1.)
    R.seed= (R.a * R.seed + R.c) % R.m
    vmax*convert(Float64,R.seed)/R.m
end
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
function doall(n::Int64)
    Ro= RandoData()
    M=Array{Float64}(n,n+1)
    RandomFeedMatrix(M,Ro)
    t1 = time_ns()
    factorize(M)
    time_ns()-t1
end

# main starts here.
MatrixSize=2048
liminf=2
D= Dict{Integer,Real}()
println("start")


while MatrixSize>liminf
    T=doall(MatrixSize)
    println(MatrixSize)
    D[MatrixSize]=T*1.e-9
    MatrixSize=convert(Int64,MatrixSize/2)
end
#


# Results:
S=sort(collect(zip(keys(D),values(D))),rev=true)

open("RunningOn"*gethostname(),"w") do f
    for k in S
        write(f,string(k[1])," ",string(k[2]),"\n")
    end
end

