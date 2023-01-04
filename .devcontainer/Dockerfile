FROM ubuntu:latest

ARG USERNAME=devuser
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN apt-get update -y -qq && \
    apt-get install build-essential cmake ninja-build llvm-dev zlib1g-dev g++ python3-dev autotools-dev libicu-dev libbz2-dev gdb sudo openssh-client wget -y -qq && \
    apt-get install mpich -y -qq && \
    apt-get clean && \
    wget https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.tar.gz && \
    tar xvf boost_1_81_0.tar.gz && rm boost_1_81_0.tar.gz && cd boost_1_81_0/ && \
    ./bootstrap.sh --prefix=/usr/ --with-libraries=all && echo using mpi \; >> tools/build/src/user-config.jam && ./b2 install threading=multi link=shared && \
    cd .. && rm -r -f boost_1_81_0 && \
    groupadd --gid $USER_GID $USERNAME && \
    useradd --uid $USER_UID --gid $USER_GID -m $USERNAME -s /bin/bash && \
    echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME && \
    chmod 0440 /etc/sudoers.d/$USERNAME 


USER $USERNAME