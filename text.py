import open3d as o3d
import numpy as np
input_path="C:/Users/jh813/rgbd-dataset/apple/apple_1/"
output_path="C:/Users/jh813/rgbd-dataset/apple/apple_1/"
dataname="path_to_output_text.ply"

pcd = o3d.io.read_point_cloud(input_path+dataname)
downpcd = pcd.voxel_down_sample(voxel_size=0.03)
distances = downpcd.compute_nearest_neighbor_distance()
avg_dist = np.mean(distances)
radius = 8 * avg_dist
bpa_mesh = o3d.geometry.TriangleMesh.create_from_point_cloud_ball_pivoting(downpcd,o3d.utility.DoubleVector([radius, radius * 2]))
bpa_mesh =  bpa_mesh.filter_smooth_laplacian()

o3d.visualization.draw_geometries([bpa_mesh])