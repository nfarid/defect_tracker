
FROM docker.io/drogonframework/drogon

WORKDIR /src
ENV DEBIAN_FRONTEND="noninteractive"
RUN apt-get update -y
RUN apt-get install zip cmake -y
RUN git clone https://github.com/microsoft/vcpkg --depth 1

WORKDIR vcpkg
RUN ls
RUN ./bootstrap-vcpkg.sh -disableMetrics
RUN /src/vcpkg/vcpkg install libsodium
ENV VCPKG_ROOT="/src/vcpkg"

WORKDIR /src
COPY cmake/ cmake/
COPY src/ src/
COPY tools tools/
COPY data data/
COPY CMakeLists.txt ./

RUN mkdir build/
WORKDIR build/
RUN cmake ..
RUN cmake --build . -- -j12

RUN mkdir /app
RUN mv app/defect_tracker /app

WORKDIR /app
ENTRYPOINT ./defect_tracker $PORT






