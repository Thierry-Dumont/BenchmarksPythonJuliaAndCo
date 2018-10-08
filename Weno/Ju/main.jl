push!(LOAD_PATH, "./")
using Weno
using Burghers
using Convection
using RK3TVD
using DelimitedFiles
function Init!(X,L)
    size=length(X)
    h=L/size
    for i=0:size-1
        if i>floor(size//8) && i<floor(size//2)+floor(size//8)
            X[i+1]=1.0-2*(i-floor(size/8))*h/L;
        else
            X[i+1]=0.0
        end
    end
end

@inline NumfluxGodunov(Fminf,Fmaxf,a::Float64,b::Float64)=
                     ifelse(a<=b,Fminf(a,b),Fmaxf(b,a))
@inline NumfluxLaxFriedrichs(F,a::Float64,b::Float64,alpha::Float64)=
                      0.5*(F(a)+F(b) - alpha*(b-a))



const size=1000
const L=1.0
const T=1.0
const dt=0.8/size
println("size= ",size," dt= ",dt, " nteps= ",floor(T/dt))

In=Array{Float64}(undef,size)
Init!(In,L)
Out=Array{Float64}(undef,size)


W=WenoData(size)
R=RK3TVDData(size)

# # Choose equation:
# #const EQ=Convection
const EQ=Burghers

# # Choose numerical flux:
  const NF=NumfluxGodunov
# const NF=NumfluxLaxFriedrichs

if NF==NumfluxGodunov
    @inline NumFlux(X::Float64,Y::Float64)=NumfluxGodunov(EQ.minf,EQ.maxf,X,Y)
else
    @inline NumFlux(X::Float64,Y::Float64)=NumfluxLaxFriedrichs(EQ.flux,X,Y,1.0)
end

S(X::Array{Float64,1},Y::Array{Float64,1})=weno!(W,NumFlux,L,X,Y)

# # be sure to run once before actually running the benchmark!
Init!(In,L)
Rk3tvd!(R,S,dt,In,Out)
Out,In=In,Out
# #
Init!(In,L)

f=open("gp0","w")
writedlm(f, In)
close(f)

println("start computation")
t=0.0
t1 = time_ns()

#@profile
while t<T
    Rk3tvd!(R,S,dt,In,Out)
    global Out,In=In,Out
    global t+=dt
end
#Profile.print(format=:flat)
tcomp = (time_ns() - t1)*1.0e-9 

println("computing time: ",tcomp," seconds.")

f=open("gp","w")
writedlm(f, In)
close(f)
println("A file 'gp' with the final solution was created.")
open("RunningOn"*gethostname(),"w") do f
    write(f,string(tcomp),"\n")
end
