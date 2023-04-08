
#define numVertices 3
#define numEdges 3
#define numFaces 1

float vertices[numVertices][3] = {
    { 0.0,  0.5, 0.0 },  // top
    { 0.5, -0.5, 0.0 },  // bottom right
    { -0.5, -0.5, 0.0 }  // bottom left
};


int edges[numEdges][2] = {
    { 0, 1 },
    { 1, 2 },
    { 2, 0 }
};


int faces[numFaces][3] = {
    { 0, 1, 2 }
};