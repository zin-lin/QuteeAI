# zin
from dynamixel_sdk import *
import dynamixel_sdk as dx1

### PYTHON SECTION FOR CONTROL SET UP

# Set the port and baudrate
DEVICENAME = '/dev/ttyUSB0'  # Modify this according to your setup
BAUDRATE = 57600  # Modify this according to your Dynamixel configuration
# Control table addresses
OG_POS_1 = 228
OG_POS_2 = 1138
ADDR_PRO_MOVING_SPEED = 32
ADDR_START = 116
ADDR_END = 43
ADDR_MAX_POS = 48
TORQUE_ENABLE = 1  # Value for enabling the torque
ADDR_PRO_TORQUE_ENABLE = 1
NEW_POS_1 = 683
NEW_POS_2 = 683
NEW_BACK_POS_1 = 683
NEW_BACK_POS_2 = 1593
# Define protocol version
PROTOCOL_VERSION = 2.0

# Initialize PortHandler instance
portHandler = PortHandler(DEVICENAME)

# Initialize PacketHandler instance
packetHandler = PacketHandler(PROTOCOL_VERSION)
# Open port
if portHandler.openPort():
    print("Succeeded to open the port")
else:
    print("Failed to open the port")
    quit()


# connection control
if portHandler.setBaudRate(BAUDRATE):
    print("Succeeded to change the baudrate")
else:
    print("Failed to change the baudrate")
    exit(1)

d1 = 1
d2 = 2
d3 = 3
d4 = 4
d5 = 5
d6 = 6
d7 = 7
d8 = 8
array_id = [d1, d2, d3, d4, d5, d6, d7, d8]

### END SECTION

##
## Movement will go with 2-2-4

# reset position
def _reset_():
    # For root mortors
    for i in range(4):

        # Set enable torque to move around the servos
        dxl.write1ByteTxRx(portHandler, array_id[i], ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE)
        # set max value
        dxl.write2ByteTxRx(portHandler, array_id[i], ADDR_MAX_POS, 1600)

        # Set goal position
        dxl.write4ByteTxRx(portHandler, array_id[i], ADDR_START, OG_POS_1)

    # For adjacent mortors
    for i in range(4,8):
        # Set enable torque to move around the servos
        dxl.write1ByteTxRx(portHandler, array_id[i], ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE)
        # set max value
        dxl.write2ByteTxRx(portHandler, array_id[i], ADDR_MAX_POS, 1600)
        # Set goal position
        dxl.write4ByteTxRx(portHandler, array_id[i], ADDR_START, OG_POS_2)

def _reset1_ (id):
    mid = array_id[id-1]
    # Set enable torque to move around the servos
    dxl.write1ByteTxRx(portHandler, mid, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE)

    # Set goal position
    dxl.write4ByteTxRx(portHandler, mid, ADDR_START, OG_POS_1)

    mid = array_id[id - 1] + 4
    dxl.write1ByteTxRx(portHandler,mid, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE)

    # Set goal position
    dxl.write4ByteTxRx(portHandler, mid, ADDR_START, OG_POS_2)


def flash():
    reset()
    print()

# for one leg when pressed w
def _w1_(id):


    mid = array_id[id-1]
    dxl.write1ByteTxRx(portHandler, mid, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE)
    # Set goal position
    dxl.write4ByteTxRx(portHandler, ADDR_START,  NEW_POS_1)

    mid = array_id[id-1] +4
    dxl.write1ByteTxRx(portHandler, mid, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE)
    # Set goal position
    dxl.write4ByteTxRx(portHandler, mid,ADDR_START,  NEW_POS_2)
    print()

# for one leg when pressed s
def _s1_():

    mid = array_id[id-1]
    dxl.write1ByteTxRx(portHandler, mid, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE)
    # Set goal position
    dxl.write4ByteTxRx(portHandler, ADDR_START,  NEW_POS_1)

    mid = array_id[id-1] +4
    dxl.write1ByteTxRx(portHandler, mid, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE)
    # Set goal position
    dxl.write4ByteTxRx(portHandler, mid,ADDR_START,  NEW_POS_2)
    print()


# for one leg when pressed a
def _a1_():
    mid = array_id[id-1]
    dxl.write1ByteTxRx(portHandler, mid, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE)
    # Set goal position
    dxl.write4ByteTxRx(portHandler, ADDR_START,  NEW_POS_1)

    mid = array_id[id-1] +4
    dxl.write1ByteTxRx(portHandler, mid, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE)
    # Set goal position
    dxl.write4ByteTxRx(portHandler, mid,ADDR_START,  NEW_POS_2)
    print()

    print()

