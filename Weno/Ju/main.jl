push!(LOAD_PATH, "./")
using Weno
using Burghers
using Convection
using Godunov
using RK3TVD
function Init!(X,L)
    size=length(X)
    h=L/size
    for i=0:size-1
        if i>floor(size//8) && i<floor(size//2)+floor(size//8)
            X[i+1]=1.-2*(i-floor(size/8))*h/L;
        else
            X[i+1]=0.0
        end
    end
end

size=400
L=1.
T=3.
dt=0.8/size
println("dt= ",dt, " nteps= ",floor(T/dt))
In=Array{Float64}(size)
Init!(In,L)
Out=Array{Float64}(size)
println("start computation")

S(X,Y)=weno!(Godunov,Burghers,L,X,Y)

f=open("gp0","w")
writedlm(f, In)
close(f)

t=0.
while t<T
    X=Rk3tvd(S,dt,In)
    X,In=In,X
    t+=dt
end

f=open("gp","w")
writedlm(f, In)
close(f)
