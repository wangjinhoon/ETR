#include <iostream>
#include <vector>
#include <Open3D/Open3D.h>
#include<queue>
using namespace std;

void PaintMesh(open3d::geometry::TriangleMesh& mesh,
    const Eigen::Vector3d& color) {
    mesh.vertex_colors_.resize(mesh.vertices_.size());
    for (size_t i = 0; i < mesh.vertices_.size(); i++) {
        mesh.vertex_colors_[i] = color;
    }
}

int main() {
    // Define input and output paths
    using namespace open3d;
    std::string input_path = "C:\\Users\\jh813\\source\\repos\\ConsoleApplication4\\test.ply";
    std::string output_file_path = "C:\\Users\\jh813\\test.obj";



    auto cloud_ptr = io::CreatePointCloudFromFile(input_path);
    // Load point cloud
    //visualization::DrawGeometries({ cloud_ptr }, "TestPCDFileFormat", 1920, 1080);

    //// Perform Poisson surface reconstruction
    int depth = 10;
    float width = 0.0f;
    float scale = 1.1f;
    bool linear_fit = false;
    auto result = open3d::geometry::TriangleMesh::CreateFromPointCloudPoisson(*cloud_ptr, depth, width, scale, linear_fit);

    auto mesh_ptr = std::get<std::shared_ptr<open3d::geometry::TriangleMesh>>(result);

    // Create a vector of shared pointers to geometries


    if (io::WriteTriangleMesh(output_file_path, *mesh_ptr)) {
        std::cout << "Mesh saved to " << output_file_path << std::endl;
    }

    return 0;
}
