# Get latest debian (>=10)
FROM debian:10

# Set the working directory to /app
WORKDIR /kiso-project

# Define environment variable
ENV NAME kiso-container

# Update package management and install necessary packages
RUN apt-get update && apt-get install -y \
    clang-format \
    clang-tidy \
    cmake \
    cppcheck \
    doxygen \
    g++ \
    gcc \
    gcc-arm-none-eabi \
    git \
    graphviz \
    hugo \
    lcov \
    libgtest-dev \
    libxml2-utils \
    ninja-build \
    plantuml \
    python3 \
    python3-pip

RUN python3 -m pip install pipenv
