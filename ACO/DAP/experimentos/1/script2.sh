g++ ../../src/main.cpp -o dap -D EXPERIMENTO -D V_1

printf "" > execucoes/sucesso_1.txt   

for k in {100,200,300,400,500,600,700,800,900,1000}
do
    printf "Execuções com $k iterações:\n"       
        
    (./dap $k 50 0.5 0.01 1.0 1 100) >> execucoes/sucesso_1.txt    
        
done

g++ ../../src/main.cpp -o dap -D EXPERIMENTO -D V_2

printf "" > execucoes/sucesso_2.txt   

for k in {100,200,300,400,500,600,700,800,900,1000}
do
    printf "Execuções com $k iterações:\n"       
        
    (./dap $k 50 0.5 0.01 1.0 1 100) >> execucoes/sucesso_2.txt
    
done
