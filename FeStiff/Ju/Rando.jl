module Rando
export RandoData,fv
#=
Coefficients for a simple, but reproductible random number generator.
=#
mutable struct RandoData
    seed::Int64
    a::Int64
    c::Int64
    m::Int64
    RandoData()=new(123456789,1103515245,12345,4294967296)
end
#=
return a random float in [0,vmax]
=#
function fv!(R::RandoData,vmax=1.)
    R.seed= (R.a * R.seed + R.c) % R.m
    vmax*Float64(R.seed)/R.m
end
end
