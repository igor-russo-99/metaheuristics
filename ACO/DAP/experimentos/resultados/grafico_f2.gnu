set xrange[100:1000]
set yrange[0:100]
set terminal pdf
set output 'grafico.pdf'
set pointsize 0.8

set xlabel "Número de iterações"
set ylabel "Taxa de sucesso (%)"

set key left

plot 'dados_f1_1.dat' using 1:2 title 'Original' with linespoints, 'dados_f1_2.dat' using 1:2 title 'Alteração' with linespoints
