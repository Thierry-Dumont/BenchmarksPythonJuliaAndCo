module m_burghers

  implicit none

contains
  
  function minf(a, b) result(m)

    real(kind = 8) :: a, b, m

    if(b <= 0.) then
       m = 0.5 * b**2
    else if(a >= 0.) then
       m = 0.5 * a**2
    else
       m = 0.
    end if
    
  end function minf


  function maxf(a, b) result(M)
    
    real(kind = 8) :: a, b, M
    M = 0.5 * max(a**2, b**2)
    
  end function maxf
  
end module m_burghers
