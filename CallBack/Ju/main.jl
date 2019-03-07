using BenchmarkTools
using InteractiveUtils

function trapz(a::Float64,b::Float64,n::Int64,f::Function)
    h::Float64=(b-a)/n
    sum=0.5*(f(a)+f(b))
    for i=1:n-1
        sum+=f(i*h)
    end
    sum*=h
end
               
@inline f(x::Float64)=+exp(-x)*x^2

function g(x::Float64)
    h=0.0
    if x<0.5
        h=-exp(-x)
    else
        h= exp(x)
    end
    return h*x^2
end

# Note: replacing f by f1 and g by g1 do not change the computing time:
# f1(x)=x < 1.5 ? +exp(-x)*x^2 :  +exp(-x)*x^2
# g1(x::Float64)=x < 0.5 ? -exp(-x)*x^2 :  exp(x)*x^2

function implicit(t::Float64)
    # implicit = root of  4*sin(x)-exp(x)+t
    # Newton iterations, starting from zero:
    x= 0.0
    F= 4*sin(x)-exp(x)+t
    while abs(F)> 1.e-15
        #
        x-= F/(4*cos(x) - exp(x))
        F=  4*sin(x)-exp(x)+t
        #
    end
    x
end

io = IOContext(stdout, :compact => false)

fw=open("RunningOn"*gethostname(),"w")
# Note: it seems that a loop like:
#
# for F in [f,g,implicit]
#   bench_res = @benchmark trapz(0.,1.,1000,F)
# end
#
# is incompatible with @benchmark... who knows why?

f(0.5)
println("f:")
bench_res = @benchmark trapz(0.,1.,1000,f);
show(io, bench_res)
write(fw,"\nf: "*string(bench_res)*"\n")

g(0.5)
println("\ng:")
bench_res = @benchmark trapz(0.,1.,1000,g);
show(io, bench_res)
write(fw,"g: "*string(bench_res)*"\n")

implicit(0.5)
println("\nimplicit:")
bench_res = @benchmark trapz(0.,1.,1000,implicit)
show(io, bench_res)
write(fw,"implicit: "*string(bench_res)*"\n")

println("\nend.")
close(fw)
