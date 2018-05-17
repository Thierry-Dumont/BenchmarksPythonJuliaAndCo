
# initalize an array
function Init!(X::Array{Float64,2},L)
    siz=size(X)[1]
    h=L/siz
    for j=0:siz-1
        for i=0:siz-1
            if i>floor(siz//8) && i<floor(siz//2)+floor(siz//8)
                X[i+1,j+1]=1.-2*(i-floor(siz/8))*h/L;
            else
                X[i+1,j+1]=0.0
            end
        end
    end
end



# proc[1-5]! functions make the same computation, with different programing
# styles.

function proc1!(In::Array{Float64,2},Out::Array{Float64,2},niter::Int64)
    const siz=size(In)[1]
    h2::Float64= (1./siz)^2
    for it=1:niter
        Out[2:siz-1,2:siz-1]=
        h2*(In[1:siz-2,2:siz-1 ] + In[2:siz-1,1:siz-2]-
            4.0*In[2:siz-1,2:siz-1]+
            In[3:siz,2:siz-1]+In[2:siz-1,3:siz])
        In,Out=Out,In
    end 
end
function proc2!(In::Array{Float64,2},Out::Array{Float64,2},niter::Int64)
    const siz=size(In)[1]
    h2::Float64= (1./siz)^2
    for it=1:niter
       @. Out[2:siz-1,2:siz-1]=
        h2*(In[1:siz-2,2:siz-1 ] + In[2:siz-1,1:siz-2]-
            4.0*In[2:siz-1,2:siz-1]+
            In[3:siz,2:siz-1]+In[2:siz-1,3:siz])
        In,Out=Out,In
    end 
end
function proc3!(In::Array{Float64,2},Out::Array{Float64,2},niter::Int64)
    const siz=size(In)[1]
    h2::Float64= (1./siz)^2
    for it=1:niter
        for j=2:siz-1
            for i=2:siz-1
                Out[i,j]=
                h2*(In[i-1,j] + In[i,j-1]-
                    4.0*In[i,j]+
                    In[i+1,j]+In[i,j+1])
            end
        end
        In,Out=Out,In
    end 
end
function proc4!(In::Array{Float64,2},Out::Array{Float64,2},niter::Int64)
    const siz=size(In)[1]
    h2::Float64= (1./siz)^2
    for it=1:niter
        for j=2:siz-1
            @simd for i=2:siz-1
                Out[i,j]=
                h2*(In[i-1,j] + In[i,j-1]-
                    4.0*In[i,j]+
                    In[i+1,j]+In[i,j+1])
            end
        end
        In,Out=Out,In
    end 
end
function proc5!(In::Array{Float64,2},Out::Array{Float64,2},niter::Int64)
    const siz=size(In)[1]
    h2::Float64= (1./siz)^2
    for it=1:niter
        for i=2:siz-1
            @simd for j=2:siz-1
                Out[i,j]=
                h2*(In[i-1,j] + In[i,j-1]-
                    4.0*In[i,j]+
                    In[i+1,j]+In[i,j+1])
            end
        end
        In,Out=Out,In
    end 
end
# measure as precisely as possible the execution time for a given
# p= proc[1-5]!
function test(p,In::Array{Float64,2},Out::Array{Float64,2},nit::Int)
   
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
DD=Dict("proc1!"=>"Vectorisation with braces             ",
        "proc2!"=>"Vectorisation with @. and braces      ",
        "proc3!"=>"Unrolled loop (good order)            ",
        "proc4!"=>"Unrolled loop with @simd (good order) ",
        "proc5!"=>"Unrolled loop with @simd (bad order)  "
        )
# computation starts here:
siz=8
sizemax=1025
const niter=2
while siz<sizemax
    println("siz: ",siz)
    In=Array{Float64,2}(siz,siz)
    Out=Array{Float64,2}(siz,siz)
    tbest=10.^29
    best=0
    t=0.0
    for p= [proc1!,proc2!,proc3!,proc4!,proc5!]
        t,it=test(p,In,Out,niter)
        if t<tbest
            tbest=t
            best=p
        end
        t*=10.0^(-9)
        println(DD[string(p)]," : t= ",t," seconds ")
    end

    nflops=6*(siz-2)^2
    flops=nflops/tbest
    println("\nbest: ",DD[string(best)])
    println("nb. flops (best): ",nflops, ", Gflops/s: ",flops)
    println("-------")
    
    siz*=2
    println()
end
