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



# proc[1-5]! functions make the same computation, with different programing
# styles.

function proc1!(A::Array{Float64,1},B::Array{Float64,1},
                C::Array{Float64,1},D::Array{Float64,1},niter::Int64)
    for it=1:niter
        @. A=1.7*B-0.8*C-0.9*D
        A,D = D,A
    end 
end
function proc2!(A::Array{Float64,1},B::Array{Float64,1},
                C::Array{Float64,1},D::Array{Float64,1},niter::Int64)
    for it=1:niter
        @. A[:]=1.7*B[:]-0.8*C[:]-0.9*D[:]
        A,D = D,A
    end
end
function proc3!(A::Array{Float64,1},B::Array{Float64,1},
                C::Array{Float64,1},D::Array{Float64,1},niter::Int64)
    const size=length(A)
    for it=1:niter
        @simd for i in 1:size
            A[i]=1.7*B[i]-0.8*C[i]-0.9*D[i]
        end
        A,D = D,A
    end
end
function proc4!(A::Array{Float64,1},B::Array{Float64,1},
                C::Array{Float64,1},D::Array{Float64,1},niter::Int64)
    for it=1:niter
        A[:]=1.7*B[:]
        A[:]-=0.8*C[:]
        A[:]-=0.9*D[:]
        A,D = D,A
    end
end
function proc5!(A::Array{Float64,1},B::Array{Float64,1},
                C::Array{Float64,1},D::Array{Float64,1},niter::Int64)
    for it=1:niter
        @. A=1.7*B
        @. A-=0.8*C
        @. A-=0.9*D
        A,D = D,A
    end
    
end

# measure as precisely as possible the execution time for a given
# p= proc[1-5]!
function test(p,A::Array{Float64,1},B::Array{Float64,1},
              C::Array{Float64,1},D::Array{Float64,1},nit::Int)
   
    niter=nit
    # be sure to run once before actually running the benchmark!
    Init!(A,1.)
    Init!(B,1.)
    Init!(C,1.)
    Init!(D,1.)
    p(A,B,C,D,niter)
    #
    T=0.
    while true
        Init!(A,1.)
        Init!(B,1.)
        Init!(C,1.)
        Init!(D,1.)
        t1 = time_ns()
        p(A,B,C,D,niter)
        t = (time_ns() -t1)/niter
        if abs(t-T)/t<0.05
            break
    else
            T=t
            if niter> 1500
                error("too many iterations",niter)
            else
                niter*=2
            end
    end
    
end
    T,niter
end
DD=Dict("proc1!"=>"Naïve vectorisation               ",
        "proc2!"=>"Vectorisation with braces         ",
        "proc3!"=>"Unrolled loop                     ",
        "proc4!"=>"Vectorisation with braces by steps",
        "proc5!"=>"Vectorisation with @. by steps    "
        )
# computation starts here:
size=32
sizemax=10^6
const niter=2
fw=open("RunningOn"*gethostname()*"_cl","w")
while size<sizemax
    println("size: ",size,":\n")
    A=Array{Float64}(size)
    B=Array{Float64}(size)
    C=Array{Float64}(size)
    D=Array{Float64}(size)
    tbest=10.^29
    best=0
    t=0.0
    for p= [proc1!,proc2!,proc3!,proc4!,proc5!]
        t,it=test(p,A,B,C,D,niter)
        if t<tbest
            tbest=t
            best=p
        end
        t*=10.0^(-9)
        println(DD[string(p)]," : t= ",t," seconds")
    end
    nflops=size*5
    flops=nflops/tbest
    println("\nbest: ",DD[string(best)])

    write(fw,string(size)," ",string(tbest*10.0^(-9)),"\n")
    
    println("nb. flops: ",nflops, ", Gflops/s (best): ",flops)
    println("-------")
    
    size*=2
    println()
    
end
close(fw)
