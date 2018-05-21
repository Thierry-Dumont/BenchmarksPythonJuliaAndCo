
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
function proc1!(In::Array{Float64,1},Out::Array{Float64,1},niter::Int64)
    const size=length(In)
    h2::Float64= (1./size)^2
    for it=1:niter
        Out[2:size-1]= h2*(In[1:size-2]- 2.0*In[2:size-1]+ In[3:size])
        In,Out=Out,In
    end 
end
function proc1!(In::Array{Float64,1},Out::Array{Float64,1},niter::Int64)
    const size=length(In)
    h2::Float64= (1./size)^2
    for it=1:niter
        Out[2:size-1]= h2*(In[1:size-2]- 2.0*In[2:size-1]+ In[3:size])
        In,Out=Out,In
    end 
end
function proc2!(In::Array{Float64,1},Out::Array{Float64,1},niter::Int64)
    const size=length(In)
    h2::Float64= (1./size)^2
    for it=1:niter
        @.  Out[2:size-1]= h2*(In[1:size-2]- 2.0*In[2:size-1]+ In[3:size])
        In,Out=Out,In
    end 
end
function proc3!(In::Array{Float64,1},Out::Array{Float64,1},niter::Int64)
    const size=length(In)
    h2::Float64= (1./size)^2
    for it=1:niter
        @simd for i=2:size-1
            Out[i]= h2*(In[i-1]- 2.0*In[i]+ In[i+1])
        end
        In,Out=Out,In
    end 
end
# measure as precisely as possible the execution time for a given
# p= proc[1-5]!
function test(p,In::Array{Float64,1},Out::Array{Float64,1},nit::Int)
   
    niter=nit
    Init!(In,1.)
    Init!(Out,1.)
 
    p(In,Out,niter)
    T=0.
    while true
        Init!(In,1.)
        Init!(Out,1.)
        t1 = time_ns()
        p(In,Out,niter)
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
DD=Dict("proc1!"=>"Vectorisation with braces         ",
        "proc2!"=>"Vectorisation with @. and braces  ",
        "proc3!"=>"Unrolled loop                     "
        )

fw=open("RunningOn"*gethostname()*"_lapl_1","w")

# computation starts here:
size=32
sizemax=10^5
const niter=2
while size<sizemax
    println("size: ",size)
    In=Array{Float64}(size)
    Out=Array{Float64}(size)
    tbest=10.^29
    best=0
    t=0.0
    for p= [proc1!,proc2!,proc3!]
        t,it=test(p,In,Out,niter)
        if t<tbest
            tbest=t
            best=p
        end
        t*=10.0^(-9)
        println(DD[string(p)]," : t= ",t," seconds ")
    end

    nflops=size*4
    flops=nflops/tbest
    println("\nbest: ",DD[string(best)])

    write(fw,string(size)," ",string(tbest*10.0^(-9)),"\n")
    
    println("nb. flops: ",nflops, ", Gflops/s (best): ",flops)
    println("-------")
    
    size*=2
    println()
end
close(fw)
