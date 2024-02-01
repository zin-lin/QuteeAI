# Qutee_v2
Repository for the base code and design of the Qutee V2 robot


# Environment Setup
Setup your Linux environment and Docker.

## Linux
For Linux, you can choose WSL **OR** a VM

### Window Subsystem for Linux ```(WSL)```
Allows a linux distro to run directly on a Windows machine. The default distro will be ```Ubuntu```

- Step 1 - WSL install command
  - Open powershell or windows cmd in administrator mode
  - Enter the command, then restart your machine 

  ```powershell
    wsl --install
  ``` 
*For more info: https://learn.microsoft.com/en-us/windows/wsl/install*
### Virtual Machine ``(VM)``
TBC


## Docker
- Step 1 - Install Docker Desktop
  - Download docker desktop on Windows (https://www.docker.com/products/docker-desktop/)

### ``WSL ONLY``
- Step 2 - Modify Docker Settings
  - Ensure "Use the WSL 2 based engine" is checked.
  - Go to **Settings > Resources > WSL > Enable Integration**

*For more info: https://learn.microsoft.com/en-us/windows/wsl/tutorials/wsl-containers*

---


# Installing `ROS2`

- Step 1 - Set Locale
    ```shell
    locale  # check for UTF-8

    sudo apt update && sudo apt install locales
    sudo locale-gen en_US en_US.UTF-8
    sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
    export LANG=en_US.UTF-8

    locale  # verify settings
    ```
- Step 2 - Enable required respositories

    - Enable Ubuntu Universe repository
    ```shell
    sudo apt install software-properties-common
    sudo add-apt-repository universe
    ```

    - Add the ROS 2 GPG key with apt
    ```shell
    sudo apt update && sudo apt install curl -y
    sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
    ```

    - Add the repository to your sources list
    ```shell
    echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
    ```

- Step 3 - Install ROS2

    - Update your apt repository caches after setting up the repositories.
    ```shell
    sudo apt update
    ```
    - Make sure system is up to date
    ```shell
    sudo apt upgrade
    ```
    - Install ROS2
    ```shell
    sudo apt install ros-humble-desktop
    ```

- Step 4 - Setup Environment
    ```shell
    source /opt/ros/humble/setup.bash
    ```

- Step 5 - Test
    ```shell
    ros2 wtf
    ```
  -  Should say "All 5 Checks Passed"

*For more info: https://docs.ros.org/en/iron/Installation/Ubuntu-Install-Debians.html*
## Getting Access to Repository on `Linux`
- Step 1
  - In your Linux/Ubuntu VM/Ubuntu App/WSL Powershell, generate SSH key, please keep it out of the repo dir
  ```shell
  ssh-keygen
  ```

- Step 2
  - name the generated key anything you want and skip the `re-paraphrasing` bit
  - this will generate the `$name.pub` file and the `$name`, where the later is the private and the former the public key.
- Step 3
  - Request `zin-lin` on Discord and send your `$name.pub` file's content.
- Step 4
  - activating your key on Local Linux on your system.
  - ```shell
    eval "$(ssh-agent -s)"
    ssh-add path/to/your/private/file/$name
    ```
- Step 5
  - See if your key connection works
  - ```shell
    ssh git@githum.com
    ```
  - The following will be printed if it does
  - ```shell
    PTY allocation request failed on channel 0
    Hi zin-lin/QuteeAI! You've successfully authenticated, but GitHub does not provide shell access.
    Connection to github.com closed.
    ```
- Step 6 
  - Allocate where you want the project and pull
  - ```shell
    mkdir qtai
    cd qtai
    git init
    git remote add origin git@github.com:zin-lin/QuteeAI.git
    git pull origin master
    ```