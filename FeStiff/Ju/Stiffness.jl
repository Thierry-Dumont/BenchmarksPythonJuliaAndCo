module Stiffness
export StiffnessData

mutable struct StiffnessData
grads::Array{Float64,3}
gq::Array{Float64,3}#grads of 6 basis functions at 3 quadrature points.
StiffnessData()=
    new(zeros(2,3,6),
        reshape([-1.0,-1.0,1.0,1.0,-1.0,-1.0,1.0,0.0,1.0,0.0,-1.0,0.0,0.0,
                 -1.0,0.0,1.0,0.0,1.0,0.0,-2.0,-2.0,-2.0,2.0,0.0,0.0,2.0,2.0,
                 2.0,2.0,0.0,0.0,2.0,-2.0,-2.0,-2.0,0],2,3,6)
        )
end

function op!(S::StiffnessData,x::Array{Float64,1},y::Array{Float64,1},
            m::Array{Float64,1})
    a11=-y[1] + y[3]
    a12= y[1] - y[2]
    a21= x[1] - x[3]
    a22=-x[1] + x[2]
    for f=1:6
        @simd for p=1:3
            S.grads[1,p,f]=a11*S.gq[1,p,f]+a12*S.gq[2,p,f]
            S.grads[2,p,f]=a21*S.gq[1,p,f]+a22*S.gq[2,p,f]
        end
        # S.grads[1,1:3,f]=a11*S.gq[1,1:3,f]+a12*S.gq[2,1:3,f]
        # S.grads[2,1:3,f]=a21*S.gq[1,1:3,f]+a22*S.gq[2,1:3,f]
    end
    det= -(x[2] - x[3])*y[1] + (x[1] - x[3])*y[2] - (x[1] - x[2])*y[3]
    dv=1.0/(6.0*det)
    ii=1
    for i=1:6
        for j=1:i
            s=0.0
            @simd for k=1:3
                s+=S.grads[1,k,i]*S.grads[1,k,j]+S.grads[2,k,i]*S.grads[2,k,j]
            end
            m[ii]=dv*s
            # m[ii]=dv*(dot(S.grads[1,:,i],S.grads[1,:,j])+
            #           dot(S.grads[2,:,i],S.grads[2,:,j]))
            ii+=1
        end
    end
    m
end
end
