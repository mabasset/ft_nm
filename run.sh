#!/bin/bash

# diff <(docker run --rm ft_nm_image nm $1 2>&1) \
#      <(docker run --rm ft_nm_image ./ft_nm $1 2>&1)

# docker run --rm ft_nm_image ./run all

# docker run --rm ft_nm_image ./ft_nm $1 2>&1

docker run --rm ft_nm_image $1 $2 $3
#./run.sh ./ft_nm -a ressources/stack-buffer-overflow-i386  > containers2.txt