FROM debian:stretch
MAINTAINER Maksym Planeta <mplaneta@os.inf.tu-dresden.de>

ENV	WHISK="whisk2"

RUN	apt-get update && \
    apt-get -y --no-install-recommends install \
        binutils \
        cmake \
        gcc \
        g++ \
        libc-dev \
        libc++-dev \
        libboost-all-dev \
        libsimgrid-dev \
        make \
        simgrid \
        sudo

RUN	useradd -m whisk && \
    echo whisk:whisk | chpasswd && \
    cp /etc/sudoers /etc/sudoers.bak && \
    echo 'whisk ALL=(root) NOPASSWD: ALL' >> /etc/sudoers
USER	whisk
WORKDIR	/home/whisk

# Copy across source files needed for build

RUN mkdir ${WHISK}
ADD CMakeLists.txt \
    ${WHISK}/
ADD include ${WHISK}/include
ADD src ${WHISK}/src
ADD doc ${WHISK}/doc

RUN sudo chown --recursive whisk: ${WHISK}

RUN cd ${WHISK} && \
    mkdir build && cd build && \
    cmake .. && \
    make -j 5 && sudo make install

