module LaxFriedrichs
export NumFlux
function NumFlux(F,a::Float64,b::Float64,alpha::Float64)
    0.5*(F.flux(a)+F.flux(b) - alpha*(b-a))
end
end
