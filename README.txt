/*
To compile: make

Usage:  pd [-n] [-l] partition1 partition2

where, 
partition1 = a comma-delimited partition, enclosed in quotes
partition2 = a comma-delimited partition, enclosed in quotes

Options: 
-n normalize the partition distance to the maximum possible value
-l relabel partitions sensibly, then print out

Examples: ./pd "1,1,1,1,1,1" "2,2,2,2,2,1"
          ./pd "1,1,1,1,1,1" "2,2,2,2,1,1"
          ./pd "1,1,1,1,1,1" "2,2,2,1,1,1"
          ./pd "1,1,1,1,1,1" "2,2,2,2,3,1"
          ./pd "1,1,1,1,1,1" "2,2,3,3,1,1"
          ./pd "1,1,1,1,1,1" "6,5,4,3,2,1"
          ./pd "1,1,0,0" "3,3,4,4"
          ./pd "1,1,2,2,2,6,6,5,6,7,8,8,946" "4,4,6,6,6,0,0,0,2,2,2,2,4"

Notes: Encode partitions using positive integers. pd returns the partition distance of Gusfield 2002, Information Processing 
       Letters, 82:159, optionally normalized by dividing by the maximum possible partition distance, as described 
       by Charon et al. 2006, Journal of Classification 23:103. Relabeling based on restricted growth function 
       described in Hulsenbeck and Andolfatto 2007, Genetics 175: 1787. 
*/
