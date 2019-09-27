#!/bin/bash

#module load Julia
julia -O3 --check-bounds=no main1d.jl
