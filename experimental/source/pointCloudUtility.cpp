#include <pointCloudUtility.h>

cv::Mat computeNormals(const cv::Mat& point_cloud, const cv::Rect& roi, const std::string& path)
{
	int rows = point_cloud.rows;
	int cols = point_cloud.cols;

	cv::Mat retval(rows, cols, CV_32FC3, cv::Scalar(0,0,1)); 

	for(int j = roi.y ; j < roi.height; ++j){
		for(int i = roi.x; i < roi.width; ++i){
			cv::Vec3f p1 = point_cloud.at<cv::Vec3f> (j, i - 1); 
			cv::Vec3f p2 = point_cloud.at<cv::Vec3f> (j - 1, i);
			cv::Vec3f p3 = point_cloud.at<cv::Vec3f> (j, i + 1);
			cv::Vec3f p4 = point_cloud.at<cv::Vec3f> (j + 1, i);

			cv::Vec3f eastWest		= p3 - p1;
			cv::Vec3f northSouth	= p2 - p4;
		
			retval.at<cv::Vec3f>(j, i) = cv::normalize(eastWest.cross(northSouth));
		}
	}

	//separate save from computation for performance??
	if (!path.empty()) {
		FILE* fp = fopen(path.c_str(), "wt");
		for (int y = 0; y < rows; y++){
			for (int x = 0; x < cols; x++){
				cv::Vec3f point = retval.at<cv::Vec3f>(y, x);	
				fprintf(fp, "%f %f %f\n", point[0], point[1], point[2]);
			}
		}
		fclose(fp);
	}

	return retval;
}