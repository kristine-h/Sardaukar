### A binary loader implementation in C++ using The Binary File Descriptor library (libbfd)

The program displays some basic information about the binary such as the _filename_, _type_, _architecture_, _endianness_ and _entry point_. 
It then prints the _number of symbols_ contained in both _static_ and _dynamic symbol tables_. 

#### Example usage
`g++ test.cpp loader.cpp -lbfd -o test`

`./test <binary file>`

#### Resources
_Practical Binary Analysis, Dennis Andriesse_
