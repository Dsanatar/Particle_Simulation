
//install clang
$wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
$sudo apt-add-repository "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-6.0 main"
$sudo apt-get update
$sudo apt-get install -y clang-6.0

//install CImg
$sudo apt-get install cimg-dev 


//To run program

$ clang++ -std=c++11 -O3 simulation.cpp particle.cpp -lX11 -lpthread -o test
$ ./test
