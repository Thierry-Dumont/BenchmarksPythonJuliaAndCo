module RK3TVD
export Rk3tvd!,RK3TVDData
# RK explicit method, TVD.
struct RK3TVDData
    size::Int64
    c21::Float64
    c22::Float64
    c31::Float64
    c32::Float64
    u1::Array{Float64,1}
    u2::Array{Float64,1}
RK3TVDData(Size)=new(Size,3./4.,1./4,1./3.,2./3.,zeros(Size),zeros(Size))
end
function Rk3tvd!(R::RK3TVDData,W,dt,In::Array{Float64},Out::Array{Float64})
    size=R.size
    
    W(In,R.u1)
    #R.u1=In + dt*R.u1
    @simd for i=1:size
        R.u1[i]=In[i] + dt*R.u1[i]
    end
    
    W(R.u1,R.u2)
    #R.u2= R.c21*In+R.c22*(R.u1+dt*R.u2)
    @simd for i=1:size
        R.u2[i]= R.c21*In[i]+R.c22*(R.u1[i]+dt*R.u2[i])
    end
    
    W(R.u2,R.u1)
    @simd for i=1:size
        Out[i]=R.c31*In[i]+R.c32*(R.u2[i]+dt*R.u1[i])
    end
    #Out= @. R.c31*In+R.c32*(R.u2+dt*R.u1)
    nothing
end
end
