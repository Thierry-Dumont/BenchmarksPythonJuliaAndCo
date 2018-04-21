module RK3TVD
export Rk3tvd,RK3TVDData
struct RK3TVDData
    c21
    c22
    c31
    c32
RK3TVDData()=new(3./4.,1./4,1./3.,2./3.)
end
function Rk3tvd(W,dt,In)
    size=length(In)
    R=RK3TVDData()
    u1=Array{Float64}(size)
    u2=Array{Float64}(size)

    W(In,u1)
    u1=In + dt*u1

    W(u1,u2)
    u2= R.c21*In+R.c22*(u1+dt*u2)

    W(u2,u1)
    return R.c31*In+R.c32*(u2+dt*u1)
end
end
