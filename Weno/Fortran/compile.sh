#!/bin/bash

gfortran -O3 -march=native m_burghers.F90 m_godunov.F90 m_weno.F90 m_RK3TVDData.F90 main.F90 -o weno
