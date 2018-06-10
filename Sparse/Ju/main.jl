function PreLapl2(size)
    I=Int[]
    J=Int[]
    V=Float64[]
    h=1./(size-1)
    size2=size*size
    h2=h*h
    cd=-4.0/h2
    hd=1.0/h2
    for i=2:size-1
        for j=2:size-1
            ind=j*size+i
            push!(I,ind)
            push!(J,ind)
            push!(V,cd)
            
            # push!(I,ind)
            # push!(J,ind-size)
            # push!(V,hd)
            # push!(I,ind)
            # push!(J,ind+size)
            # push!(V,hd)

            # push!(I,ind)
            # push!(J,ind-1)
            # push!(V,hd)
            # push!(I,ind)
            # push!(J,ind+1)
            # push!(V,hd)
        end
    end
    for i=1:size
        push!(I,i)
        push!(J,1)
        push!(V,cd)
        push!(I,i)
        push!(J,size)
        push!(V,cd)

        push!(I,1)
        push!(J,i)
        push!(V,cd)
        push!(I,size)
        push!(J,i)
        push!(V,cd)        
        
    end
    M=sparse(I,J,V)
end
function PreLapl3(size)
    
nothing
end
function dotest(dim::Int,size::Int)

    if dim==2
        M=PreLapl2(size)
        sizeV=size*size
    else
        M=PreLapl3(size)
        sizeV=size^3
    end
    println(M.n," ",M.m)
    V=Vector{Float64}([1 for i in 1:sizeV])
    #W=Vector{Float64}([0 for i in 1:size])
    #println(V)
    W=M*V
end


fw=open("RunningOn"*gethostname()*"_lapl_2","w")
dotest(2,32)
# computation starts here:
size=128
dotest(2,size)
#dotest(3,size)

