module Godunov
export NumFlux
# Godunov flux:
function NumFlux(F,a::Float64,b::Float64)
    a<=b ? F.minf(a,b):  F.maxf(b,a)
    #ifelse(a<=b,F.minf(a,b),F.maxf(b,a))
end
end
