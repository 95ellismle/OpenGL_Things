# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a script to create the points on a sphere and return them in a form
OpenGL can understand

 Equation for sphere:
    r^2 = (x - x1)^2 + (y - y1)^2 + (z - z1)^2

 In spherical coordinates this is:
     r = R
 
 To convert from spherical to cartesian we can use:
      x = R sinφ cosθ
      y = R sinφ sinθ
      z = R cosφ
     

 To get evenly spaced points on a cirlce the points can be sampled
  at regular angle intervals.
 To do this divide 2pi by the number of points and use this as a regular
  angle interval for φ and θ.
"""
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


class Shape(object):
    """
    A base class for building shapes.
    
    Important attributes:
        * xarr, yarr, zarr <list> => The x, y, z vertex coords respectively.
    """
    def __init__(self):
        self.xyz = []
        self.indices = []
        
        # Set up the vertices and indices creating the triangles
        self.create_vertices()
        self.create_indices()
        
        # Convert to a nicer format and set tiny values to zero.
        self.xyz = np.array(self.xyz)
        
    def create_vertices(self): pass
    def create_indices(self): pass

    def write_data(self, vert_filepath=False, ind_filepath=False):
        """
        Write vertices and/or indices as an .arr file 
        
        This filepath can be read by the OpenGL code.

        Parameters
        ----------
        vert_filepath : str, optional
            Where to write the vertices. The default is False.
        ind_filepath : str, optional
            Where to write the indices. The default is False.

        Returns
        -------
        None.
        """
        if vert_filepath is not False:
            vert_str = self.xyz
            vert_str = np.apply_along_axis(lambda i: ' '.join(["%.5f" % j for j in i]).ljust(26), 1, vert_str)
            vert_str = '\n'.join(vert_str)
            with open(vert_filepath, "w") as f:
                f.write(vert_str)
        
        if ind_filepath is not False:
            inds = self.indices.astype(str)
            ind_str = '\n'.join([' '.join(i) for i in inds])
            with open(ind_filepath, "w") as f:
                f.write(ind_str)

    def plot_points(self):
        """Plot the x, y, z coords in a 3D matplotlib figure as points."""
        f = plt.figure()
        a = f.add_subplot(111, projection="3d", proj_type="ortho")
        a.plot(self.xyz[:,0], self.xyz[:,1], self.xyz[:,2], 'k.')
        
        a.set_xlabel("X")
        a.set_ylabel("Y")
        a.set_zlabel("Z")
        plt.show()
    
    def plot_triangles(self):
        f = plt.figure()
        a = f.add_subplot(111, projection="3d", proj_type="ortho")
        for i in self.indices:
            pos = self.xyz[i]
            a.plot(pos[:,0], pos[:,1], pos[:,2])
        
        a.set_xlabel("X")
        a.set_ylabel("Y")
        a.set_zlabel("Z")
        plt.show()




class Sphere(Shape):
    """
    Create the xyz data for a sphere and create the 
    """
    # These are the initial coords and indices for an octahedron
    verts = np.array([[ 0.,  0.,  1.],  [ 1.,  0.,  0.], [ 0.,  1.,  0.],
                      [-1.,  0.,  0.],  [-0., -1.,  0.],   [ 0.,  0., -1.]])
    init_inds = [[0, 1, 2],  [0, 2, 3],  [0, 3, 4],  [0, 4, 1],
                          [5, 1, 2],  [5, 2, 3],  [5, 3, 4],  [5, 4, 1]]
    nvert = 6
    
    def __init__(self, radius, mesh_resolution=2):
        self.R = radius
        self.mesh_resolution = mesh_resolution
        super().__init__()
    
    # def __normalise_point__(pos, radius=1):
    #     """
    #     Will normalise a point so it is a set distance, radius, from the center.
        
    #     The center is defined as (0, 0, 0).

    #     Parameters
    #     ----------
    #     pos : a list of length 3
    #         The position to be normalised to the radius
    #     radius : float, optional
    #         The radius of the sphere. The default is 1.

    #     Returns
    #     -------
    #     None.
    #     """
        
    
    def create_vertices(self):
        """
        Will create the sphere's vertices.
        
        The vertices are the xyz coord for certain points on a sphere.
        
        Parameters
        ----------
        None.
        
        Returns
        -------
        None.
        """   
        # Create a mesh of triangles by subdividing an ocathedron
        mesh_count = 0
        while mesh_count < self.mesh_resolution:
            
            len_init_inds = len(self.init_inds)
            for ind_id in range(len_init_inds):
                inds = self.init_inds[ind_id]
                p1, p2, p3 = self.verts[inds]
                
                # Calculate and save the 3 new vertices
                new_verts =[(p1 + p2)/2., (p1 + p3)/2., (p2 + p3)/2.]
                self.verts = np.vstack((self.verts, new_verts))
    
                # Add the 4 new triangles with the indices
                self.init_inds.append([inds[0], self.nvert, self.nvert+1])
                self.init_inds.append([inds[1], self.nvert, self.nvert+2])
                self.init_inds.append([inds[2], self.nvert+2, self.nvert+1])
                self.init_inds.append([self.nvert, self.nvert+1, self.nvert+2])
                
                self.nvert += 3
            
            mesh_count += 1

        # Now set each vertex a set distance away from the center (0, 0, 0)
        norms = np.linalg.norm(self.verts, axis=1)
        self.verts[:, 0] /= norms
        self.verts[:, 1] /= norms
        self.verts[:, 2] /= norms
        self.xyz = self.verts * self.R

    
    def create_indices(self):  self.indices = np.array(self.init_inds)

sphere = Sphere(0.7, mesh_resolution=1)
# sphere.plot_points()
# sphere.plot_triangles()
sphere.write_data("../data/vertices.arr", "../data/elements.arr")
