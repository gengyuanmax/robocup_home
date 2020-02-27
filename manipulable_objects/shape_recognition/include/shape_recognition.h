/*
 * ShapeRecognition.h
 *
 *  Created on: 07-2019
 *      Author: luz.martinez@tum.de
 */

#ifndef SHAPERECOGNITION_H_
#define SHAPERECOGNITION_H_

// C, C++
#include <iostream>
#include <string>
#include <cmath>
#include <limits>
#include <map>
#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

// OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

// ROS
#include <ros/ros.h>
 #include <ros/package.h>
#include <tf/transform_listener.h>
#include <pcl_ros/transforms.h>
#include <pcl_ros/point_cloud.h>
#include <visualization_msgs/Marker.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>


// pcl
#include <pcl/point_cloud.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/point_types.h>
#include <pcl/impl/point_types.hpp>
#include <pcl/conversions.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_types_conversion.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/pcl_base.h>
#include <pcl/PointIndices.h>

// #include <pcl/filters/filter.h>
// #include <pcl/filters/project_inliers.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/passthrough.h>
// #include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/filter_indices.h>
// #include <pcl/kdtree/kdtree.h>
// #include <pcl/sample_consensus/ransac.h>
#include <pcl/sample_consensus/sac_model_line.h>
// #include <pcl/sample_consensus/method_types.h>
// #include <pcl/sample_consensus/model_types.h>
#include <pcl/search/kdtree.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
// #include <pcl/surface/concave_hull.h>
// #include <pcl/surface/convex_hull.h>

#include <object_pose_estimation/SQFitting.h>
#include <object_pose_estimation/SQTypes.h>
#include <object_pose_estimation/ObjectPoseEstimator.h>
#include <sample_superquadric.h>

// srvs and msgs
#include <rhome_srvs/Onoff.h>
#include <robmosys_srvs/objectinformation.h>
#include <geometry_msgs/Pose.h>


namespace rhome_perception {

typedef pcl::PointXYZ Point;
typedef pcl::PointCloud<Point> PointCloud;


using std::string;

class ShapeRecognition  {


private:
	 ros::NodeHandle priv;
	std::string _name;
	bool _enabled;

	// - - - - - Parameters - - - - - - -
	std::string _base_frame;
	std::string _rgbd_frame;
	std::string _rgb_frame;
	std::string _depth_frame;
	std::string _rgb_topic;
	std::string _depth_topic;
	std::string _point_topic;
	std::string _imagePoint_topic;
	bool _visualize;

	ros::Time lasttime_transform;
	std::vector<geometry_msgs::Point> sphere_list;
	std::vector<geometry_msgs::Point> cube_list;
	std::vector<geometry_msgs::Point> cylinder_list;

	float _crop_width;
	float _crop_depth;
	float _crop_min_z;
	float _crop_max_z;
	float _leaf_size;

	// publishers
	image_transport::Publisher _mask_pub;

	//Subscribers 
	ros::Subscriber _subs_rgb;
	ros::Subscriber _subs_depth;
	ros::Subscriber _subs_point;

	pcl::PCLPointCloud2::ConstPtr cloud_in;
	sensor_msgs::ImageConstPtr rgb_in;
	sensor_msgs::ImageConstPtr depth_in;
	std::string image_frameid;


	// services
    ros::ServiceServer _active_server;
    ros::ServiceServer _getobjects_server;

	// Listeners
	ros::Subscriber _depth_sub;
	tf::TransformListener *_tf_listener;
	tf::TransformListener tf_listener;
	// pcl
	pcl::VoxelGrid<Point> sor;


public:
	bool _is_on;
	bool ready_rgb;
	bool ready_depth;
	bool ready_point;

	ShapeRecognition(std::string name);
	virtual ~ShapeRecognition();

	void run();


private:

	// - - - - - - S u b s c r i b e r   C a l l b a c k s  - - - - - - - - -
	void _process_point(const pcl::PCLPointCloud2::ConstPtr &point_cloud_in);
	void _process_depth(const sensor_msgs::ImageConstPtr& img);
	void _process_rgb(const sensor_msgs::ImageConstPtr& img);

	// - - - - - - - - - - - - S e r v i c e s - - - - - - - - -
	bool _active_service(rhome_srvs::Onoff::Request  &req, rhome_srvs::Onoff::Response &res) ;
	bool getobjects_service(robmosys_srvs::objectinformation::Request  &req, robmosys_srvs::objectinformation::Response &res);

	// - - - - -  Functions  - - - - - - - - - -
	cv::Point point3d_to_point2d(geometry_msgs::Point pt_in);
	void downsample_pointcloud(const PointCloud::Ptr cloud_in, PointCloud::Ptr cloud_out);
	bool transform_pointcloud(const PointCloud::Ptr cloud_in, PointCloud::Ptr cloud_out, std::string target_frame);

	void filter_voxel(PointCloud::Ptr  cloud_in, PointCloud::Ptr cloud_out);
	void filter_passthrough(PointCloud cloud_in, PointCloud::Ptr cloud_out);
	void extract_planar(PointCloud::Ptr  cloud_in, PointCloud::Ptr cloud_out);
	std::vector<pcl::PointCloud<pcl::PointXYZ>::Ptr> get_clusters(PointCloud::Ptr  cloud_in);
	cv::Rect get_rect(pcl::PointCloud<pcl::PointXYZ>::Ptr obj);
	int quadratic_form_matcher(const ope::SQParameters& param, const std::string& frame_id);
	int quadratic_matcher(pcl::PointCloud<pcl::PointXYZ>::Ptr& obj, const std::string& frame_id);

};

} /* namespace  */
#endif /* ShapeRecognition_H_ */


