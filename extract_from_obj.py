# read file from models/

import sys
import os
import numpy as np

fileContents = ""

# filename = sys.argv[1]
filename = "theCube.obj"
filename = "water-tap.obj"
filename = "pentagon-prism.obj"
filename = "reduced-plant.obj"

with open((filename), "r") as f:
    fileContents = f.read()


# split file into lines
lines = fileContents.split("\n")

# get vertices  
vertices = []
for line in lines:
    if(line[0:2] == "v "):
        elements = (line.split(" "))
        vertices.append([float(elements[1]), float(elements[2]), float(elements[3])])

# get faces
faces = []
for line in lines:
    if(line[0:2] == "f "):
        vertices_of_face = line.split(" ")[1:]
        vertices_of_face = [int(vertex.split("/")[0]) - 1 for vertex in vertices_of_face]
        faces.append(vertices_of_face)

# for line in lines:
#     if(line[0:2] == "f "):
#         elements = (line.split(" "))
#         face_components = [elements[1].split("/"), elements[2].split("/"), elements[3].split("/")]
#         face_vertices = [int(face_components[0][0]) - 1, int(face_components[1][0]) - 1, int(face_components[2][0]) - 1]
#         faces.append(face_vertices)


# find edges
edges = []
for face in faces:
    for i in range(len(face)):
        edge = [face[i], face[(i+1)%len(face)]]
        if(edge[0] > edge[1]):
            edge = [edge[1], edge[0]]
        if(edge not in edges):
            edges.append(edge)


# convert all multi point face to triangles
faces3 = []
for face in faces:
    if(len(face) == 3):
        faces3.append(face)
    else:
        for i in range(len(face) - 2):
            faces3.append([face[0], face[i+1], face[i+2]])
        
faces = faces3


# export to model.c
with open("model.h", "w") as f:
    f.write("#define numVertices " + str(len(vertices)) + "\n")
    f.write("#define numEdges " + str(len(edges)) + "\n")
    f.write("#define numFaces " + str(len(faces)) + "\n")

    f.write("float vertices[numVertices][3] = {\n")
    for vertex in vertices:
        f.write("\t{" + str(vertex[0]) + ", " + str(vertex[1]) + ", " + str(vertex[2]) + "},\n")
    f.write("\n};\n")

    f.write("int edges[numEdges][2] = {\n")
    for edge in edges:
        f.write("\t{" + str(edge[0]) + ", " + str(edge[1]) + "},\n")
    f.write("};\n")

    f.write("int faces[numFaces][3] = {\n")
    for face in faces:
        f.write("\t{" + str(face[0]) + ", " + str(face[1]) + ", " + str(face[2]) + "},\n")
    f.write("};\n")

# # export to model.h
# with open("model.h", "w") as f:
#     f.write("#define numVertices " + str(len(vertices)) + "\n")
#     f.write("#define numEdges " + str(len(edges)) + "\n")
#     f.write("#define numFaces " + str(len(faces)) + "\n")

#     f.write("extern float vertices[numVertices][3];\n")
#     f.write("extern int edges[numEdges][2];\n")
#     f.write("extern int faces[numFaces][3];\n")
