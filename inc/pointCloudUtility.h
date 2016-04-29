#ifndef POINT_CLOUD_UTILITY_H
#define POINT_CLOUD_UTILITY_H

#include <opencv2/core/core.hpp>

// Computes the normals of the point cloud within the roi specified by taking the cross product of the lines connecting 
// nearest neighbouring points to the point of interest (N->S, E->W). The cross product is normalised. 
//
// The ROI is there as convenient way of dealing with the point cloud matrix boundaries as we are not interested in these values.
// As such the roi must have at least a one pixel boundary to the point cloud. Points wihin the skipped border have their normal values
// set to look down the postive z-axis, i.e. away from the opengl viewport. 
//
// Optionally you can provide a string to save the normals to a location of choice. Default is empty string meaning nothing is saved. 
cv::Mat computeNormals(const cv::Mat& point_cloud, const cv::Rect& ROI, const std::string& savePathFile = std::string());




#endif 