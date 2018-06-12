module Sparse23
export PreLapl2,PreLapl3
function PreLapl2(size::Int)
    I(i::Int64,j::Int64)=(j-1)*size+i
    #I(i,j)=(j-1)*size+i
    nc= 5*(size-2)^2+ 16*(size-2)+ 12
    row=Array{Int64}(nc)
    col=Array{Int64}(nc)
    v=Array{Float64}(nc)
    h=1./(size-1)
    size2=size*size
    h2=h*h
    cd=-4.0/h2
    hd=1.0/h2
    count=1
    for j=1:size
        for i=1:size
            l=I(i,j)
            row[count]=l
            col[count]=l
            v[count]=cd
            count+=1
            for ii1 =0:2:2
                i1=ii1-1
                if i+i1>0 && i+i1<=size
                    row[count]=l
                    col[count]=I(i+i1,j)
                    v[count]=hd
                    count+=1
                end
                if j+i1>0 && j+i1<=size
                    row[count]=l
                    col[count]=I(i,j+i1)
                    v[count]=hd
                    count+=1
                end
            end

        end
    end
    # for i=2:count
    #     if row[i-1]> row[i]
    #         println(i," ", row[i-1]," ",row[i])
    #     end
    # end
    M=sparse(row,col,v)
    return M,size2,count-1
end
function PreLapl3(size)
    #const size2=size*size
    I(i::Int64,j::Int64,k::Int64)=(k-1)*size*size+(j-1)*size+i
    nc= 7*(size-2)^3 + 36*(size-2)^2 + 60*(size-2)+ 8*4
    row=Array{Int64}(nc)
    col=Array{Int64}(nc)
    v=Array{Float64}(nc)
    h=1./(size-1)
    h2=h*h
    cd=-6.0/h2
    hd=1.0/h2
    count=1
    for k=1:size
        for j=1:size
            for i=1:size
                l=I(i,j,k)
                row[count]=l
                col[count]=l
                v[count]=cd
                count+=1
                for ii1 =0:2:2
                    i1=ii1-1
                    if i+i1>0 && i+i1<=size
                        row[count]=l
                        col[count]=I(i+i1,j,k)
                        v[count]=hd
                        count+=1
                    end
                    if j+i1>0 && j+i1<=size
                        row[count]=l
                        col[count]=I(i,j+i1,k)
                        v[count]=hd
                        count+=1
                    end
                    if k+i1>0 && k+i1<=size
                        row[count]=l
                        col[count]=I(i,j,k+i1)
                        v[count]=hd
                        count+=1
                    end                    
                end                
            end
        end
    end
    M=sparse(row,col,v)
    return M,size^3,count-1
end
end
