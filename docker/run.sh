#!/usr/bin/env bash

CPU_IND=0
GPU_IND=0

IMAGE_NAME="m-ipc"

# to do single run
sudo docker run --rm -it \
	-v $(pwd):/Medial-IPC/ \
	--cpuset-cpus ${CPU_IND} -m 10g \
	--runtime=nvidia -e NVIDIA_VISIBLE_DEVICES=${GPU_IND} \
	${IMAGE_NAME} /bin/bash
