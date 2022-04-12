
FROM ubuntu:20.04

WORKDIR /src
ENV DEBIAN_FRONTEND="noninteractive"

#Obtain the build tools
RUN apt-get update -y && \
    apt-get install software-properties-common -y && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get install g++-11 git curl cmake zip pkg-config autoconf libjsoncpp-dev uuid-dev openssl libssl-dev zlib1g-dev -y && \
    ln -sf /usr/bin/g++-11 /usr/bin/g++
ENV CXX="/usr/bin/g++-11"

#Obtain the libraries
RUN git clone https://github.com/microsoft/vcpkg --depth 1 && \
    vcpkg/bootstrap-vcpkg.sh -disableMetrics && \
    vcpkg/vcpkg install libsodium drogon[core,orm,postgres,ctl]
RUN mv vcpkg/installed/x64-linux/tools/drogon/drogon_ctl /usr/bin/
ENV VCPKG_ROOT="/src/vcpkg"

#Build the app
COPY . .
RUN mkdir build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . -- -j4

WORKDIR /app
RUN mv /src/build/app/defect_tracker /src/data /app && \
    rm -rf /src && \
    apt-get remove g++-11 git curl cmake zip pkg-config autoconf -y && \
    apt-get autoremove -y && \
    apt-get clean -y


ENTRYPOINT ./defect_tracker $PORT












