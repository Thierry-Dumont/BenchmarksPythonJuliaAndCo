module Convection
export flux,minf,maxf
function flux(x::Float64)
    x
end
function minf(a::Float64,b::Float64)
    if b<=0.0
        return flux(b)
    elseif a>=0.0
        return flux(a)
    else
        return 0.0
    end
end
function maxf(a::Float64,b::Float64)
    max(flux(a),flux(b))
end
end
