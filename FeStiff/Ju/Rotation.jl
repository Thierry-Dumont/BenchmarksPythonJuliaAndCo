module Rotation
export  Rotation_data,Rotate_point,Rotate_triangle!
mutable struct Rotation_data
    α::Float64
    m::Array{Float64,2}
    temp::Array{Float64,1}
    Rotation_data(angle)=new(angle,
                             [cos(angle) -sin(angle);sin(angle) cos(angle)],
                             zeros(2)
                             )
    
end
function Rotate_point(R::Rotation_data,x)
    y=x
    y[1]= R.m[1,1]*x[1]+R.m[1,2]*x[2]
    y[2]= R.m[2,1]*x[1]+R.m[2,2]*x[2]
    y
end
function Rotate_triangle!(R::Rotation_data,x,y)
    for i in 1:3
        R.temp=Rotate_point(R,[x[i],y[i]])
        x[i]=R.temp[1]
        y[i]=R.temp[2]
    end
end
end
