#!/bin/bash

docker build \
    --build-arg NAME=$(id -un) \
    --build-arg UID=$(id -u) \
    --build-arg GID=$(id -g) \
    -t ft_nm_image \
    .

docker run --name ft_nm_container -d ft_nm_image

docker exec -it ft_nm_container /bin/bash

