push!(LOAD_PATH, "./")
# if you want to experiment with the "pushed" matrix creation method,
# switch comments on the two following lines:
using Sparse23
#using Sparse23push
using Printf
using SparseArrays,LinearAlgebra

function dotest(dim::Int,size::Int)
    # 1) matrix creation:
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

    # 2) matrix x vector product.
    t2 = time_ns()
    #A_mul_B!(W,M,V)
    mul!(W,M,V)
    
    t2 = time_ns()-t2

    return order,nc,t1,t2
end

println()
@printf "%-5s %1s %-20s\n" "size" ":" "size of the grid."
@printf "%-5s %1s %-20s\n" "order" ":" "order of the matrix."
@printf "%-5s %1s %-20s\n" "nc" ":" "number of ceofficients."
@printf "%-5s %1s %-20s\n" "T.b" ":" "time to build the matrix (s)."
@printf "%-5s %1s %-20s\n" "T.p" ":" "time for matrix x vector product (s)."

# computation starts here:
for dim=2:3
    println("\n"*string(dim)*"d:")
    fw=open("RunningOn"*gethostname()*"-"*string(dim),"w")
    if dim==2
        size=32
        sizeM=2048
    else
        size=16
        sizeM=256
    end
    @printf "%4s %8s %9s %10s %10s \n" "size" "order" "nc" "T.b" "T.p"
    while size<=sizeM
        
        order,nc,t1,t2=dotest(dim,size)#warmup
        
        order,nc,t1,t2=dotest(dim,size)

 
        s=@sprintf "%4d %8d %9d %10.2e %10.2e \n" size order nc float(t1)*10.0^(-9) float(t2)*10.0^(-9)
        print(s)
        write(fw,s)
        size*=2
    end
    close(fw)
end
