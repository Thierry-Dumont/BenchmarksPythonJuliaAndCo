module Sparse23
export PreLapl2,PreLapl3
function PreLapl2(size::Int)
    ind(i::Int,j::Int)=(j-1)*size+i
    I=Int[]
    J=Int[]
    V=Float64[]
    h=1./(size-1)
    size2=size*size
    h2=h*h
    cd=-4.0/h2
    hd=1.0/h2
    nc=0
    for i=1:size
        for j=1:size
            ii=ind(i,j)
            push!(I,ii)
            push!(J,ii)
            push!(V,cd)
            nc+=1
            for i1 in [-1,1]
                if i+i1>0 && i+i1<=size
                    ii1=ind(i+i1,j)
                    push!(I,ii)
                    push!(J,ii1)
                    push!(V,hd)
                    nc+=1
                end
                if j+i1>0 && j+i1<=size
                    ii1=ind(i,j+i1)
                    push!(I,ii)
                    push!(J,ii1)
                    push!(V,hd)
                    nc+=1
                end
            end

        end
    end
   
    M=sparse(I,J,V)
    return M,size*size,nc
end
function PreLapl3(size)
    size2=size*size
    ind(i::Int,j::Int,k::Int)=(k-1)*size2+(j-1)*size+i
    I=Int[]
    J=Int[]
    V=Float64[]
    h=1./(size-1)
    size2=size*size
    h2=h*h
    cd=-6.0/h2
    hd=1.0/h2
    nc=0
    for i=1:size
        for j=1:size
            for k=1:size
                ii=ind(i,j,k)
                push!(I,ii)
                push!(J,ii)
                push!(V,cd)
                nc+=1
                for i1 in [-1,1]
                    if i+i1>0 && i+i1<=size
                        ii1=ind(i+i1,j,k)
                        push!(I,ii)
                        push!(J,ii1)
                        push!(V,hd)
                        nc+=1
                    end
                   if j+i1>0 && j+i1<=size
                       ii1=ind(i,j+i1,k)
                       push!(I,ii)
                       push!(J,ii1)
                       push!(V,hd)
                       nc+=1
                   end
                    if k+i1>0 && k+i1<=size
                        ii1=ind(i,j,k+i1)
                        push!(I,ii)
                        push!(J,ii1)
                        push!(V,hd)
                        nc+=1
                    end                    
                end                   
            end
        end
        
    end
    M=sparse(I,J,V)
    return M,size^3,nc
end
end
