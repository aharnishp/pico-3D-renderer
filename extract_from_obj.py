# read file from models/

import sys
import os
import numpy as np

fileContents = ""
with open(sys.argv[1], "r") as f:
    fileContents = f.read()

# split file into lines
lines = fileContents.split("\n")

# get vertices  
vertices = []
for line in lines:
    if(line[0:1] == "v "):
        elements = (line.split(" "))
        vertices.append([float(elements[1]), float(elements[2]), float(elements[3])])


# get faces
faces = []
for line in lines:
    if(line[0:1] == "f "):
        elements = (line.split(" "))
        faces.append([int(elements[1]), int(elements[2]), int(elements[3])])


# find edges
edges = []
for face in faces:
    for i in range(3):
        edge = [face[i], face[(i+1)%3]]
        if(edge not in edges):
            edges.append(edge)


# export to model.c
with open("model.c", "w") as f:
    f.write("const int numVertices = %d;\n" % len(vertices))
    f.write("const float vertices[][3] = {\n")
    for vertex in vertices:
        f.write("\t{ %f, %f, %f },\n" % (vertex[0], vertex[1], vertex[2]))
    f.write("};\n\n")

    f.write("const int numEdges = %d;\n" % len(edges))
    f.write("const int edges[][2] = {\n")
    for edge in edges:
        f.write("\t{ %d, %d },\n" % (edge[0], edge[1]))
    f.write("};\n\n")

    f.write("const int numFaces = %d;\n" % len(faces))
    f.write("const int faces[][3] = {\n")
    for face in faces:
        f.write("\t{ %d, %d, %d },\n" % (face[0], face[1], face[2]))
    f.write("};\n\n")
