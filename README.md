# Demonstration of integrating XACC with existing or new application

Here we provide an example CMakeLists.txt file that demonstrates how 
to import the XACC CMake target. To build run the following 

```bash
$ mkdir build && cd build
$ cmake .. -DXACC_DIR=$HOME/.xacc
$ make 
$ ./integration_test
```

We also provide a script demonstrating how to build and link 
an application using XACC. See the commands in make.sh.
