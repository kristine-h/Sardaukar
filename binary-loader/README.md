A binary loader implementation in C++ using The Binary File Descriptor library (libbfd)

The program displays some basic information about the binary such as the filename, type, architecture, endianness and entry point. It then prints the number of symbols contained in both static and dynamic symbol tables. 

Example usage:
g++ test.cpp loader.cpp -lbfd -o test
./test <binary file>

Resources:
Practical Binary Analysis, Dennis Andriesse
