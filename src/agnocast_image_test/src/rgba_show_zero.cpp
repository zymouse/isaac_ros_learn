#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.hpp>
#include "agnocast/agnocast.hpp"

using std::placeholders::_1;
namespace zero{

class ImageViewerNode : public rclcpp::Node
{
public:
  ImageViewerNode(const rclcpp::NodeOptions & options) : Node("image_viewer_node", options)
  {
    // 声明并获取参数
    this->declare_parameter<std::string>("image_topic", "/camera/image_raw");
    std::string topic = this->get_parameter("image_topic").as_string();

    RCLCPP_INFO(this->get_logger(), "订阅图像话题: %s", topic.c_str());
    rclcpp::CallbackGroup::SharedPtr group =
      create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    agnocast::SubscriptionOptions agnocast_options;
    agnocast_options.callback_group = group;

    image_sub_ = agnocast::create_subscription<sensor_msgs::msg::Image>(
      this, topic, 100, std::bind(&ImageViewerNode::image_callback, this, _1), agnocast_options);
  }

private:
  void image_callback(const agnocast::ipc_shared_ptr<sensor_msgs::msg::Image> & msg)
  {
    try {
      // 检查格式（例如我们现在处理的是 "rgba8"）
      if (msg->encoding != "rgba8") {
        RCLCPP_WARN(this->get_logger(), "图像编码格式不是 rgba8, 实际是: %s", msg->encoding.c_str());
        return;
      }

      // 构造 Mat，注意：不要拷贝数据，直接引用 msg->data 内存
      cv::Mat rgba(msg->height, msg->width, CV_8UC4, const_cast<uint8_t*>(msg->data.data()));

      // 如果你想直接显示 rgba，可以这样（OpenCV 有 alpha channel 也能显示）
      cv::imshow("Image Viewer - RGBA", rgba);
      cv::waitKey(1);

    } catch (cv_bridge::Exception& e) {
      RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
    }
  }

  agnocast::Subscription<sensor_msgs::msg::Image>::SharedPtr image_sub_;

};
  
};

#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(zero::ImageViewerNode)