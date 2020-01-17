g++ -std=c++14 -g -I $HOME/.xacc/include/xacc -c integration_test.cpp -o integration_test.o
g++ integration_test.o -rdynamic -Wl,-rpath,$HOME/.xacc/lib:$HOME/intel/mkl/lib/intel64 -L $HOME/.xacc/lib -lxacc -lCppMicroServices -o integration_test.x
