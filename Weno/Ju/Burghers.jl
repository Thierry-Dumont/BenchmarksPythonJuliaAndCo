module Burghers
export flux,minf,maxf
function flux(x::Float64)
    0.5*x^2
end
function minf(a::Float64,b::Float64)
    if b<=0.0
        return 0.5*b^2
        #return flux(b)
    elseif a>=0.0
        return 0.5*a^2
        #return flux(a)
    else
        return 0.0
    end
end
function maxf(a::Float64,b::Float64)
    #max(flux(a),flux(b))
    0.5*max(a^2,b^2)
end
end
