x,y=var("x,y")
# the barycentric coordinates on the reference triangle:
lamb=[1-x-y, x,y]
# P2 basis functions, on the reference triangle:
phi=[2*l*(l-1/2) for l in lamb]+[4*lamb[i]*lamb[(i+1)%3] for i in range(0,3)]
#
# verify:
#
print("verify that basis functions value is zero or one where it should be:")
points=[[0,0],[1,0],[0,1],[1/2,0],[1/2,1/2],[0,1/2]]
A=matrix(6,6)
for i in range(0,6):
    f=phi[i]
    for j in range(0,6):
        A[i,j]=f(x=points[j][0],y=points[j][1])
print(A==identity_matrix(6,6))
#
# gradients of  basis functions, on the reference triangle:
#
grads=[[diff(f,x),diff(f,y)] for f in phi]
#
# gradients at quadrature points (midpoints of the edges),
# on the reference triangle:
#
# gq[i][j]= gradient of ith function at jth quadrature point.
#
quad=[[1/2,0],[1/2,1/2],[0,1/2]]
gq=[[(f[0](x=q[0],y=q[1]),f[1](x=q[0],y=q[1])) for q in quad]  for f in grads]  
#
# transformation matrix from reference to current triangle:
#
x0,x1,x2,y0,y1,y2=var("x0,x1,x2,y0,y1,y2")# current triangle
J=matrix([[x1-x0,x2-x0],[y1-y0,y2-y0]])
det=J.determinant().simplify_rational()
Jinv=J.inverse().simplify_rational()
JinvDet=Jinv*det
# transpose of J^-1 multiplied by determinant:
JinvDetTrans=JinvDet.transpose()
print("verify that 'everything' is ok on the reference triangle:")
print(det(x0=0,x1=1,y0=0,y1=0,x2=0,y2=1)==1)
print(JinvDetTrans(x0=0,x1=1,y0=0,y1=0,x2=0,y2=1)==identity_matrix(2,2))

#
# integration on reference triangle:
integreftr=lambda f: f.integral(x,0,1-y).integral(y,0,1)

# stiffness matrix on reference triangle:
M=matrix(QQ,6,6)
for i in range(0,6):
    for j in range(0,6):
        gigj= grads[i][0]*grads[j][0]+grads[i][1]*grads[j][1]
        M[i,j]=integreftr(gigj)
        
# stiffness matrix on current triangle, numerically computed:
MN=matrix(SR,6,6)
Ji=JinvDetTrans
for i in range(0,6):
    for j in range(0,6):
        s=0
        for k in range(0,3):
            V=Ji*vector(gq[i][k])
            W=Ji*vector(gq[j][k])
            s+=V.dot_product(W)
        MN[i,j]=s
MN*=(1/(6*det))
# verify that stiffness matrix as computed by quadrature == exact computation (
# on the reference element !).
print(MN(x0=0,x1=1,y0=0,y1=0,x2=0,y2=1))
print("verify:")
print(MN(x0=0,x1=1,y0=0,y1=0,x2=0,y2=1)==M)
#verify also that if v=[1,1,1,1,1,1], M*V==0 and MN*V=0 an any triangle:
x=vector([1,1,1,1,1,1])
print( (M*x).is_zero(),(MN*x).simplify_rational().is_zero())
# Define a function that will return the stiffness matrix, for x=[], y=[]
# containing the coordinates  of the summits of a triangle (usefull for
# debugging codes!)
def Stiff(x,y):
    return MN(x0=x[0],x1=x[1],y0=y[0],y1=y[1],x2=x[2],y2=y[2])
#
# do we again compute the same matrix?
#
x=vector([0,1,0])
y=vector([0,0,1])
print(Stiff(x,y)==M)
#
# Now, the matrix should be invariant when dilating a triangle; let's
# verify this with the reference triangle:
for i in range(0,200):
    dilat= abs(QQ.random_element(100,100)) # a "random" number.
    if dilat != 0:
        x1=dilat*x
        y1=dilat*y
        assert(Stiff(x1,y1) == M)
#
# Verify that the Stiffness matrix is invariant under some rotations.
# (we must rotate by angles whose sinus and cosinus are Algebraic Numbers,
# and compute in (real) Algebraic Numbers (AA for SageMath), so as to make
# again exact computations):
tetas=[pi/12,pi/6,pi/4,pi/3,pi/2]
for teta in tetas:
    Rot=matrix(AA,[[cos(teta),-sin(teta)],[sin(teta),cos(teta)]])
    V=[Rot*vector(AA,[x[i],y[i]]) for i in range(0,3)]
    print teta,": ",Stiff([s[0] for s in V],[s[1] for s in V])==M
#
# usefull for Python programing (see Py*/ directories)
import numpy as np
gqnp=np.array(gq,dtype=np.float64).reshape(18,2)
#print(gqnp)
print("end")


