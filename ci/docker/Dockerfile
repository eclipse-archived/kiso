# Get latest debian (>=10)
FROM debian:10

# Set the working directory to /app
WORKDIR /cddk-project

# Define environment variable
ENV NAME cddk-container

# Update package management
RUN apt-get update

# Install python and pip
RUN apt-get install -y python3

# Get cmake & ninja
RUN apt-get install -y cmake ninja-build

# Get latest static analysis tools
RUN apt-get install -y clang-format clang-tidy cppcheck

# Get latest gtest
RUN apt-get install -y libgtest-dev

# Get latest coverage report generator
RUN apt-get install -y lcov

# Get doxygen
RUN apt-get install -y doxygen graphviz

# Get the compilers
RUN apt-get install -y gcc-arm-none-eabi gcc g++
