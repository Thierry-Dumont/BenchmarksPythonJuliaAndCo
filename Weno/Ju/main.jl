push!(LOAD_PATH, "./")
using Weno
using Burghers
using Convection
using Godunov
using RK3TVD
function Init!(X,L)
    size=length(X)
    h=L/size
    for i=1:size
        if i>size/8 && i<size/2+size/8
            X[i]=1.-2*(i-size/8)*h/L;
        else
            X[i]=0.0
        end
    end
end

size=100
L=1.
dt=0.8/size
In=Array{Float64}(size)
Init!(In,L)
Out=Array{Float64}(size)
println("start computation")
#X=weno!(Godunov,Burghers,L,In,Out)
S(X,Y)=weno!(Godunov,Burghers,L,X,Y)
Z=S(In,Out)

Init!(In,L)
println("In:",In)
X=Rk3tvd(S,dt,In)
println("X ",X)
println(length(X))
