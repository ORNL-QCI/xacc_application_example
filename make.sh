rm *.o *.x
g++ `$HOME/.exatn/bin/exatn-config --cxxflags` `$HOME/.exatn/bin/exatn-config --includes` -I $HOME/.xacc/include/xacc -c integration_test.cpp -o integration_test.o
g++ integration_test.o -Wl,-rpath,$HOME/.xacc/lib -L $HOME/.xacc/lib -lxacc `$HOME/.exatn/bin/exatn-config --libs` -o integration_test.x
