To run the code, just type:

```
./script
```

If you do not have a machine with AVX instructions, comment out the
line
```
export NUMBA_ENABLE_AVX=1
```
in ./script .

### Note:

lambda functions in jitted routines could not be compiled.
