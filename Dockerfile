FROM ubuntu:latest

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary packages for ROS 2
RUN apt-get update && apt-get install -y \
    curl \
    gnupg2 \
    lsb-release \
    git \
    cmake \
    build-essential \
    libusb-1.0-0-dev \
    && rm -rf /var/lib/apt/lists/*

# Add ROS 2 repository and install ROS 2 packages
RUN curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | apt-key add - \
    && sh -c 'echo "deb [arch=$(dpkg --print-architecture)] http://packages.ros.org/ros2/ubuntu $(lsb_release -cs) main" > /etc/apt/sources.list.d/ros2-latest.list' \
    && apt-get update && apt-get install -y \
    ros-humble-desktop \
    && rm -rf /var/lib/apt/lists/*

# Source ROS 2 setup script
RUN echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc

# Optional: Install additional ROS 2 packages or dependencies


# Clone Dynamixel SDK repository
RUN git clone https://github.com/ROBOTIS-GIT/DynamixelSDK.git /opt/DynamixelSDK

# Build Dynamixel SDK
RUN cd /opt/DynamixelSDK/c++ && mkdir build && cd build && cmake .. && make

# Install dependencies for Dynamixel SDK Python
RUN pip install pyserial

# Set entrypoint
CMD ["bash"]