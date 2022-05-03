
FROM docker.io/drogonframework/drogon

ENV DEBIAN_FRONTEND="noninteractive"
ENV CXX="/usr/bin/g++-11"
ENV VCPKG_ROOT="/source/vcpkg"

WORKDIR /source
RUN apt-get update -y && \
    apt-get install software-properties-common -y && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get install g++-11 zip cmake -y && \
    ln -sf /usr/bin/g++-11 /usr/bin/g++ && \
    git clone https://github.com/microsoft/vcpkg --depth 1 && \
    vcpkg/bootstrap-vcpkg.sh -disableMetrics && \
    vcpkg/vcpkg install libsodium #&& \


COPY . .
ARG use_postgresql=OFF
RUN pwd
RUN ls
RUN  mkdir build/ && \
    cd build/ && \
    cmake -DCMAKE_BUILD_TYPE=Release .. -DENABLE_ADDITIONAL_WARNINGS=ON -DUSE_POSTGRESQL="$use_postgresql" && \
    cmake --build . -- -j4 && \
    mkdir /app && \
    mv /source/build/app/defect_tracker /source/build/app/test-defect_tracker /source/data /app && \
    mv /source/sql /sql #&& \
    #apt-get remove g++-11 git curl cmake zip pkg-config autoconf -y && \
    #apt-get autoremove -y && \
    #apt-get clean -y

RUN ls /app
RUN ls /app/data

ENTRYPOINT /app/defect_tracker $PORT






