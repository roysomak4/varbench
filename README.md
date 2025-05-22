# VarBench (VARiant BENCHmarking tool)
Varbench is a tool to quickly compare a query VCF with a benchmark VCF and calculate performance metrics, i.e, recall, precision, and F-score. It has been tested on Debian Linux but should work with most Linux distros. 

MacOS is currently not supported due to gzstream library issue (work in progress). Workaround is to build the app using a linux container. An out of the box option is to use microsoft vscode devcontainers `docker pull mcr.microsoft.com/devcontainers/cpp:debian-11`

## Install instructions
Dependency (included in `include` and `lib` folders) Separate install and compile is NOT required.
1. zlib
2. gzstream (to handle gzipped VCF files)

Clone the repository
```
git clone git@gitlab.com:roysomak4/ngs_benchmarking_tools.git
```
Change into the folder varbench folder
```
cd ngs_benchmarking_tools/cpp/varbench
```
create a build directory and change into it
```
mkdir build && cd build
```
Use the following instructions to compile
```
cmake ../ && cmake --build .
```

## Usage
Once compiled, it is simple to use `varbench`. Simply run the following command
```
./varbench <benchmark.vcf.gz> <query.vcf.gz> <comparison_result.txt>
```
The three positional arguments are required.