# Qutee_v2
Repository for the base code and design of the Qutee V2 robot


# Installing `ROS2`

- Step 1 - Set Locale
    ```powershell
    locale  # check for UTF-8

    sudo apt update && sudo apt install locales
    sudo locale-gen en_US en_US.UTF-8
    sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
    export LANG=en_US.UTF-8

    locale  # verify settings
    ```
- Step 2 - Enable required respositories

    Enable Ubuntu Universe repository
    ```powershell
    sudo apt install software-properties-common
    sudo add-apt-repository universe
    ```

    Add the ROS 2 GPG key with apt
    ```powershell
    sudo apt update && sudo apt install curl -y
    sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
    ```

    Add the repository to your sources list
    ```powershell
    echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
    ```

- Step 3 - Install ROS2

    Update your apt repository caches after setting up the repositories.
    ```powershell
    sudo apt update
    ```
    Make sure system is up to date
    ```powershell
    sudo apt upgrade
    ```
    Install ROS2
    ```powershell
    sudo apt install ros-iron-desktop
    ```

- Step 4 - Setup Environment
    ```powershell
    source /opt/ros/iron/setup.bash
    ```

- Step 5 - Test
    ```powershell
    ros2 wtf
    ```
    Should say "All 5 Checks Passed"