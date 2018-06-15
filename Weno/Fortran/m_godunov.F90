module m_godunov

  use m_burghers
  
  implicit none

contains
  
  function numFlux(a, b) result(flux)
    
    real(kind = 8) :: a, b
    real(kind = 8) :: flux
    
    if(a <= b) then
       flux = minf(a, b)
    else
       flux = maxf(b, a)
    end if
    
  end function numFlux
  
end module m_godunov
