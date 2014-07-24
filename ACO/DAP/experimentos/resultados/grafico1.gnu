set xrange[100:1000]
set yrange[0:100]
set terminal eps
set pointsize 1.0

set xlabel "Número de iterações"
set ylabel "Taxa de sucesso (%)"

set key left

set output 'graficoF1.eps'
plot 'dados_1.dat' using 1:2 title 'DAP' with linespoints lw 2

set key right

set output 'graficoF2.eps'
plot 'dados_1.dat' using 1:3 title 'DAP' with linespoints lw 2

set key left

set output 'graficoF3.eps'
plot 'dados_1.dat' using 1:4 title 'DAP' with linespoints lw 2
