module m_weno

  use m_burghers
  use m_godunov
  
  implicit none
  
  private
  
  public :: initWenoData
  public :: weno


  type, public :: WenoData

     integer :: size
     
     real(kind = 8) :: b0, b1, epsilon
     
     real(kind = 8), dimension(3) :: dright, dleft
     real(kind = 8), dimension(:), allocatable :: InC
     real(kind = 8), dimension(3) :: right, left
     real(kind = 8), dimension(:), allocatable :: reconstructed, work
     real(kind = 8), dimension(3) :: alpharight, alphaleft
     real(kind = 8), dimension(3) :: beta
     
     real(kind = 8), dimension(3, 4) :: c
     
  end type WenoData

  
  contains

    subroutine initWenoData(wdata, len)

      type(WenoData), intent(inout) :: wdata
      integer :: len

      wdata%c = reshape((/ 11./6, -7./6, 1./3, 1./3, 5./6, -1./6, -1./6, 5./6, 1./3, 1./3, -7./6, 11./6 /), shape(wdata%c))
      wdata%dright = (/ 3/10., 3/5., 1/10. /)
      wdata%dleft = (/ 1/10., 3/5., 3/10. /)
      wdata%b0 = 13/12.
      wdata%b1 = 1/4.
      wdata%epsilon = 1e-6
      wdata%size = len
      
      allocate(wdata%InC(len + 4))
      allocate(wdata%reconstructed(2 * len + 8))
      allocate(wdata%work(len + 2))
      
    end subroutine initWenoData



    subroutine weno(W, L, In, Out)

      type(WenoData), intent(inout) :: W
      real(kind = 8), intent(in) :: L

      real(kind = 8), dimension(:), intent(in) :: In
      real(kind = 8), dimension(:), intent(inout) :: Out

      integer :: size
      real(kind = 8) :: h1
      real(kind = 8) :: sleft, sright, s, recleft, recright

      integer :: vol, r
      
      size = W%size
      h1 = -1. / (L / size)

      W%InC(1) = In(size -1)
      W%InC(2) = In(size)
      W%InC(3:2+size) = In
      W%InC(size+3) = In(1)
      W%InC(size+4) = In(2)

      do vol = 1, size+2
         W%work(vol) = (W%InC(vol) - 2 * W%InC(vol + 1) + W%InC(vol + 2))** 2
      end do

      do vol = 3, size+2
         do r = 0, 2
            W%left(r+1) = dot_product(W%c(:, r+1), W%InC(vol-r:vol-r+2))
            W%right(r+1) = dot_product(W%c(:, r+2), W%InC(vol-r:vol-r+2))
         end do

         W%beta(1) = W%b0 * W%work(vol) + W%b1 * (3 * W%InC(vol) - 4 * W%InC(vol+1) + W%InC(vol+2))**2
         W%beta(2) = W%b0 * W%work(vol-1) + W%b1 * (W%InC(vol-1) - W%InC(vol+1))**2
         W%beta(3) = W%b0 * W%work(vol-2) + W%b1 * (W%InC(vol-2) - 4 * W%InC(vol-1) + 3 * W%InC(vol))**2

         sleft = 0.
         sright = 0.

         do r = 1, 3
            s = 1. / (W%epsilon + W%beta(r))**2
            W%alphaleft(r) = W%dleft(r) * s
            W%alpharight(r) = W%dright(r) * s
            sleft = sleft + W%alphaleft(r)
            sright = sright + W%alpharight(r)
         end do

         recleft = dot_product(W%alphaleft, W%left)
         recright = dot_product(W%alpharight, W%right)
         W%reconstructed(2*vol-1) = recleft / sleft
         W%reconstructed(2*vol) = recright / sright
         
      end do

      W%reconstructed(2*size+5:size+8) = W%reconstructed(5:8)
      do vol = 1, size
         W%work(vol+1) = numFlux(W%reconstructed(2*vol+4), W%reconstructed(2*vol+5))
      end do

      W%work(1) = W%work(size+1)
      do vol = 1, size
         Out(vol) = h1 * (W%work(vol+1) - W%work(vol))
      end do
      
    end subroutine weno

    
end module m_weno
