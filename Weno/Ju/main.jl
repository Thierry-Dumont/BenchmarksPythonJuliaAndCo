push!(LOAD_PATH, "./")
using Weno
using Burghers
#using Convection
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

const size=400
const L=1.
const T=0.5
const dt=0.8/size
println("dt= ",dt, " nteps= ",floor(T/dt))
In=Array{Float64}(size)
Init!(In,L)
Out=Array{Float64}(size)
println("start computation")

S(X,Y)=weno!(Godunov,Burghers,L,X,Y)

#S(In,Out)
#Rk3tvd!(S,dt,In,Out)
#@time Rk3tvd!(S,dt,In,Out)
#@profile S(In,Out)
#@profile (for i = 1:100; S(In,Out);In,Out=Out,In; end)
#Profile.print(format=:flat)
f=open("gp0","w")
writedlm(f, In)
close(f)
set_zero_subnormals(true)
t=0.
t1 = time_ns()

#@profile
while t<T
    Rk3tvd!(S,dt,In,Out)
    Out,In=In,Out
    t+=dt
end
#Profile.print(format=:flat)
tcomp = (time_ns() - t1)*1.e-9 

println("computing time: ",tcomp," seconds.")

f=open("gp","w")
writedlm(f, In)
close(f)
