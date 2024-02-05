# zin
from dynamixel_sdk import *

def move_mortor():
    # Set the port and baudrate
    DEVICENAME = '/dev/ttyUSB0'  # Modify this according to your setup
    BAUDRATE = 57600  # Modify this according to your Dynamixel configuration

    # Define protocol version
    PROTOCOL_VERSION = 2.0

    # Initialize PortHandler instance
    portHandler = PortHandler(DEVICENAME)

    # Initialize PacketHandler instance
    packetHandler = PacketHandler(PROTOCOL_VERSION)

    # Open the port
    if portHandler.openPort():
        print("Succeeded to open the port")
    else:
        print("Failed to open the port")
        exit(1)

    # Set the baudrate
    if portHandler.setBaudRate(BAUDRATE):
        print("Succeeded to change the baudrate")
    else:
        print("Failed to change the baudrate")
        exit(1)

    # Set goal position
    DXL_ID = 1  # Modify this according to your motor ID
    DXL_GOAL_POSITION = 512  # Modify this according to your goal position

    # Enable Dynamixel torque
    dxl.write1ByteTxRx(portHandler, DXL_ID, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE)

    # Set goal position
    dxl.write4ByteTxRx(portHandler, DXL_ID, ADDR_PRO_GOAL_POSITION, 120)
    if dxl_comm_result != COMM_SUCCESS:
        print("%s" % packetHandler.getTxRxResult(dxl_comm_result))
    elif dxl_error != 0:
        print("%s" % packetHandler.getRxPacketError(dxl_error))
    else:
        print("Goal position has been set: %d" % DXL_GOAL_POSITION)

    # Close the port
    portHandler.closePort()