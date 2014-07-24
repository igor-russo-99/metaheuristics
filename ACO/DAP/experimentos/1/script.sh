#!/bin/bash

#make -B
#make media -B

g++ ../../src/main.cpp -o dap -D EXPERIMENTO -D V_2

for k in {100,200,300,400,500,600,700,800,900,1000}
do
    printf "Execuções com $k iterações:\n"
    
    printf "" > execucoes/$k.txt
    
    for i in {1..100}
    do
        printf "Execução $i \n"
        
        (./dap $k 50 0.5 0.01 1.0 $i 1) >> execucoes/$k.txt    
    
    done
    
done

