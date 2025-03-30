#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

class CameraPublisher : public rclcpp::Node
{
public:
  CameraPublisher(): Node("camera_publisher")
  {
    // 声明并获取参数
    this->declare_parameter<std::string>("image_topic", "/camera/image_raw");
    std::string topic = this->get_parameter("image_topic").as_string();

    publisher_ = this->create_publisher<sensor_msgs::msg::Image>(topic, 100);
    timer_ = this->create_wall_timer(std::chrono::milliseconds(30), std::bind(&CameraPublisher::publish_frame, this));

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

    std_msgs::msg::Header header;
    header.stamp = this->now();
    header.frame_id = "camera_frame";

    sensor_msgs::msg::Image::SharedPtr msg = cv_bridge::CvImage(header, "bgra8", rgba_frame).toImageMsg();
    publisher_->publish(*msg);
  }

  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  cv::VideoCapture cap_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CameraPublisher>());
  rclcpp::shutdown();
  return 0;
}
