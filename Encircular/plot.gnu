# set terminal png transparent nocrop enhanced size 450,320 font "arial,8" 
# set output 'steps.1.png'
set style data lines
set title "Robot movement plot" 
x = 0.0
## Last datafile plotted: "steps.dat"
plot [-12:12][-12:12] "plot_data_0.dat" title 'bogus'
