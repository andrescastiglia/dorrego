FROM ubuntu:latest

ARG USERNAME=devuser
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN apt-get update -y -qq && \
    apt-get install build-essential cmake gdb sudo -y -qq && \
    apt-get install mpich -y -qq && \
    apt-get clean && \
    groupadd --gid $USER_GID $USERNAME && \
    useradd --uid $USER_UID --gid $USER_GID -m $USERNAME -s /bin/bash && \
    echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME && \
    chmod 0440 /etc/sudoers.d/$USERNAME 

USER $USERNAME