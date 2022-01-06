# FROM nvidia/cuda:11.3.1-cudnn8-runtime-ubuntu20.04
FROM nvidia/cudagl:11.3.0-devel-ubuntu20.04
LABEL maintainer "chungmin99@gmail.com"

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=America/Vancouver

RUN apt-get update -y && \
    apt-get install -y \
        python3-pip \ 
        python3-dev \
        ninja-build \
        freeglut3-dev \
	# Remaining dependencies
        liboctomap-dev libfcl-dev \
        gifsicle libfreetype6-dev libpng-dev \
        build-essential libsuitesparse-dev libmetis-dev \
        libgmp-dev libopenblas-dev libomp-dev cmake intel-mkl libeigen3-dev \
        git libglu1-mesa-dev mesa-common-dev libqglviewer-dev-qt5 \
        xorg-dev libboost-program-options-dev libboost-all-dev && \
    rm -rf /var/lib/apt/lists/*

RUN echo 'export CUDA_HOME=/usr/local/cuda' >> ~/.bashrc
RUN echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda/lib64:/usr/local/cuda/extras/CUPTI/lib64' >> ~/.bashrc
RUN echo 'export PATH=$PATH:$CUDA_HOME/bin' >> ~/.bashrc