# for one leg when pressed d
def _d1_():
    print()

# for one leg when pressed w reposition
def _w2_():

    mid = array_id[id-1]
    dxl.write1ByteTxRx(portHandler, mid, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE)
    # Set goal position
    dxl.write4ByteTxRx(portHandler, ADDR_START,  NEW_BACK_POS_1)

    mid = array_id[id-1]
    dxl.write1ByteTxRx(portHandler, mid, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE)
    # Set goal position
    dxl.write4ByteTxRx(portHandler, mid, ADDR_START,  NEW_BACK_POS_2)
    print()


# for one leg when pressed s reposition
def _s2_():
    print()

# for one leg when pressed a reposition
def _a2_():
    print()

# for one leg when pressed d reposition
def _d2_():
    print()


# for all legs when pressed w
def w ():
    threads = []
    # 2 of 2-2-4
    for i in range (2):
        id = i + 1
        thread = threading.Thread(target=_w1_, args=( id))
        threads.append(thread)
        thread.start()

    # Wait for all threads to complete
    for thread in threads:
        thread.join()

    threads.clear()

    # 2 of 2-2-4
    for i in range (2,4):
        id = i + 1
        thread = threading.Thread(target=_w2_, args=( id ))
        threads.append(thread)
        thread.start()

    # Wait for all threads to complete
    for thread in threads:
        thread.join()

    threads.clear()

    # 4 of 2-2-4
    for i in range (4):
        id = 1+ i
        thread = threading.Thread(target=_reset1_, args=(id))
        threads.append(thread)
        thread.start()

    # Wait for all threads to complete
    for thread in threads:
        thread.join()

    threads.clear()

    print("Moved forward")

# for all legs when pressed s
def s ():
    threads = []


    # 2 of 2-2-4
    for i in range(2, 4):
        id = i + 1
        thread = threading.Thread(target=_w1_, args=(id))
        threads.append(thread)
        thread.start()

        # Wait for all threads to complete
    for thread in threads:
        thread.join()

    # 2 of 2-2-4
    for i in range(2):
        id = i + 1
        thread = threading.Thread(target=_w2_, args=(id))
        threads.append(thread)
        thread.start()


    # Wait for all threads to complete
    for thread in threads:
        thread.join()

    threads.clear()

    # 4 of 2-2-4
    for i in range(4):
        id = 1 + i
        thread = threading.Thread(target=_reset1_, args=(id))
        threads.append(thread)
        thread.start()

    # Wait for all threads to complete
    for thread in threads:
        thread.join()

    threads.clear()

    print("Moved Backward")

# for all legs when pressed a
def a ():
    threads = []
    # 2 of 2-2-4
    for i in range(3):
        if i == 1:
            continue # skip loop
        id = i + 1
        thread = threading.Thread(target=_w1_, args=(id))
        threads.append(thread)
        thread.start()

    # Wait for all threads to complete
    for thread in threads:
        thread.join()

    threads.clear()

    # 2 of 2-2-4
    for i in range(1,4):
        if i == 2:
            continue

        id = i + 1
        thread = threading.Thread(target=_w2_, args=(id))
        threads.append(thread)
        thread.start()

    # Wait for all threads to complete
    for thread in threads:
        thread.join()

    threads.clear()

    # 4 of 2-2-4
    for i in range(4):
        id = 1 + i
        thread = threading.Thread(target=_reset1_, args=(id))
        threads.append(thread)
        thread.start()

    # Wait for all threads to complete
    for thread in threads:
        thread.join()

    threads.clear()

    print("Moved left")

# for all legs when pressed d
def d ():
    threads = []
    # 2 of 2-2-4
    for i in range(1, 4):
        if i == 2:
            continue  # skip loop
        id = i + 1
        thread = threading.Thread(target=_w1_, args=(id))
        threads.append(thread)
        thread.start()

    # Wait for all threads to complete
    for thread in threads:
        thread.join()

    threads.clear()

    # 2 of 2-2-4
    for i in range(3):
        if i == 1:
            continue

        id = i + 1
        thread = threading.Thread(target=_w2_, args=(id))
        threads.append(thread)
        thread.start()

    # Wait for all threads to complete
    for thread in threads:
        thread.join()

    threads.clear()

    # 4 of 2-2-4
    for i in range(4):
        id = 1 + i
        thread = threading.Thread(target=_reset1_, args=(id))
        threads.append(thread)
        thread.start()

    # Wait for all threads to complete
    for thread in threads:
        thread.join()

    threads.clear()

    print("Moved left")




