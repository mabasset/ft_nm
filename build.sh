#!/bin/bash

docker build \
    --build-arg NAME=$(id -un) \
    --build-arg UID=$(id -u) \
    --build-arg GID=$(id -g) \
    -t ft_nm_image \
    .

