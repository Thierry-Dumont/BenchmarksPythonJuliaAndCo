push!(LOAD_PATH, "./")
using Sparse23

function dotest(dim::Int,size::Int)
    t1 = time_ns()
    if dim==2
        M,order,nc=PreLapl2(size)
        sizeV=size^2
    else
        M,order,nc=PreLapl3(size)
        sizeV=size^3
    end
    t1 = time_ns()-t1
    V=Vector{Float64}([1 for i in 1:sizeV])
    W=Vector{Float64}([0 for i in 1:sizeV])

    t2 = time_ns()
    A_mul_B!(W,M,V)
    t2 = time_ns()-t2

    return order,nc,t1,t2
end


fw=open("RunningOn"*gethostname(),"w")

# computation starts here:
println("\n2d:")
size=32
sizeM=2048
dotest(2,size)
@printf "%4s %8s %8s %10s %10s \n" "size" "order" "nc" "T.b" "T.p"
while size<=sizeM
    order,nc,t1,t2=dotest(2,size)
    @printf "%4d %8d %8d %10.2e %10.2e \n" size order nc float(t1)*10.0^(-9) float(t2)*10.0^(-9)
    size*=2
end
println("\n3d:")
size=16
sizeM=256
dotest(3,size)
@printf "%4s %8s %8s %10s %10s \n" "size" "order" "nc" "T.b" "T.p"
while size<=sizeM
    order,nc,t1,t2=dotest(3,size)
    @printf "%4d %8d %8d %10.2e %10.2e \n" size order nc float(t1)*10.0^(-9) float(t2)*10.0^(-9)
    size*=2
end


