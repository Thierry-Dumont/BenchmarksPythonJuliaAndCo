module Weno
export weno!,WenoData
#using Devectorize
#===================================================
Weno 5  method in 1d.
===================================================#
struct WenoData
    c
    dright
    dleft
    b0::Float64
    b1::Float64
    epsilon::Float64
    size::Int32
    InC::Array{Float64}
    right::Array{Float64}
    left::Array{Float64}
    reconstructed::Array{Float64}
    numflux::Array{Float64}
    alpharight::Array{Float64}
    alphaleft::Array{Float64}
    beta::Array{Float64}
WenoData(size)=new(Array([
    [11./6.,-7./6.,1./3.],
    [1./3.,5./6.,-1./6.],
    [-1./6.,5./6.,1./3.],
    [1./3.,-7/6.,11./6.]]),
               Array([3./10.,3./5.,1./10.]),
               Array([1./10.,3./5.,3.10]),
               13./12.,
               1./4.,
                   1.e-6,
                   size,
                   Array{Float64}(size+4),
                   Array{Float64}(3),Array{Float64}(3),
                   Array{Float64}(2*size+8),
                   Array{Float64}(size+1),
                   Array{Float64}(3),Array{Float64}(3),
                   Array{Float64}(3)
               )

end

function weno!(W,F,L,In::Array{Float64},Out::Array{Float64})

    const size=length(In)

    size1=size+1
    size4=size+4
    const h1 = -1./(L/size)
    
    # build an extended array with phantom cells to deal with periodicity:
    W.InC[1]=In[size-1]
    W.InC[2]=In[size]
    W.InC[3:2+size]=In
    W.InC[size+3]=In[1]
    W.InC[size+4]=In[2]
 
    # lets's start computation: 
    for vol= 3:2+size
        @simd for r= 0:2
            W.left[r+1] =dot(W.c[r+1],W.InC[vol-r:vol-r+2])
            #W.left[r+1] = sum(W.c[r+1].*W.InC[vol-r:vol-r+2])
            W.right[r+1]=dot(W.c[r+2],W.InC[vol-r:vol-r+2])
            #W.left[r+1] = sum(W.c[r+1].*W.InC[vol-r:vol-r+2])
            #W.right[r+1]= sum(W.c[r+2].*W.InC[vol-r:vol-r+2])
        end
         # regularity coefficients
        W.beta[1]=W.b0* (W.InC[vol]-2.0*W.InC[vol+1]+W.InC[vol+2])^2+ 
	W.b1*(3.*W.InC[vol]-4.*W.InC[vol+1]+W.InC[vol+2])^2
        
        W.beta[2]=W.b0*(W.InC[vol-1]-2.0*W.InC[vol]+W.InC[vol+1])^2+ 
        W.b1*(W.InC[vol-1]-W.InC[vol+1])^2
        
        W.beta[3]=W.b0*(W.InC[vol-2]-2.0*W.InC[vol-1]+W.InC[vol])^2+ 
	W.b1*(W.InC[vol-2]-4.*W.InC[vol-1]+3*W.InC[vol])^2

        
        sleft=0.0
        sright=0.0
        @simd for r=1:3
            s=1.0/(W.epsilon+W.beta[r])^2
            W.alphaleft[r]=W.dleft[r]*s
            W.alpharight[r]=W.dright[r]*s
            sleft+=W.alphaleft[r]
            sright+=W.alpharight[r]
        end
        
        recleft=dot(W.alphaleft,W.left)
        recright=dot(W.alpharight,W.right)
        W.reconstructed[2*vol-1]  = recleft/sleft
        W.reconstructed[2*vol]= recright/sright
    end
    #W.reconstructed[1:4]=W.reconstructed[2*size+1:2*size+4]
    W.reconstructed[2*size+5:2*size+8]=W.reconstructed[5:8]
    #Numerical flux at boundaries of volumes:
    @simd for vol in 1:size
        W.numflux[vol+1]=F(W.reconstructed[2*vol+4], W.reconstructed[2*(vol+1)+3])
    end
    W.numflux[1]=W.numflux[size+1]
    # result:
    @simd for vol in 1:size
        Out[vol]=h1*(W.numflux[vol+1]-W.numflux[vol])
    end
    #@. Out[1:size] = h1*(W.numflux[2:size+1]-W.numflux[1:size])
    nothing
end

end
