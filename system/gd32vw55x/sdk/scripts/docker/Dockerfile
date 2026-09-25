FROM ubuntu:22.04

# set noninteractive mode for apt to avoid prompts
ENV DEBIAN_FRONTEND=noninteractive

# install build tools and utilities needed to compile SDK and manage files
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    python3 \
    python3-pip \
    git \
    wget \
    xz-utils \
    bzip2 \
    unzip \
    zip \
    libncurses5 \
    srecord \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# install python crypto library without caching to reduce image size
RUN pip3 install --no-cache-dir pycryptodome
RUN pip3 install --no-cache-dir cbor
RUN pip3 install --no-cache-dir cryptography
RUN pip3 install --no-cache-dir pycryptodome
RUN pip3 install --no-cache-dir pyasn1
RUN pip3 install --no-cache-dir intelhex

# argument for SDK repo url; default is official github mirror
ARG GD32VW55X_SDK_URL=https://github.com/GigaDeviceSemiconductor/GD32VW55x_WiFi_BLE_SDK.git

# clone SDK repo, extract toolchain and OpenOCD into /opt
RUN git clone ${GD32VW55X_SDK_URL} /opt/gd32_sdk && \
    cat /opt/gd32_sdk/tools/gd32vw55x_toolchain_linux.tar.gz* | tar xvz -C /opt/ && \
    tar xvzf /opt/gd32_sdk/tools/xpack-openocd-0.11.0-3_linux.tar.gz -C /opt/ && \
    rm -rf /opt/gd32_sdk

# add toolchain and openocd binaries to PATH
ENV PATH="/opt/gd32vw55x_toolchain_linux/bin:${PATH}"
ENV PATH="/opt/xpack-openocd-0.11.0-3_linux/bin:${PATH}"

# create project workspace directory and make it the working dir
RUN mkdir -p /project
WORKDIR /project

# expose common debug ports (e.g. OpenOCD default)
EXPOSE 3333 4444

# default command to run when container starts
CMD ["/bin/bash"]
