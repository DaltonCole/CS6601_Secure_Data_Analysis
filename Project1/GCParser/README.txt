Only file I made was millionaireProblem.cir
To run it either type ./runparserrandom circuitfile.cir
to run with input files ./runtestgcparser circuitfile.cir inputfile
For more details see given readme

readme for wei
First gcparser must be downloaded and java installed
It can be downloaded directly from
http://mightbeevil.org/gcparser/
or 
https://github.com/wrmelicher/GCParser
Then you can use our given circuit file to securely compute millionare problem
To run use script given in GCParser
runparserrandom runs the circuit with random input
runtestgcparser runs the circuit with inputs from a given input file
we provide two sample input files one with first party greater one with second
to define your own input change values in either of the given files

results are displayed and piped into GCParser/results as siclientout and
siserverout.