#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.hpp>

class ImageViewerNode : public rclcpp::Node
{
public:
  ImageViewerNode() : Node("image_viewer_node")
  {
    // 声明并获取参数
    this->declare_parameter<std::string>("image_topic", "/camera/image_raw");
    std::string topic = this->get_parameter("image_topic").as_string();

    RCLCPP_INFO(this->get_logger(), "订阅图像话题: %s", topic.c_str());

    using std::placeholders::_1;
    image_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
      topic, 10, std::bind(&ImageViewerNode::image_callback, this, _1));
  }

private:
  void image_callback(const sensor_msgs::msg::Image::SharedPtr msg)
  {
    try {
      cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
      cv::imshow("Image Viewer", cv_ptr->image);
      cv::waitKey(1);  // 必须调用，否则窗口不会刷新
    } catch (cv_bridge::Exception& e) {
      RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
    }
  }

  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_sub_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ImageViewerNode>());
  rclcpp::shutdown();
  return 0;
}
