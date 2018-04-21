module Godunov
export NumFlux
function NumFlux(F,a,b)
    if a<=b
        return F.minf(a,b)
    else
        return F.maxf(b,a)
    end
end
end
