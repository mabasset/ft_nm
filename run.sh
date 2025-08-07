#!/bin/bash

diff <(docker run --rm ft_nm_image nm "$@") \
     <(docker run --rm ft_nm_image ./ft_nm "$@")
