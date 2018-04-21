module Burghers
export flux,minf,maxf
function flux(x)
    0.5*x*x
end
function minf(a,b)
    if b<=0.0
        return flux(b)
    elseif a>=0.0
        return flux(a)
    else
        return 0.0
    end
end
function maxf(a,b)
    max(flux(a),flux(b))
end
end
