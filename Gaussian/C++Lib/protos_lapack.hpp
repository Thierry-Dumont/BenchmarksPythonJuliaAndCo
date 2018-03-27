#ifndef protos_lapack__h
#define  protos_lapack__h
namespace odes
{
  //! prototypes for lapack routines.
extern "C"{
  void dgetrf_(int *n,int *m,double* a,int *lda,int *ipiv,int *info);
}
extern "C"{
  void zgetrf_(int *n,int *m,double* a,int *lda,int *ipiv,int *info);
}
extern "C"{
  void dgetrs_(const char *s,int *N,int *NRHS,double *A,int *LDA,int *IPIV,
	       double *B,int *LDB,int *INFO );

}
extern "C"{
  void zgetrs_(const char *s,int *N,int *NRHS,double *A,int *LDA,int *IPIV,
	       double *B,int *LDB,int *INFO );
}
extern "C"{
  void dgbtrf_(int *n,int *m,int *k1,int *k2,
	       double* a,int *lda,int *ipiv,int *info);
}
extern "C"{
  void zgbtrf_(int *n,int *m,int *k1,int *k2,
	       double* a,int *lda,int *ipiv,int *info);
}
extern "C"{
  void dgbtrs_(const char *s,int *N,int *k1,int *k2,
	       int *NRHS,double *A,int *LDA,int *IPIV,
	       double *B,int *LDB,int *INFO );
}
extern "C"{
  void zgbtrs_(const char *s,int *N,int *k1,int *k2,
	       int *NRHS,double *A,int *LDA,int *IPIV,
	       double *B,int *LDB,int *INFO );
}
extern "C"{
  void dlarnv_(int *idist,int iseed[],int *n,double *x);
}
extern "C"{
  void dgehrd_(int *n,int *ilo,int *ihi,double *a,int *lda,double tau[],
	       double work[],int *lwork,int *info);
}
extern "C"{
  void dorghr_(int *n,int *ilo,int *ihi,double *a,int *lda,double tau[],
	       double work[],int *lwork,int *info);
}
  extern "C"{
    void dgeev_(const char *jobvl,const char *jobvr,int * n,double *a,
		int *lda,double *WR, double *WI,double *VL,int *LDVL, 
		double *VR,int  *LDVR,
		double *WORK,int  *LWORK,int *INFO );

  }
}
#endif
