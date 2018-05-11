push!(LOAD_PATH, "./")
using Rando
using Stiffness
function prSubDiag(m)
    
    for i = 0:5
        for j=0:i
            print(m[div(i*(i+1),2)+j+1]," ")
        end
        println()
    end
end
function RandomTriangle!(R::RandoData,x::Array{Float64,1},y::Array{Float64,1})
    for i in 1:3
        x[i]=Rando.fv!(R,10.)
    end
    for i in 1:3
        y[i]=Rando.fv!(R,10.)
    end
end
S=StiffnessData()
x=Float64[0.,1.,0.]
y=Float64[0.,0.,1.]
m=zeros(21)
print("\nVerify that, on the reference element, we are coherent with sage ")
println("(see ../sage/):\n")
Stiffness.op!(S,x,y,m)
prSubDiag(m)
print("\nWe must get the same result if we dilate the triangle:\n")
x*=2.0
y*=2.0
Stiffness.op!(S,x,y,m)
prSubDiag(m)
#Profile.print()

println("\nNow, start the benchmark:")
const ntri=1000000
println(ntri," triangles.")
R=RandoData()
t1 = time_ns()
for t=1:ntri
    RandomTriangle!(R,x,y)
    Stiffness.op!(S,x,y,m)
end
t1 = time_ns()-t1
println("first phase: ",float(t1)*10.0^(-9)," seconds.")

t11 = time_ns()
for t=1:ntri
    RandomTriangle!(R,x,y)
end
t11 = time_ns()-t11
println("second phase: ",float(t11)*10.0^(-9)," seconds.")
T=t1-t11
Tsecond=float(T)*10.0^(-9)
println("Total time: ",Tsecond)
byTr=Tsecond/ntri
println("Time by triangle: ",byTr," seconds.")
println("end.")

# ntri1=100000
# @profile for t=1:ntri1
#     RandomTriangle!(R,x,y)
#     Stiffness.op!(S,x,y,m)
#  end
#  Profile.print(format=:flat)
