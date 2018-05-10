module Stiffness
export StiffnessData

mutable struct StiffnessData
ji::Array{Float64,1}
grads::Array{Float64,3}
gq::Array{Float64,3}#grads of 6 basis functions at 3 quadrature points.
StiffnessData()=
    new(zeros(4),zeros(2,3,6),
        reshape([-1, -1, 1, 1, -1, -1, 1, 0, 1, 0, -1,0, 0, -1,
                          0, 1, 0, 1, 0, -2, -2, -2, 2, 0, 0, 2, 2, 2, 2,
                          0, 0, 2, -2, -2, -2, 0],2,3,6)
        )
end

function op!(S::StiffnessData,x::Array{Float64,1},y::Array{Float64,1},
            m::Array{Float64,1})
    S.ji[1]=-y[1] + y[3]
    S.ji[2]= y[1] - y[2]
    S.ji[3]= x[1] - x[3]
    S.ji[4]=-x[1] + x[2]
    for f=1:6
        @simd for p=1:3
            S.grads[1,p,f]=S.ji[1]*S.gq[1,p,f]+S.ji[2]*S.gq[2,p,f]
            S.grads[2,p,f]=S.ji[3]*S.gq[1,p,f]+S.ji[4]*S.gq[2,p,f]
        end
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
            ii+=1
         end
    end
    m
end
end
