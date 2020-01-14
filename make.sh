g++ -std=c++14 -I $HOME/.xacc/include/xacc -c integration_test.cpp -o integration_test.o
g++ integration_test.o -rdynamic -Wl,-rpath,$HOME/.xacc/lib -L /home/cades/.xacc/lib -lxacc -lCppMicroServices -o integration_test.exe
