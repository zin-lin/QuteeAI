import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Imu
from Adafruit_BNO055 import BNO055  # Import the correct library for your Adafruit BNO055 sensor

class IMUNode(Node):
    def __init__(self):
        super().__init__('imu_node')
        self.publisher_ = self.create_publisher(Imu, 'imu_data', 10)
        self.bno = BNO055.BNO055()  # Initialize the BNO055 sensor

    def read_imu_data(self):
        # Read accelerometer, gyroscope, and magnetometer data
        accel_x, accel_y, accel_z = self.bno.read_linear_acceleration()
        gyro_x, gyro_y, gyro_z = self.bno.read_gyroscope()
        mag_x, mag_y, mag_z = self.bno.read_magnetometer()

        # Publish IMU data as ROS 2 message
        imu_msg = Imu()
        imu_msg.linear_acceleration.x = accel_x
        imu_msg.linear_acceleration.y = accel_y
        imu_msg.linear_acceleration.z = accel_z
        imu_msg.angular_velocity.x = gyro_x
        imu_msg.angular_velocity.y = gyro_y
        imu_msg.angular_velocity.z = gyro_z
        imu_msg.orientation.x = mag_x
        imu_msg.orientation.y = mag_y
        imu_msg.orientation.z = mag_z
        self.publisher_.publish(imu_msg)

def main(args=None):
    rclpy.init(args=args)
    imu_node = IMUNode()
    while rclpy.ok():
        imu_node.read_imu_data()

    imu_node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
