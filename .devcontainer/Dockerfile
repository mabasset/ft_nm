FROM debian:bookworm

ARG USER=developer
ARG UID=1000
ARG GID=1000

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y build-essential \
        valgrind \
        git

RUN groupadd -g ${GID} ${USER} && \
    useradd -m -s /bin/bash -u ${UID} -g ${GID} ${USER} && \
    usermod -aG sudo ${USER}

WORKDIR /home/${USER}/ft_nm
USER ${USER}

CMD ["sleep", "infinity"]
