from dynamixel_sdk import *
import dynamixel_sdk as dxl
import time
# Set the port and baudrate
DEVICENAME = '/dev/ttyUSB0'  # Modify this according to your setup
BAUDRATE = 57600  # Modify this according to your Dynamixel configuration
# zin


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
DXL_ID = 1 # Modify this according to your motor ID
DXL_GOAL_POSITION = 512  # Modify this according to your goal position





def leg1_2_3(val):
    # Enable torque
    ids = [41, 42, 43]
    for DXL_ID in ids:
        dxl_comm_result, dxl_error = packetHandler.write1ByteTxRx(portHandler, DXL_ID, 64, 1)
        if dxl_comm_result != COMM_SUCCESS:
            print("TX is :: " + str(dxl_comm_result) + " %s" % packetHandler.getTxRxResult(dxl_comm_result))
        elif dxl_error != 0:
            print("Error:: %s" % packetHandler.getRxPacketError(dxl_error))
        else:
            print("Torque enabled")

        # Write goal position
        dxl_comm_result, dxl_error = packetHandler.write4ByteTxRx(portHandler, DXL_ID, 116, val)
        if dxl_comm_result != COMM_SUCCESS:
            print("TX is :: " + str(dxl_comm_result) + " %s" % packetHandler.getTxRxResult(dxl_comm_result))
        elif dxl_error != 0:
            print("Error:: %s" % packetHandler.getRxPacketError(dxl_error))
        else:
            print("Goal position set to: "+ str(val) +" neutral")


def leg(id, val):
    # Enable torque
    ids = [id]
    for DXL_ID in ids:
        dxl_comm_result, dxl_error = packetHandler.write1ByteTxRx(portHandler, DXL_ID, 64, 1)
        if dxl_comm_result != COMM_SUCCESS:
            print("TX is :: " + str(dxl_comm_result) + " %s" % packetHandler.getTxRxResult(dxl_comm_result))
        elif dxl_error != 0:
            print("Error:: %s" % packetHandler.getRxPacketError(dxl_error))
        else:
            print("Torque enabled")

        # Write goal position
        dxl_comm_result, dxl_error = packetHandler.write4ByteTxRx(portHandler, DXL_ID, 116, val)
        if dxl_comm_result != COMM_SUCCESS:
            print("TX is :: " + str(dxl_comm_result) + " %s" % packetHandler.getTxRxResult(dxl_comm_result))
        elif dxl_error != 0:
            print("Error:: %s" % packetHandler.getRxPacketError(dxl_error))
        else:
            print("Goal position set to 0: neutral")

# count = 1
# while(True):
#     if count%2 == 0:
#         leg1_2_3(2000)
#     else:
#         leg1_2_3(0)
#     count+=1

#leg1_2_3(0)

leg(33, 0)
# Close port
portHandler.closePort()