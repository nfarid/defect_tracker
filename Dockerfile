
#Setup build
FROM docker.io/drogonframework/drogon
WORKDIR /src
ENV DEBIAN_FRONTEND="noninteractive"

#Obtain the build tools
RUN apt-get update -y && \
    apt-get install software-properties-common -y && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get install g++-11 zip cmake -y && \
    ln -sf /usr/bin/g++-11 /usr/bin/g++
ENV CXX="/usr/bin/g++-11"

#Obtain the libraries
RUN git clone https://github.com/microsoft/vcpkg --depth 1 && \
    vcpkg/bootstrap-vcpkg.sh -disableMetrics && \
    vcpkg/vcpkg install libsodium
ENV VCPKG_ROOT="/src/vcpkg"

#Copy source files
COPY . .

ARG use_postgresql=OFF
#Build the application
RUN mkdir build/ && \
    cd build/ && \
    cmake -DCMAKE_BUILD_TYPE=Release .. -DENABLE_ADDITIONAL_WARNINGS=ON -DUSE_POSTGRESQL="$use_postgresql" && \
    cmake --build . -- -j4

#Cleanup
WORKDIR /app
RUN mv /src/build/app/defect_tracker /src/build/app/test-defect_tracker /src/data /app && \
    mv /src/sql /sql && \
    apt-get remove g++-11 git curl cmake zip pkg-config autoconf -y && \
    apt-get autoremove -y && \
    apt-get clean -y

#Provide entry point
ENTRYPOINT /app/defect_tracker $PORT






