#!/bin/bash

# diff <(docker run --rm ft_nm_image nm $1) \
#      <(docker run --rm ft_nm_image ./ft_nm $1)
diff <(docker run --rm ft_nm_image ./ft_nm $1) \
     <(docker run --rm ft_nm_image nm $1)