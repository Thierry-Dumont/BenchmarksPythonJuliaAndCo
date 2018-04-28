
# initalize an array
function Init!(X,L)
    size=length(X)
    h=L/size
    for i=0:size-1
        if i>floor(size//8) && i<floor(size//2)+floor(size//8)
            X[i+1]=1.-2*(i-floor(size/8))*h/L;
        else
            X[i+1]=0.0
        end
    end
end



# proc[1-3]! functions make the same computation, with different programing
# styles.

function proc1!(X::Array{Float64,1},Y::Array{Float64,1},niter::Int)
 
    s=0.0
    for it=1:niter
        s=dot(X,Y)
        X,Y=Y,X
    end
    s
end
function proc2!(X::Array{Float64,1},Y::Array{Float64,1},niter::Int)
    const size=length(X)
    s=0.0
    for it=1:niter
        s=0.0
        @simd for i= 1:size
            s+=X[i]*Y[i]
        end
        X,Y=Y,X
    end
    s
end
function proc3!(X::Array{Float64,1},Y::Array{Float64,1},niter::Int)
    const size=length(X)
    s=0.0
    for it=1:niter
        s= sum(X.*Y)
        X,Y=Y,X
    end
    s
end
# measure as precisely as possible the execution time for a given
# p= proc[1-5]!
function test(p,X::Array{Float64,1},Y::Array{Float64,1},nit::Int)
   
    niter=nit
    Init!(X,1.)
    Init!(Y,1.)
 
    p(X,Y,niter)
    T=0.
    while true
        Init!(X,1.)
        Init!(Y,1.)
        t1 = time_ns()
        p(X,Y,niter)
        t = (time_ns() -t1)/niter
        if abs(t-T)/t<0.025
            break
    else
            T=t
            if niter> 150000
                error("too many iterations",niter)
            else
                niter*=2
            end
    end
    
end
    T,niter
end

# computation starts here:
size=16
sizemax=10^5
const niter=2
while size<sizemax
    println("size: ",size)
    X=Array{Float64}(size)
    Y=Array{Float64}(size)
    tbest=10.^29
    best=0
    t=0.0
    for p= [proc1!,proc2!,proc3!]
        t,it=test(p,X,Y,niter)
        if t<tbest
            tbest=t
            best=p
        end
        println(p," : t= ",t," nano-seconds ")
    end

    nflops=size*4
    flops=10^9*nflops/tbest
    println("\nbest: ",best)
    println("nb. flops (best): ",nflops, ", Gflops/s: ",flops/10^9)
    println("-------")
    
    size*=2
    println()
end
