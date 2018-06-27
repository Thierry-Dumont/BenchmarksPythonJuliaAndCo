program main

  use m_weno
  use m_RK3TVDData
  
  implicit none

  integer, parameter :: size = 1000
  real(kind = 8), parameter :: L = 1.
  real(kind = 8), parameter :: T = 1.
  real(kind = 8), parameter :: dt = 0.8 / size
  
  real(kind = 8) :: tt, start, finish

  real(kind = 8), dimension(size) :: In, Out, Temp

  character(len=64) :: hostname

  type(WenoData) :: W
  type(RK3TVDData) :: R
  
  print *, "size=",size, "dt=",dt, "nsteps=",floor(T/dt)

  call init(In, L, size)
  call initWenoData(W, size)
  call initRK3TVDData(R, size)

  open(unit=12, file="gp0", action="write", status="replace")
  write(12, "(1f16.12)") In
  close(12)
  
  print *, "Start computation"

  tt = 0.
  call cpu_time(start)
  do while(tt < T)
     call Rk3tvd(R, W, L, dt, In, Out)
     Temp = In
     In = Out
     Out = Temp

     tt = tt + dt
  end do
  call cpu_time(finish)

  print *, "Time : ", finish - start

  open(unit=12, file="gp", action="write", status="replace")
  write(12, "(1f16.12)") In
  close(12)

  call hostnm(hostname)
  open(unit=12, file="../RunningOn" // trim(hostname), action="write", status="replace")
  write(12, *) finish - start
  close(12)
  
end program main


subroutine init(X, L, len)

  implicit none

  integer, intent(in) :: len
  real(kind = 8), intent(in) :: L
  real(kind = 8), dimension(len), intent(inout) :: X

  real(kind = 8) :: h
  integer :: i

  h = L / len

  do i = 0, len-1
     if(i > floor(len / 8.) .and. i < floor(len / 2.) + floor(len / 8.)) then
        X(i+1) = 1. - 2. * (i - floor(len / 8.)) * h / L
     else
        X(i+1) = 0.
     end if
  end do
     
end subroutine init
