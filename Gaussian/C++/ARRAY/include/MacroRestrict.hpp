// define the "restrict" keyword for different compilers.
// If the macro is not defined for a given compiler, a compilation error
// will occur at compile time.
#ifdef GCC
// gcc:
#define Restrict __restrict__
#endif
#ifdef ICC
// Intel compiler:
#define Restrict restrict
#endif
#ifdef CLANG
// Clang compiler.
#define Restrict restrict
#endif
