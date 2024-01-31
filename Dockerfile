FROM ros:foxy

# Install dependencies
RUN apt-get update && apt-get install -y \
    git \
    cmake \
    build-essential \
    libusb-1.0-0-dev \
    python3-colcon-common-extensions

# Clone Dynamixel SDK repository
RUN git clone https://github.com/ROBOTIS-GIT/DynamixelSDK.git /opt/DynamixelSDK

# Build Dynamixel SDK
RUN cd /app/DynamixelSDK/c++ && rm -rf build && mkdir build && cd build && cmake .. && make

# Install dependencies for Dynamixel SDK Python
RUN pip install pyserial

## Set up Python environment
#WORKDIR /app
#RUN pip install --no-cache-dir -r requirements.txt
#
## Copy Dynamixel Python example file
#COPY example_python/protocol1.0/ping/ping.py .

CMD ["python", "ping.py"]
