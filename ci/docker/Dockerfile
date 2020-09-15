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
    curl \
    doxygen \
    g++ \
    gcc \
    gcc-arm-none-eabi \
    git \
    graphviz \
    hugo \
    lcov \
    libgtest-dev \
    libncurses5 \
    libxml2-utils \
    ninja-build \
    plantuml \
    python3 \
    python3-pip

RUN python3 -m pip install pipenv

RUN JLINK_ARCH=`([ $(uname -m) = 'x86_64' ] && echo 'x86_64') || ([ $(uname -m) = 'armv7l' ] && echo 'arm')` \
    && TEMP_DEB="$(mktemp)" \
    && curl -X POST -F 'accept_license_agreement=accepted' "https://www.segger.com/downloads/jlink/JLink_Linux_${JLINK_ARCH}.deb" -o "$TEMP_DEB" \
    && dpkg -i "$TEMP_DEB" \
    && rm -f "$TEMP_DEB"
