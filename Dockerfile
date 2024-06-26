FROM ros:humble

# Install dependencies
RUN apt-get update && apt-get install -y \
    git \
    python3-pip \
    pip \
    cmake \
    build-essential \
    libusb-1.0-0-dev \
    python3 \
    python3-colcon-common-extensions \
    nano


# Clone Dynamixel SDK repository
RUN git clone https://github.com/ROBOTIS-GIT/DynamixelSDK.git /opt/DynamixelSDK

# Build Dynamixel SDK
RUN cd /opt/DynamixelSDK/python && python3 setup.py install


# Set the working directory inside the container
WORKDIR /app

# Copy everything from the local "folder_to_copy" directory into the container's "/app" directory
COPY pysoftware/ /app/

# Install dependencies for Dynamixel SDK
RUN pip3 install -r requirements.txt

# Run deployment
RUN chmod +x install-caddy.sh \
    ./install-caddy.sh

# Define the command to run when the container starts
CMD ["bash", "-c", "source /opt/ros/humble/setup.bash && bash"]

## Set up Python environment

