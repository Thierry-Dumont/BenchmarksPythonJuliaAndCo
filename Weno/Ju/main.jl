push!(LOAD_PATH, "./")
using Weno
using Burghers
using Convection
using Godunov
using LaxFriedrichs
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
@inline function NumFluxL(a::Float64,b::Float64)
    0.5*(0.5*a^2+0.5*b^2-0.5*(b-a))
end
const size=1000
const L=1.
const T=1.0
const dt=0.8/size
println("size= ",size," dt= ",dt, " nteps= ",floor(T/dt))
In=Array{Float64}(size)
Init!(In,L)
Out=Array{Float64}(size)
println("start computation")

W=WenoData(size)
R=RK3TVDData(size)

#NumFlux(X,Y)=Godunov.NumFlux(Convection,X,Y)
#NumFlux(X::Float64,Y::Float64)=LaxFriedrichs.NumFlux(Convection,X,Y,1.)
#@inline NumFlux(X,Y)=Godunov.NumFlux(Burghers,X,Y)
@inline NumFlux(X,Y)=LaxFriedrichs.NumFlux(Burghers,X,Y,1.0)
#S(X::Array{Float64,1},Y::Array{Float64,1})=weno!(W,NumFlux,L,X,Y)
S(X::Array{Float64,1},Y::Array{Float64,1})=weno!(W,NumFluxL,L,X,Y)
f=open("gp0","w")
writedlm(f, In)
close(f)

t=0.
t1 = time_ns()
#Profile.clear_malloc_data()
#@profile 
while t<T
    Rk3tvd!(R,S,dt,In,Out)
    Out,In=In,Out
    t+=dt
end
#Profile.print(format=:flat)
tcomp = (time_ns() - t1)*1.e-9 

println("computing time: ",tcomp," seconds.")

f=open("gp","w")
writedlm(f, In)
close(f)
