FROM debian:bookworm

ARG USER=student
ARG UID=1000
ARG GID=1000

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y build-essential \
        gcc-multilib \
        valgrind \
        sudo \
        git \
        vim

RUN groupadd -g ${GID} ${USER} && \
    useradd -m -s /bin/bash -u ${UID} -g ${GID} ${USER} && \
    usermod -aG sudo ${USER}

RUN mkdir /home/${USER}/

COPY    ft_nm /usr/bin/ft_nm
RUN     chmod 777 /usr/bin/ft_nm

WORKDIR /home/${USER}/ft_nm
USER ${USER}

CMD ["sleep", "infinity"]