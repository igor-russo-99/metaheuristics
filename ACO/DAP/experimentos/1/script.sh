#!/bin/bash

#make -B
#make media -B

declare -i PCORES
PCORES=4


for k in {100,200,300,400,500,600,700,800,900,1000}
do
    printf "Execuções com $k iterações:\n"
    
    printf "" > execucoes/$k.txt
    
    for i in {1..100}
    do
        printf "Execução $i \n"
        
        (../../src/dap 1000 50 0.5 0.01 1.0 $i) >> execucoes/$k.txt    
    
    done
    
done


#Script para plotar o gráfico
#gnuplot scripts_grafico.gnu
#eog Experimento/grafico.png



