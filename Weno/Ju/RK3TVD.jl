module RK3TVD
export Rk3tvd!,RK3TVDData
struct RK3TVDData
    c21::Float64
    c22::Float64
    c31::Float64
    c32::Float64
RK3TVDData()=new(3./4.,1./4,1./3.,2./3.)
end
function Rk3tvd!(W,dt,In::Array{Float64},Out::Array{Float64})
    size=length(In)
    R=RK3TVDData()
    u1=Array{Float64}(size)
    u2=Array{Float64}(size)

    W(In,u1)
    #u1=In + dt*u1
    @simd for i=1:size
        u1[i]=In[i] + dt*u1[i]
    end
    
    W(u1,u2)
    #u2= R.c21*In+R.c22*(u1+dt*u2)
    @simd for i=1:size
        u2[i]= R.c21*In[i]+R.c22*(u1[i]+dt*u2[i])
    end
    
    W(u2,u1)
    @simd for i=1:size
        Out[i]=R.c31*In[i]+R.c32*(u2[i]+dt*u1[i])
    end
    #Out= @. R.c31*In+R.c32*(u2+dt*u1)
    nothing
end
end
