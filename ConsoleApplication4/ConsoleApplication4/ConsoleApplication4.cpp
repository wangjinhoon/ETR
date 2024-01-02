#include <iostream>
#include <vector>
#include <Open3D/Open3D.h>
#include<queue>
using namespace std;

int ply2obj(const string& arg_inputPath, const string& arg_outputPath) {
    // Define input and output paths
    using namespace open3d;
    //std::string input_path = "C:\\Users\\jh813\\source\\repos\\ConsoleApplication4\\cloud5.ply";
    //std::string output_file_path = "C:\\Users\\jh813\\test.obj";

    std::string input_path = arg_inputPath;
    std::string output_file_path = arg_outputPath;


    // Load point cloud
    auto cloud_ptr = io::CreatePointCloudFromFile(input_path);
    //visualization::DrawGeometries({ cloud_ptr }, "TestPCDFileFormat", 1920, 1080); // point cloud data viewer

    // Filter PointCloud data to eliminate points where nx, ny, and nz are all zeroes.
    open3d::geometry::PointCloud pcd_filtered;

    for (int i = 0; i < cloud_ptr->points_.size(); i++) {
        Eigen::Vector3d normal = cloud_ptr->normals_[i];
        if (normal.norm() > 0.0) {
            pcd_filtered.points_.push_back(cloud_ptr->points_[i]);  //copy location information
            pcd_filtered.normals_.push_back(normal);
            pcd_filtered.colors_.push_back(cloud_ptr->colors_[i]);  // Copy color information
        }
    }
    
    pcd_filtered.EstimateNormals();


    /*Calculate the initial bounding box, including the original point 
    cloud,and use the value to filter all surfaces outside the bounding box 
    for neat results*/
    // Gets the points in the point cloud data.
    const open3d::geometry::PointCloud& point_cloud = pcd_filtered;

    // Sets the initial value.
    Eigen::Vector3d min_point = point_cloud.GetMinBound();
    Eigen::Vector3d max_point = point_cloud.GetMaxBound();

    // print result
    //std::cout << "min point: " << min_point.transpose() << std::endl;
    //std::cout << "max point: " << max_point.transpose() << std::endl;


   //open3d::io::WritePointCloud("cleaned_point_cloud.ply", pcd_filtered); 

    // Perform Poisson surface reconstruction
    int depth = 8;
    float width = 0.0f;
    float scale = 1.1f;
    bool linear_fit = false;
    /*std::tuple<std::shared_ptr<TriangleMesh>, std::vector<double>>
      TriangleMesh::CreateFromPointCloudPoisson(const PointCloud& pcd,
                                          size_t depth,
                                          float width,
                                          float scale,
                                          bool linear_fit,
                                          int n_threads)*/
    auto poisson_mesh = open3d::geometry::TriangleMesh::CreateFromPointCloudPoisson(pcd_filtered, depth, width, scale, linear_fit);

    //Output is shared_ptr<TriangleMesh>, std::vector<double>, so only shared_ptr information is extracted as a get function
    auto crop_before_poisson_mesh = std::get<std::shared_ptr<open3d::geometry::TriangleMesh>>(poisson_mesh);
    
    // Defines the bounding box to be cropped.
    open3d::geometry::AxisAlignedBoundingBox crop_bbox(min_point, max_point);

    // Create a cropped mesh.
    auto cropped_mesh = crop_before_poisson_mesh->Crop(crop_bbox);


    //save masy to path 
    if (io::WriteTriangleMesh(output_file_path, *cropped_mesh)) {
        std::cout << "Mesh saved to " << output_file_path << std::endl; 
    }

    return 0;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "사용법: " <<  "파일명, 입력경로, 출력경로" << std::endl;
        return 1;
    }
    std::string inputPath = argv[1];
    std::string outputPath = argv[2];
    std::cout  << inputPath << std::endl;
    std::cout  << outputPath << std::endl;

    ply2obj(inputPath, outputPath);
}
