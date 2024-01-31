FROM ros:foxy

# Install dependencies
RUN apt-get update && apt-get install -y \
    git \
    pip \
    cmake \
    build-essential \
    libusb-1.0-0-dev \
    python3 \
    python3-colcon-common-extensions

# Clone Dynamixel SDK repository
RUN git clone https://github.com/ROBOTIS-GIT/DynamixelSDK.git /opt/DynamixelSDK

# Build Dynamixel SDK
RUN cd /opt/DynamixelSDK/python && python3 setup.py install

# Install dependencies for Dynamixel SDK Python
#
# Define the command to run when the container starts
CMD ["bash", "-c", "source /opt/ros/foxy/setup.bash && bash"]

## Set up Python environment

