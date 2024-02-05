from dynamixel_sdk import *
from controls import *

# simpleexamples.motormove.move_mortor()

simpleexamples.imu_data_msg.main()
while True:
    print('Command')
    cmd = input('type wsad:')
    if cmd == 'w':
        controls.w()
    elif cmd =='d':
        controls.d()
    elif cmd == 's':
        controls.s()
    elif cmd == 'a':
        controls.a()
    else:
        print('illegitimate')

