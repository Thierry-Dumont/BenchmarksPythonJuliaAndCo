module Weno
export weno!,WenoData
#===================================================
Weno 3 (5) method in 1d.
===================================================#
struct WenoData
    c
    dright
    dleft
    b0::Float64
    b1::Float64
    epsilon::Float64
WenoData()=new(Array([
    [11./6.,-7./6.,1./3.],
    [1./3.,5./6.,-1./6.],
    [-1./6.,5./6.,1./3.],
    [1./3.,-7/6.,11./6.]]),
                Array([3./10.,3./5.,1./10.]),
                Array([1./10.,3./5.,3.10]),
                13./12.,
                1./4.,
                    1.e-6,
 
                )

end

function weno!(NumFlux,flux,L,In::Array{Float64},Out::Array{Float64})
    #
    F(x,y)=  NumFlux.NumFlux(flux,x,y)
    #
    W=WenoData()
    size=length(In)
    size1=size+1
    size4=size+4
    h1= -1./(L/size)
    
    # build an extended array with phantom cells to deal with periodicity:
    InC=Array{Float64}(size4)
    InC[1]=In[size-1]
    InC[2]=In[size]
    InC[3:2+size]=In
    InC[size+3]=In[1]
    InC[size+4]=In[2]
    # usefull arrays:
    right=Array{Float64}(3); left=Array{Float64}(3)
    beta=Array{Float64}(3)
    reconstructed=Array{Float64}(2*size4)
    numflux=Array{Float64}(size1)
    alpharight=Array{Float64}(3)
    alphaleft=Array{Float64}(3)
    # lets's start computation: 
    for vol= 3:2+size
        for r= 0:2
            left[r+1] =dot(W.c[r+1],InC[vol-r:vol-r+2])
            right[r+1]=dot(W.c[r+2],InC[vol-r:vol-r+2])
            #left[r+1] = sum(W.c[r+1].*InC[vol-r:vol-r+2])
            #right[r+1]= sum(W.c[r+2].*InC[vol-r:vol-r+2])
        end
        # regularity coefficients
        beta[1]=W.b0* (InC[vol]-2.0*InC[vol+1]+InC[vol+2])^2+ 
	W.b1*(3.*InC[vol]-4.*InC[vol+1]+InC[vol+2])^2
        
        beta[2]=W.b0*(InC[vol-1]-2.0*InC[vol]+InC[vol+1])^2+ 
        W.b1*(InC[vol-1]-InC[vol+1])^2
        
        beta[3]=W.b0*(InC[vol-2]-2.0*InC[vol-1]+InC[vol])^2+ 
	W.b1*(InC[vol-2]-4.*InC[vol-1]+3*InC[vol])^2

        #alpharight=Array{Float64}([W.dright[r]/(W.epsilon+beta[r])^2 for r=1:3])
        #alphaleft=Array{Float64}([W.dleft[r]/(W.epsilon+beta[r])^2 for r=1:3])
        sleft=0.0
        sright=0.0
        @simd for r=1:3
            alphaleft[r]=W.dleft[r]/(W.epsilon+beta[r])^2
            alpharight[r]=W.dright[r]/(W.epsilon+beta[r])^2
            sleft+=alphaleft[r]
            sright+=alpharight[r]
        end

        #sright=sum(alpharight)
        #sleft=sum(alphaleft)
        
        recleft=dot(alphaleft,left)
        recright=dot(alpharight,right)
        reconstructed[2*vol-1]  = recleft/sleft
        reconstructed[2*vol]= recright/sright
    end
    reconstructed[1:4]=reconstructed[2*size+1:2*size+4]
    reconstructed[2*size+5:2*size+8]=reconstructed[5:8]
    
    #Numerical flux at boundaries:
    @simd for vol in 1:size
        numflux[vol+1]=F(reconstructed[2*vol+4], reconstructed[2*(vol+1)+3])
    end
    numflux[1]=numflux[size+1]

    # result:
    @simd for vol in 1:size
        Out[vol]=h1*(numflux[vol+1]-numflux[vol])
    end
end

end
