FROM ubuntu:25.04

RUN apt-get update && apt-get install -y cmake git g++ libssl-dev zlib1g-dev

# install h2o
WORKDIR /
RUN git clone --recurse-submodules https://github.com/h2o/h2o.git
WORKDIR /h2o
RUN mkdir -p build
WORKDIR /h2o/build
RUN cmake ..
RUN make
RUN make install
WORKDIR /
COPY h2o.yaml /h2o.yaml

ENTRYPOINT [ "h2o", "-m", "master", "-c", "/h2o.yaml" ]