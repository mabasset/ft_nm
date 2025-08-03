#!/bin/bash

make

cp ft_nm nm_tester/

docker build \
    --build-arg NAME=$(id -un) \
    --build-arg UID=$(id -u) \
    --build-arg GID=$(id -g) \
    -t ft_nm_image \
    .