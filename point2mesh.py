# import open3d as o3d
# import numpy as np
# import os
# # input_path="C:/Users/jh813/rgbd-dataset/apple/apple_1/"
# obj_file_path="C:/Users/jh813/rgbd-dataset/apple/apple_1/output.obj"
# # dataname="path_to_output_text.ply"

# input_path ="C:/Users/jh813/Downloads/"
# dataname = "point_cloud.ply"
# # file_names = [f for f in os.listdir(input_path) if os.path.isfile(os.path.join(input_path, f))]
# # combined_pcd = o3d.geometry.PointCloud()


# # for dataname in file_names:
# #     pcd = o3d.io.read_point_cloud(input_path+dataname)
# #     combined_pcd += pcd 

# pcd = o3d.io.read_point_cloud(input_path+dataname)
# pcd_cleaned, ind = pcd.remove_radius_outlier(nb_points=10, radius=0.5)

# downpcd = pcd_cleaned.voxel_down_sample(voxel_size=0.03)
# distances = downpcd.compute_nearest_neighbor_distance()
# avg_dist = np.mean(distances)
# radius = 8 * avg_dist
# # bpa_mesh = o3d.geometry.TriangleMesh.create_from_point_cloud_ball_pivoting(downpcd,o3d.utility.DoubleVector([radius, radius * 2]))
# bpa_mesh, densities = o3d.geometry.TriangleMesh.create_from_point_cloud_poisson(downpcd, depth=9)
# # bpa_mesh =  bpa_mesh.filter_smooth_laplacian()

# o3d.visualization.draw_geometries([bpa_mesh])
# # o3d.io.write_triangle_mesh(obj_file_path, bpa_mesh)
# # print("start")
# # o3d.visualization.draw_geometries([pcd_cleaned])
import argparse
import open3d as o3d
import pandas as pd
import meshio
import numpy as np
import pyvista as pv
import os
input_path ="C:/Users/jh813/Downloads/"
dataname = "point_cloud.ply"
def points_to_mesh():
    
    ## Import point cloud
    pcd = meshio.read(input_path+dataname)
    
    ## Grabs all of the points that make up the point cloud and cretes a df from the array
    points = pcd.points
    points_df = pd.DataFrame(points, columns=['x', 'y', 'z'])
    
    ## Performs a transformation of each point to scale it
    points_df['x'] = points_df['x']
    points_df['y'] = points_df['y']
    points_df['z'] = points_df['z']
    
    ## Converts the new df to an array
    new_points = pd.DataFrame(points_df).to_numpy()
    
    ## Creates PolyData, 3D represtnation and a final mesh
    poly = pv.PolyData(new_points)
    shell = poly.delaunay_3d(alpha=0.02, progress_bar=True)
    shell_geo = shell.extract_geometry().triangulate()
    # o3d.visualization.draw_geometries([shell_geo])
    shell_geo.save(os.path.join("C:/Users/jh813/Downloads/", 'mesh.ply'))
#----------------------------------

def main():
    print("start")
    points_to_mesh()
    


if __name__ == '__main__':
    main()