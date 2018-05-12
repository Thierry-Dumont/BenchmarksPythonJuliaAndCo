module Godunov
export NumFlux
function NumFlux(F,a::Float64,b::Float64)
    # if a<=b
    #     return F.minf(a,b)
    # else
    #     return F.maxf(b,a)
    # end
    a<=b ? F.minf(a,b):  F.maxf(b,a)
end
end
