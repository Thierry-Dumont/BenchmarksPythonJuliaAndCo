push!(LOAD_PATH, "./")
using Sparse23

function dotest(dim::Int,size::Int)
    t1 = time_ns()
    if dim==2
        M=PreLapl2(size)
        sizeV=size^2
    else
        M=PreLapl3(size)
        sizeV=size^3
    end
    t1 = time_ns()-t1
    V=Vector{Float64}([1 for i in 1:sizeV])
    W=Vector{Float64}([0 for i in 1:sizeV])
    #println(M.n," ",M.m)
    t2 = time_ns()
    A_mul_B!(W,M,V)
    t2 = time_ns()-t2

    return t1,t2
end


fw=open("RunningOn"*gethostname()*"_lapl_2","w")

# computation starts here:
println("2d:")
size=32
sizeM=2048
dotest(2,size)
while size<=sizeM
    t1,t2=dotest(2,size)
    println(size," ",float(t1)*10.0^(-9)," ",float(t2)*10.0^(-9))
    size*=2
end
println("3d:")
size=16
sizeM=128
dotest(3,size)
while size<=sizeM
    t1,t2=dotest(3,size)
    println(size," ",float(t1)*10.0^(-9)," ",float(t2)*10.0^(-9))
    size*=2
end
#

