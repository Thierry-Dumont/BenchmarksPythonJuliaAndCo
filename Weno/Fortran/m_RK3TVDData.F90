module m_RK3TVDData

  use m_burghers
  use m_godunov
  use m_weno
  
  implicit none

  private
  
  public :: initRK3TVDData
  public :: Rk3tvd
  
  type, public :: RK3TVDData
     
     integer :: size
     
     real(kind = 8) :: c21, c22, c31, c32
     real(kind = 8), dimension(:), allocatable :: u1, u2
     
  end type RK3TVDData
  
  
  contains

    subroutine initRK3TVDData(rdata, len)

      type(RK3TVDData), intent(inout) :: rdata
      integer :: len

      rdata%size = len
      rdata%c21 = 3/4.
      rdata%c22 = 1/4.
      rdata%c31 = 1/3.
      rdata%c32 = 2/3.

      allocate(rdata%u1(len))
      allocate(rdata%u2(len))
      rdata%u1 = 0.
      rdata%u2 = 0.
      
    end subroutine initRK3TVDData


    subroutine Rk3tvd(R, W, L, dt, In, Out)

      type(RK3TVDData), intent(inout) :: R
      type(WenoData), intent(inout) :: W
      real(kind = 8), intent(in) :: dt, L
      real(kind = 8), dimension(:), intent(in) :: In
      real(kind = 8), dimension(:), intent(inout) :: Out

      integer :: size

      size = R%size
      call weno(W, L, In, R%u1)
      R%u1 = In + dt * R%u1

      call weno(W, L, R%u1, R%u2)
      R%u2 = R%c21 * In + R%c22 * (R%u1 + dt * R%u2)

      call weno(W, L, R%u2, R%u1)
      Out = R%c31 * In + R%c32 * (R%u2 + dt * R%u1)

    end subroutine Rk3tvd
    
end module m_RK3TVDData
