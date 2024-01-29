# Use an appropriate base image, such as Ubuntu
FROM ubuntu:latest

# Install necessary packages for APT
RUN apt-get update && apt-get install -y \
    curl \
    gnupg2 \
    lsb-release \
    && rm -rf /var/lib/apt/lists/*



RUN locale \
    sudo apt update && sudo apt install locales \
    sudo locale-gen en_US en_US.UTF-8\
    sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8\
    export LANG=en_US.UTF-8\
    locale

RUN sudo apt install software-properties-common\
    sudo add-apt-repository universe \
    sudo apt update && sudo apt install curl -y \
    sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg

RUN echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null

RUN sudo apt update \
    sudo apt upgrade

RUN sudo apt install ros-humble-desktop

RUN source /opt/ros/humble/setup.bash

RUN ros2 wtf