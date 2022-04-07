FROM ubuntu:latest
WORKDIR /code

RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y gcc make cmake man wget g++

# Default powerline10k theme, no plugins installed
RUN sh -c "$(wget -O- https://github.com/deluan/zsh-in-docker/releases/download/v1.1.2/zsh-in-docker.sh)"
