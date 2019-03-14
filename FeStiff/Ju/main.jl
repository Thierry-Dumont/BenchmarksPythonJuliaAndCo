push!(LOAD_PATH, "./")

using Rando
using Stiffness
using BenchmarkTools


function prSubDiag(m)
    
    for i=0:5
        for j=0:i
            print(m[div(i*(i+1), 2) + j+1], " ")
        end
        
        println()
    end
end

function RandomTriangle!(R::RandoData, x::Array{Float64, 1}, y::Array{Float64, 1})
    for i in 1:3
        x[i] = Rando.fv!(R, 10.)
    end
    
    for i in 1:3
        y[i] = Rando.fv!(R, 10.)
    end
end


const S = StiffnessData()
const x = Float64[0., 1., 0.]
const y = Float64[0., 0., 1.]
const m = zeros(21)
const R = RandoData()
const ntri = 1_000_000


print("\nVerify that, on the reference element, we are coherent with sage ")
println("(see ../sage/):\n")
Stiffness.op!(S, x, y, m)
prSubDiag(m)

print("\nWe must get the same result if we dilate the triangle:\n")
x[:] *= 2.0
y[:] *= 2.0
Stiffness.op!(S, x, y, m)
prSubDiag(m)


println("\nNow, start the benchmark:")
println(ntri, " triangles.")

io = IOContext(stdout, :compact => false)
bench = @benchmarkable Stiffness.op!($S, $x, $y, $m) setup = (RandomTriangle!($R, $x, $y)) samples = ntri
bench_res = run(bench)

show(io, bench_res)

println("\n")
timeByTr = BenchmarkTools.median(bench_res).time * 1e-9

open("RunningOn" * gethostname(), "w") do f
    write(f, string(timeByTr), "\n")
end

println("end.")

# to profile the code, uncomment:
# ntri1 = 100000
# @profile for t=1:ntri1
#     RandomTriangle!(R, x, y)
#     Stiffness.op!(S, x, y, m)
# end
# Profile.print(format=:flat)
