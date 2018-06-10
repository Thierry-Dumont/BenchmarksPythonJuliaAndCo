# 
# knowing the bandwidth of the machine, and the performances of the code,
# estimate the arithmetic intensity of the code.
#
bdwm=50000. #MB/s, results from stream benchmark.
maxp=120. # maxperf Gflops mesured.
#----------------------------------------------
bd=bdwm/1024/8  #same, in giga 8 bytes words.
print "\nbandwidth in Giga-double/s:",bd
print "max. perf. in Gflops:",maxp
ia=maxp/bd
print "arithmetic intensity:\n (flops/double): ",ia,"\n ", "(flops/byte):",ia/8
