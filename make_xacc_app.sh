rm *.o *.x
g++ `$HOME/.exatn/bin/exatn-config --cxxflags` `$HOME/.exatn/bin/exatn-config --includes` -std=c++17 -I $HOME/.xacc/include/xacc -c xacc_app.cpp -o xacc_app.o
g++ xacc_app.o -Wl,-rpath,$HOME/.xacc/lib -L $HOME/.xacc/lib -lxacc `$HOME/.exatn/bin/exatn-config --libs` -o xacc_app.x
