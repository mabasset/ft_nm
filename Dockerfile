FROM debian:bookworm

ARG NAME=student
ARG UID=1000
ARG GID=1000

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y \
        build-essential \
        make \
        valgrind \
        git \
        vim

RUN groupadd -g ${GID} ${NAME} && \
    useradd -m -s /bin/bash -u ${UID} -g ${GID} ${NAME} && \
    usermod -aG sudo ${NAME} && \
    echo "${NAME} ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

USER    ${NAME}
WORKDIR /home/${NAME}/ft_nm

CMD ["sleep", "infinity"]