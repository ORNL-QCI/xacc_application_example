rm *.o *.x
g++ `$HOME/.exatn/bin/exatn-config --cxxflags` `$HOME/.exatn/bin/exatn-config --includes` -c exatn_app.cpp -o exatn_app.o
g++ exatn_app.o `$HOME/.exatn/bin/exatn-config --libs` -o exatn_app.x
