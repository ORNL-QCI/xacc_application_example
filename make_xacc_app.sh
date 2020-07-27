rm *.o *.x
g++ -std=c++17 -I $HOME/.xacc/include/xacc -c xacc_app.cpp -o xacc_app.o
g++ xacc_app.o -Wl,-rpath,$HOME/.xacc/lib -L $HOME/.xacc/lib -lxacc -o xacc_app.x
