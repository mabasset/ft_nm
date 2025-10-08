#!/bin/bash

diff <(nm "$@") <(./ft_nm "$@")
