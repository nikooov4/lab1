set datafile separator ","
set title "Load Test Comparison"
set xlabel "Elements"
set ylabel "Time (s)"
set grid
set autoscale
set term png size 1024,768
set output 'load_test_plot.png'

plot 'load_test_results.csv' using 1:2 with linespoints title 'UniquePtr' linecolor rgb '#FF5733', \
     'load_test_results.csv' using 1:3 with linespoints title 'std::unique_ptr' linecolor rgb '#33FF57', \
     'load_test_results.csv' using 1:4 with linespoints title 'SharedPtr' linecolor rgb '#3357FF', \
     'load_test_results.csv' using 1:5 with linespoints title 'std::shared_ptr' linecolor rgb '#FF33A1'
