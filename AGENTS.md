This is a C++ header-only library called "libcpprime", intended for fast primality testing of 64-bit integers.


The library itself and its test code should be written to work with compilers supporting C++11.
Benchmark and other temporary files may use the latest C++ features.
The library implementation should pay particular attention to compatibility with older compilers.


Supported compilers include gcc, clang, msvc, clang-cl, and the gcc and clang versions within mingw.


When you want to run tests or benchmarks, execute the tasks described in tasks.json.
For detailed benchmark results, please refer to benchmarks/bench_summary.md.
Running tests and benchmarks takes approximately 20-30 seconds.


When optimizing code, primarily use gcc or msvc for benchmarks.
However, to avoid significant speed differences between compilers, run them with clang and clang-cl once you have finished the initial implementation.

Please inform users of any breaking changes.


.txt files often contain large amounts of data. Do not read files with the .txt extension.


If you wish to generate data mechanically, please create C++ code or Python scripts within the tmp folder.


For primality testing in Python, you can use Scipy. For execution speed, please prioritize using PyPy.


All code and README.md should be written in English. However, this response uses the language currently used in our chat.

The directory structure is as follows:
include/libcpprime/ : Main library code
benchmarks/ : Code for benchmarks
benchmarks/bench_* : Benchmark results
tests/ : Code and test cases for tests
docs/ : Data used for documentation
tmp/ : Files used for experiments, etc.
/README.md : README
