#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include "agnocast/agnocast.hpp"

class CameraPublisher : public rclcpp::Node
{
public:
  CameraPublisher(): Node("camera_publisher")
  {
    // 声明并获取参数
    this->declare_parameter<std::string>("image_topic", "/camera/image_raw");
    std::string topic = this->get_parameter("image_topic").as_string();

    double publish_rate_hz = 15.0;  // 想发布多少 Hz，自由设定
    auto interval = std::chrono::duration<double>(1.0 / publish_rate_hz);

    publisher_ = agnocast::create_publisher<sensor_msgs::msg::Image>(this, topic, 10);
    timer_ = this->create_wall_timer(interval, std::bind(&CameraPublisher::publish_frame, this));

    cap_.open("/dev/video0");
    if (!cap_.isOpened()) {
      RCLCPP_ERROR(this->get_logger(), "无法打开摄像头 /dev/video0");
      rclcpp::shutdown();
    } else {
      RCLCPP_INFO(this->get_logger(), "成功打开摄像头 /dev/video0");
    }
}

private:
  void publish_frame()
  {
    cv::Mat frame;
    cap_ >> frame;
    if (frame.empty()) {
      RCLCPP_WARN(this->get_logger(), "获取帧失败");
      return;
    }

    cv::Mat rgba_frame;
    cv::cvtColor(frame, rgba_frame, cv::COLOR_BGR2RGBA);

    // 借用 loaned message
    agnocast::ipc_shared_ptr<sensor_msgs::msg::Image> message = publisher_->borrow_loaned_message();

    // 填充 Header
    message->header.stamp = this->now();
    message->header.frame_id = "camera_frame";

    // 填充图像信息
    message->height = static_cast<uint32_t>(rgba_frame.rows);
    message->width = static_cast<uint32_t>(rgba_frame.cols);
    message->encoding = "rgba8";
    message->is_bigendian = false;
    message->step = static_cast<uint32_t>(rgba_frame.step);

    // 拷贝图像数据
    size_t size = rgba_frame.total() * rgba_frame.elemSize();
    message->data.resize(size);
    std::memcpy(message->data.data(), rgba_frame.data, size);
    publisher_->publish(std::move(message));
  }

  agnocast::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  cv::VideoCapture cap_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  agnocast::SingleThreadedAgnocastExecutor executor;
  executor.add_node(std::make_shared<CameraPublisher>());
  executor.spin();

  rclcpp::shutdown();
  return 0;
}
