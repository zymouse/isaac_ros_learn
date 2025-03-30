## Manual Installation - zero-copy

```bash
agnocast_version="1.0.2"

sudo add-apt-repository -y ppa:t4-system-software/agnocast
sudo apt update
sudo apt install -y "agnocast-heaphook=${agnocast_version}*"

if dkms status | grep agnocast | grep -q "${agnocast_version}"; then
    echo "agnocast-kmod version ${agnocast_version} is already registered in dkms. Skipping purge and install."
else
    sudo apt purge -y "agnocast-kmod=${agnocast_version}*"
    sudo apt install -y "agnocast-kmod=${agnocast_version}*"
fi

```

## 使用稳定
### step-1: 下载编译
```bash
git clone -b feature/agnocast/zero-copy-demo https://github.com/zymouse/isaac_ros_learn.git
cd isaac_ros_learn
colcon build --cmake-args -DCMAKE_BUILD_TYPE=Release
```

### step-2: 开始使用

```bash
# 插入内核模块。
sudo modprobe agnocast

# 启动相机发布程序 -- 零拷贝
ros2 launch agnocast_image_test zero.launch.xml

# 或者 启动图像显示程序 -- 没有零拷贝
ros2 launch agnocast_image_test non_zero.launch.xml

# 停止应用程序并卸载内核模块
sudo modprobe -r agnocast
```