FROM ubuntu:latest

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    cmake clang \
    llvm-15 llvm-15-dev llvm-15-doc llvm-15-tools \
    mlir-15-tools libmlir-15 libmlir-15-dev \
    libboost-dev libboost-doc


