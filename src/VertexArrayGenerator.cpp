#include <iostream>
#include <iomanip>
#include <cmath>

#include "VertexArrayGenerator.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          Base class Implementation                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void vag::Object::buildInterleavedVertices()
{
    //std::vector<float>().swap(interleavedVertices);

    std::size_t i, j;
    std::size_t count = vertices.size();
    for(i = 0, j = 0; i < count; i += 3, j += 2)
    {
        interleavedVertices.push_back(vertices[i]);
        interleavedVertices.push_back(vertices[i+1]);
        interleavedVertices.push_back(vertices[i+2]);

        interleavedVertices.push_back(normals[i]);
        interleavedVertices.push_back(normals[i+1]);
        interleavedVertices.push_back(normals[i+2]);

        interleavedVertices.push_back(texCoords[j]);
        interleavedVertices.push_back(texCoords[j+1]);
    }
}

void vag::Object::addVertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

void vag::Object::addColor(float x, float y, float z)
{
    colors.push_back(x);
    colors.push_back(y);
    colors.push_back(z);
}

void vag::Object::addNormal(float x, float y, float z)
{
    normals.push_back(x);
    normals.push_back(y);
    normals.push_back(z);
}

void vag::Object::addTexCoord(float x, float y)
{
    texCoords.push_back(x);
    texCoords.push_back(y);
}

void vag::Object::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

std::vector<float> vag::Object::computeFaceNormal  (float x1, float y1, float z1,  // v1
                                             float x2, float y2, float z2,  // v2
                                             float x3, float y3, float z3)  // v3
{
    const float EPSILON = 0.000001f;

    std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
    float nx, ny, nz;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = x2 - x1;
    float ey1 = y2 - y1;
    float ez1 = z2 - z1;
    float ex2 = x3 - x1;
    float ey2 = y3 - y1;
    float ez2 = z3 - z1;

    // cross product: e1 x e2
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if(length > EPSILON)
    {
        // normalize
        float lengthInv = 1.0f / length;
        normal[0] = nx * lengthInv;
        normal[1] = ny * lengthInv;
        normal[2] = nz * lengthInv;
    }

    return normal;
}

void vag::Object::setUpAxis(int up)
{
    if(this->upAxis == up || up < 1 || up > 3)
        return;

    changeUpAxis(this->upAxis, up);
    this->upAxis = up;
}

void vag::Object::changeUpAxis(int from, int to)
{
    // initial transform matrix cols
    float tx[] = {1.0f, 0.0f, 0.0f};    // x-axis (left)
    float ty[] = {0.0f, 1.0f, 0.0f};    // y-axis (up)
    float tz[] = {0.0f, 0.0f, 1.0f};    // z-axis (forward)

    // X -> Y
    if(from == 1 && to == 2)
    {
        tx[0] =  0.0f; tx[1] =  1.0f;
        ty[0] = -1.0f; ty[1] =  0.0f;
    }
    // X -> Z
    else if(from == 1 && to == 3)
    {
        tx[0] =  0.0f; tx[2] =  1.0f;
        tz[0] = -1.0f; tz[2] =  0.0f;
    }
    // Y -> X
    else if(from == 2 && to == 1)
    {
        tx[0] =  0.0f; tx[1] = -1.0f;
        ty[0] =  1.0f; ty[1] =  0.0f;
    }
    // Y -> Z
    else if(from == 2 && to == 3)
    {
        ty[1] =  0.0f; ty[2] =  1.0f;
        tz[1] = -1.0f; tz[2] =  0.0f;
    }
    //  Z -> X
    else if(from == 3 && to == 1)
    {
        tx[0] =  0.0f; tx[2] = -1.0f;
        tz[0] =  1.0f; tz[2] =  0.0f;
    }
    // Z -> Y
    else
    {
        ty[1] =  0.0f; ty[2] = -1.0f;
        tz[1] =  1.0f; tz[2] =  0.0f;
    }

    std::size_t i, j, t;
    std::size_t count = vertices.size();
    float vx, vy, vz;
    float nx, ny, nz;
    float tex, tey;
    for(i = 0, j = 0, t = 0; i < count; i += 3, j += 8, t += 2)
    {
        // transform vertices
        vx = vertices[i];
        vy = vertices[i+1];
        vz = vertices[i+2];
        vertices[i]   = tx[0] * vx + ty[0] * vy + tz[0] * vz;   // x
        vertices[i+1] = tx[1] * vx + ty[1] * vy + tz[1] * vz;   // y
        vertices[i+2] = tx[2] * vx + ty[2] * vy + tz[2] * vz;   // z

        // transform normals
        nx = normals[i];
        ny = normals[i+1];
        nz = normals[i+2];
        normals[i]   = tx[0] * nx + ty[0] * ny + tz[0] * nz;   // nx
        normals[i+1] = tx[1] * nx + ty[1] * ny + tz[1] * nz;   // ny
        normals[i+2] = tx[2] * nx + ty[2] * ny + tz[2] * nz;   // nz

        // texture colors
        tex = texCoords[t];
        tey = texCoords[t+1];
        texCoords[t]   =  tex;   // nx
        texCoords[t+1] = 1 - tey;   // ny

        //transform interleaved array
        interleavedVertices[j]   = vertices[i];
        interleavedVertices[j+1] = vertices[i+1];
        interleavedVertices[j+2] = vertices[i+2];
        interleavedVertices[j+3] = normals[i];
        interleavedVertices[j+4] = normals[i+1];
        interleavedVertices[j+5] = normals[i+2];

        interleavedVertices[j+6] = texCoords[t];
        interleavedVertices[j+7] = texCoords[t+1];
    }
}

void vag::Object::printLog() const
{
     std::cout<< "Smooth Shading: " << (smooth ? "true" : "false") << "\n"
              << "       Up Axis: " << (upAxis == 1 ? "X" : (upAxis == 2 ? "Y" : "Z")) << "\n"
              << "   Vertex Size: " << getVertexSize() << "\n"
              << "    Color Size: " << getColorSize() << "\n"
              << "   Normal Size: " << getNormalSize() << "\n"
              << "  Texture Size: " << getTexCoordSize() << "\n"
              << "Triangle Count: " << getTriangleCount() << "\n"
              << "   Point Count: " << getVertexCount() << "\n"
              << "   Index Count: " << getIndexCount() << "\n";
}

void vag::Object::clearArrays()
{
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);
}

void vag::Object::buildInterleavedColorVertices()
{
    std::size_t i, j;
    std::size_t count = vertices.size();
    for(i = 0, j = 0; i < count; i += 3, j += 2)
    {
        interleavedVertices.push_back(vertices[i]);
        interleavedVertices.push_back(vertices[i+1]);
        interleavedVertices.push_back(vertices[i+2]);

        interleavedVertices.push_back(colors[i]);
        interleavedVertices.push_back(colors[i+1]);
        interleavedVertices.push_back(colors[i+2]);

        interleavedVertices.push_back(normals[i]);
        interleavedVertices.push_back(normals[i+1]);
        interleavedVertices.push_back(normals[i+2]);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Mesh Vertices Data Implementation                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vag::Mesh::Mesh(std::function<float(float, float)> zCorFunc, float x_min, float x_max, float y_min, float y_max,
            int widthStepCount, int heightStepCount, bool smooth, int up)
    : zCorFunc(zCorFunc), x_min(x_min), x_max(x_max), y_min(y_min), y_max(y_max),
     widthStepCount(widthStepCount), heightStepCount(heightStepCount)
{
    buildVerticesFlat();

    // the buildVertices functions use Z = 3 is the Up axis
    // we need to change Up axis to Y = 2 to have a right shape in openGL
    if(this->upAxis != 3)
        changeUpAxis(3, this->upAxis);

    printLog();
}

vag::Mesh::~Mesh()
{

}

void vag::Mesh::printLog() const
{
    std::cout << "========== Mesh ==========\n"
              << "           x_min: " << x_min << "\n"
              << "           x_max: " << x_max << "\n"
              << "           y_min: " << y_min << "\n"
              << "           y_max: " << y_max << "\n"
              << "Width Step Count: " << widthStepCount << "\n"
              << "Height Step Count: "<< heightStepCount << "\n";

    Object::printLog();
}

void vag::Mesh::setX(float x_min, float x_max)
{
    if(this->x_min != x_min)
        this->x_min = x_min;

    if(this->x_max != x_max)
        this->x_max = x_max;
}

void vag::Mesh::setY(float y_min, float y_may)
{
    if(this->y_min != y_min)
        this->y_min = y_min;

    if(this->y_max != y_max)
        this->y_max = y_max;
}

void vag::Mesh::setWidthCount(int widthCount)
{
    if(this->widthStepCount != widthCount)
        this->widthStepCount = widthCount;
}

void vag::Mesh::setHeightCount(int heightCount)
{
    if(this->heightStepCount != heightCount)
        this->heightStepCount = heightCount;
}

void vag::Mesh::changeUpAxis(int from, int to)
{
    // initial transform matrix cols
    float tx[] = {1.0f, 0.0f, 0.0f};    // x-axis (left)
    float ty[] = {0.0f, 1.0f, 0.0f};    // y-axis (up)
    float tz[] = {0.0f, 0.0f, 1.0f};    // z-axis (forward)

    // X -> Y
    if(from == 1 && to == 2)
    {
        tx[0] =  0.0f; tx[1] =  1.0f;
        ty[0] = -1.0f; ty[1] =  0.0f;
    }
    // X -> Z
    else if(from == 1 && to == 3)
    {
        tx[0] =  0.0f; tx[2] =  1.0f;
        tz[0] = -1.0f; tz[2] =  0.0f;
    }
    // Y -> X
    else if(from == 2 && to == 1)
    {
        tx[0] =  0.0f; tx[1] = -1.0f;
        ty[0] =  1.0f; ty[1] =  0.0f;
    }
    // Y -> Z
    else if(from == 2 && to == 3)
    {
        ty[1] =  0.0f; ty[2] =  1.0f;
        tz[1] = -1.0f; tz[2] =  0.0f;
    }
    //  Z -> X
    else if(from == 3 && to == 1)
    {
        tx[0] =  0.0f; tx[2] = -1.0f;
        tz[0] =  1.0f; tz[2] =  0.0f;
    }
    // Z -> Y
    else
    {
        ty[1] =  0.0f; ty[2] = -1.0f;
        tz[1] =  1.0f; tz[2] =  0.0f;
    }

    std::size_t i, j;
    std::size_t count = vertices.size();
    float vx, vy, vz;
    float nx, ny, nz;
    for(i = 0, j = 0; i < count; i += 3, j += 8)
    {
        // transform vertices
        vx = vertices[i];
        vy = vertices[i+1];
        vz = vertices[i+2];
        vertices[i]   = tx[0] * vx + ty[0] * vy + tz[0] * vz;   // x
        vertices[i+1] = tx[1] * vx + ty[1] * vy + tz[1] * vz;   // y
        vertices[i+2] = tx[2] * vx + ty[2] * vy + tz[2] * vz;   // z
        storeMinMax(vertices[i+1]);

        // transform normals
        nx = normals[i];
        ny = normals[i+1];
        nz = normals[i+2];
        normals[i]   = tx[0] * nx + ty[0] * ny + tz[0] * nz;   // nx
        normals[i+1] = tx[1] * nx + ty[1] * ny + tz[1] * nz;   // ny
        normals[i+2] = tx[2] * nx + ty[2] * ny + tz[2] * nz;   // nz

        //// texture colors
        //float tex = texCoords[i];
        //float tey = texCoords[i+1];
        //texCoords[i]   = tx[0] * tex + ty[0] * tey + tz[0] * nz;   // nx
        //texCoords[i+1] = tx[1] * tex + ty[1] * tey + tz[1] * nz;   // ny
        //texCoords[i+2] = tx[2] * tex + ty[2] * tey + tz[2] * nz;   // nz
    }

    std::vector<float>().swap(this->colors);
    generateColor();
    std::vector<float>().swap(this->interleavedVertices);
    buildInterleavedColorVertices();
}

void vag::Mesh::generateColor()
{
    if(this->z_min > this->z_max ) throw("Invalid min max");
    float zRange_inverse = 1 / (this->z_max - this->z_min);
    std::cout <<"Z range: " << zRange_inverse<<std::endl;
    std::cout <<"Z max: " <<this->z_max<<std::endl;
    std::cout <<"Z min: " <<this->z_min<<std::endl;

    for(auto i = this->vertices.begin() + 1; i < this->vertices.end(); i = i + 3)
    {
        float zColor = (*i - z_min) * zRange_inverse * 1.5f;
        addColor(zColor + 0.6f, 0.7f, 1 - zColor);
    }
}

void vag::Mesh::storeMinMax(float value)
{
    if(value > this->z_max)
        this->z_max = value;

    if(value < this->z_min)
        this->z_min = value;
}

void vag::Mesh::buildVerticesFlat()
{
    float width = x_max - x_min;
    float height = y_max - y_min;

    float widthStep = width / widthStepCount;
    float heightStep = height / heightStepCount;

    unsigned int index = 0;

    for(unsigned int i = 0; i < this->heightStepCount; ++i)
    {
        float y1 = y_min + heightStep * i;
        for(unsigned int j = 0; j < this->widthStepCount; ++j)
        {
            float x1 = x_min + widthStep * j;
            float z1 = zCorFunc(x1, y1);
            // only need to call this function with z1 because will loop through all the other vertices anyway
            storeMinMax(z1);

            //v3--v4
            //|    |
            //v1--v2     

            float x2 = x1 + widthStep;
            float y2 = y1;
            float z2 = zCorFunc(x2, y2);

            float x3 = x1;
            float y3 = y1 + heightStep; 
            float z3 = zCorFunc(x3, y3);

            float x4 = x1 + widthStep;
            float y4 = y1 + heightStep;
            float z4 = zCorFunc(x4, y4);

            addVertex(x1, y1, z1);
            addVertex(x2, y2, z2);
            addVertex(x3, y3, z3);
            addVertex(x4, y4, z4);

            std::vector<float> n = computeFaceNormal(x1, y1, z1, x2, y2, z2, x3, y3, z3);

            for(int ni = 0; ni < 4; ++ni)
            {
                addNormal(n[0], n[1], n[2]);
            }

            addTexCoord((float)(j / widthStepCount)       , (float)(i / heightStepCount));
            addTexCoord((float)((j + 1) / widthStepCount) , (float)(i / heightStepCount));
            addTexCoord((float)(j / widthStepCount)       , (float)((i + 1) / heightStepCount));
            addTexCoord((float)((j + 1) / widthStepCount) , (float)((i + 1) / heightStepCount));


            addIndices(index, index + 1, index + 2);
            addIndices(index + 1, index + 2, index + 3);

            index += 4;
        }
    }
    // this function need the min & max of z, the above loop calculate the min&max of z 

    //buildInterleavedVertices();
    // Color Mesh instead of texture mesh
    generateColor();
    buildInterleavedColorVertices();
}

void vag::Mesh::buildVerticesSmooth()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                Sphere Vertices Data Implementation                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vag::Sphere::Sphere(float radius, int sectors, int stacks, bool smooth, int up)
                : radius(radius), sectorCount(sectors), stackCount(stacks)
{
    if(smooth)
        buildVerticesSmooth();
    else
        buildVerticesFlat();

    // the buildVertices functions use Z = 3 is the Up axis
    // we need to change Up axis to Y = 2 which is the cordinate system of openGL
    // and the default value of the class
    if(this->upAxis != 3)
        changeUpAxis(3, this->upAxis);

    printLog();
}

void vag::Sphere::printLog() const
{
    std::cout << "========== Sphere ==========\n"
              << "        Radius: " << radius << "\n"
              << "  Sector Count: " << sectorCount << "\n"
              << "   Stack Count: " << stackCount << "\n";

    Object::printLog();
}

void vag::Sphere::setRadius(float radius)
{
    if(radius != this->radius)
        this->radius = radius;
}

void vag::Sphere::setSectorCount(int sectors)
{
    if(sectors != this->sectorCount)
        this->sectorCount = sectors;
}

void vag::Sphere::setStackCount(int stacks)
{
    if(stacks != this->stackCount)
        this->stackCount = stacks;
}

void vag::Sphere::buildVerticesSmooth()
{
    const float PI = acos(-1.0f);

    // clear memory of prev arrays
    clearArrays();

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
    float s, t;                                     // texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            addVertex(x, y, z);

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            addNormal(nx, ny, nz);

            // vertex tex coord between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            addTexCoord(s, t);
        }
    }

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if(i != 0)
            {
                addIndices(k1, k2, k1+1);   // k1---k2---k1+1
            }

            if(i != (stackCount-1))
            {
                addIndices(k1+1, k2, k2+1); // k1+1---k2---k2+1
            }
        }
    }

    // generate interleaved vertex array as well
    buildInterleavedVertices();
}

// generate vertices with flat shading
// each triangle is independent (no shared vertices)
void vag::Sphere::buildVerticesFlat()
{
    const float PI = acos(-1.0f);

    // tmp vertex definition (x,y,z,s,t)
    struct Vertex
    {
        float x, y, z, s, t;
    };
    std::vector<Vertex> tmpVertices;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    // compute all vertices first, each vertex contains (x,y,z,s,t) except normal
    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);       // r * cos(u)
        float z = radius * sinf(stackAngle);        // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            Vertex vertex;
            vertex.x = xy * cosf(sectorAngle);      // x = r * cos(u) * cos(v)
            vertex.y = xy * sinf(sectorAngle);      // y = r * cos(u) * sin(v)
            vertex.z = z;                           // z = r * sin(u)
            vertex.s = (float)j/sectorCount;        // s
            vertex.t = (float)i/stackCount;         // t
            tmpVertices.push_back(vertex);
        }
    }

    // clear memory of prev arrays
    clearArrays();

    Vertex v1, v2, v3, v4;                          // 4 vertex positions and tex coords
    std::vector<float> n;                           // 1 face normal

    int i, j, k, vi1, vi2;
    int index = 0;                                  // index for vertex
    for(i = 0; i < stackCount; ++i)
    {
        vi1 = i * (sectorCount + 1);                // index of tmpVertices
        vi2 = (i + 1) * (sectorCount + 1);

        for(j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
        {
            // get 4 vertices per sector
            //  v1--v3
            //  |    |
            //  v2--v4
            v1 = tmpVertices[vi1];
            v2 = tmpVertices[vi2];
            v3 = tmpVertices[vi1 + 1];
            v4 = tmpVertices[vi2 + 1];

            // if 1st stack and last stack, store only 1 triangle per sector
            // otherwise, store 2 triangles (quad) per sector
            if(i == 0) // a triangle for first stack ==========================
            {
                // put a triangle
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v4.x, v4.y, v4.z);

                // put tex coords of triangle
                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v4.s, v4.t);

                // put normal
                n = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v4.x,v4.y,v4.z);
                for(k = 0; k < 3; ++k)  // same normals for 3 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                // put indices of 1 triangle
                addIndices(index, index+1, index+2);

                index += 3;     // for next
            }
            else if(i == (stackCount-1)) // a triangle for last stack =========
            {
                // put a triangle
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v3.x, v3.y, v3.z);

                // put tex coords of triangle
                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v3.s, v3.t);

                // put normal
                n = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v3.x,v3.y,v3.z);
                for(k = 0; k < 3; ++k)  // same normals for 3 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                // put indices of 1 triangle
                addIndices(index, index+1, index+2);

                index += 3;     // for next
            }
            else // 2 triangles for others ====================================
            {
                // put quad vertices: v1-v2-v3-v4
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v3.x, v3.y, v3.z);
                addVertex(v4.x, v4.y, v4.z);

                // put tex coords of quad
                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v3.s, v3.t);
                addTexCoord(v4.s, v4.t);

                // put normal
                n = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v3.x,v3.y,v3.z);
                for(k = 0; k < 4; ++k)  // same normals for 4 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                // put indices of quad (2 triangles)
                addIndices(index, index+1, index+2);
                addIndices(index+2, index+1, index+3);

                index += 4;     // for next
            }
        }
    }
    // generate interleaved vertex array as well
    buildInterleavedVertices();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                Cylinder Vertices Data Implementation                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vag::Cylinder::Cylinder(float baseRadius, float topRadius, float height, int sectors,
                   int stacks, bool smooth, int up)
                   : baseRadius(baseRadius), topRadius(topRadius), height(height), sectorCount(sectors), stackCount(stacks)
{
    buildUnitCircleVertices();
    if(smooth)
        buildVerticesSmooth();
    else
        buildVerticesFlat();

    // the buildVertices functions use Z = 3 is the Up axis
    // we need to change Up axis to Y = 2 to have a right shape in openGL
    if(this->upAxis != 3)
        changeUpAxis(3, this->upAxis);
    printLog();
}

void vag::Cylinder::printLog() const
{
    std::cout << "========== Cylinder ==========\n"
              << "   Base Radius: " << baseRadius << "\n"
              << "    Top Radius: " << topRadius << "\n"
              << "        Height: " << height << "\n"
              << "  Sector Count: " << sectorCount << "\n"
              << "   Stack Count: " << stackCount << "\n";

    Object::printLog();
}

std::vector<float> vag::Cylinder::getSideNormals()
{
    const float PI = acos(-1.0f);
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    // compute the normal vector at 0 degree first
    // tanA = (baseRadius-topRadius) / height
    float zAngle = atan2(baseRadius - topRadius, height);
    float x0 = cos(zAngle);     // nx
    float y0 = 0;               // ny
    float z0 = sin(zAngle);     // nz

    // rotate (x0,y0,z0) per sector angle
    std::vector<float> normals;
    for(int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        normals.push_back(cos(sectorAngle)*x0 - sin(sectorAngle)*y0);   // nx
        normals.push_back(sin(sectorAngle)*x0 + cos(sectorAngle)*y0);   // ny
        normals.push_back(z0);  // nz
        /*
        //debug
        float nx = cos(sectorAngle)*x0 - sin(sectorAngle)*y0;
        float ny = sin(sectorAngle)*x0 + cos(sectorAngle)*y0;
        std::cout << "normal=(" << nx << ", " << ny << ", " << z0
                  << "), length=" << sqrtf(nx*nx + ny*ny + z0*z0) << std::endl;
        */
    }

    return normals;
}

void vag::Cylinder::buildUnitCircleVertices()
{
    const float PI = acos(-1.0f);
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    std::vector<float>().swap(unitCircleVertices);
    for(int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(sin(sectorAngle)); // y
        unitCircleVertices.push_back(0);                // z
    }
}

void vag::Cylinder::buildVerticesFlat()
{
    // tmp vertex definition (x,y,z,s,t)
    struct Vertex
    {
        float x, y, z, s, t;
    };
    std::vector<Vertex> tmpVertices;

    int i, j, k;    // indices
    float x, y, z, s, t, radius;

    // put tmp vertices of cylinder side to array by scaling unit circle
    //NOTE: start and end vertex positions are same, but texcoords are different
    //      so, add additional vertex at the end point
    for(i = 0; i <= stackCount; ++i)
    {
        z = -(height * 0.5f) + (float)i / stackCount * height;      // vertex position z
        radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);     // lerp
        t = 1.0f - (float)i / stackCount;   // top-to-bottom

        for(j = 0, k = 0; j <= sectorCount; ++j, k += 3)
        {
            x = unitCircleVertices[k];
            y = unitCircleVertices[k+1];
            s = (float)j / sectorCount;

            Vertex vertex;
            vertex.x = x * radius;
            vertex.y = y * radius;
            vertex.z = z;
            vertex.s = s;
            vertex.t = t;
            tmpVertices.push_back(vertex);
        }
    }

    // clear memory of prev arrays
    clearArrays();

    Vertex v1, v2, v3, v4;      // 4 vertex positions v1, v2, v3, v4
    std::vector<float> n;       // 1 face normal
    int vi1, vi2;               // indices
    int index = 0;

	// j+1 j
	// || ||
	// v  v 
    // v4-v2 <== stack at i+1
    // | / |
    // v3-v1 <== stack at i
    for(i = 0; i < stackCount; ++i)
    {
        vi1 = i * (sectorCount + 1);            // index of tmpVertices
        vi2 = (i + 1) * (sectorCount + 1);

        for(j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
        {
            v1 = tmpVertices[vi1];
            v2 = tmpVertices[vi2];
            v3 = tmpVertices[vi1 + 1];
            v4 = tmpVertices[vi2 + 1];

            // compute a face normal of v1-v3-v2
            n = computeFaceNormal(v1.x,v1.y,v1.z, v3.x,v3.y,v3.z, v2.x,v2.y,v2.z);

            // put quad vertices: v1-v2-v3-v4
            addVertex(v1.x, v1.y, v1.z);
            addVertex(v2.x, v2.y, v2.z);
            addVertex(v3.x, v3.y, v3.z);
            addVertex(v4.x, v4.y, v4.z);

            // put tex coords of quad
            addTexCoord(v1.s, v1.t);
            addTexCoord(v2.s, v2.t);
            addTexCoord(v3.s, v3.t);
            addTexCoord(v4.s, v4.t);

            // put normal
            for(k = 0; k < 4; ++k)  // same normals for all 4 vertices
            {
                addNormal(n[0], n[1], n[2]);
            }

            // put indices of a quad
            addIndices(index,   index+2, index+1);    // v1-v3-v2
            addIndices(index+1, index+2, index+3);    // v2-v3-v4

            // vertical line per quad: v1-v2
            lineIndices.push_back(index);
            lineIndices.push_back(index+1);
            // horizontal line per quad: v2-v4
            lineIndices.push_back(index+1);
            lineIndices.push_back(index+3);
            if(i == 0)
            {
                lineIndices.push_back(index);
                lineIndices.push_back(index+2);
            }

            index += 4;     // for next
        }
    }

    // remember where the base index starts
    baseIndex = (unsigned int)indices.size();
    unsigned int baseVertexIndex = (unsigned int)vertices.size() / 3;

    // put vertices of base of cylinder
    z = -height * 0.5f;
    addVertex(0, 0, z);
    addNormal(0, 0, -1);
    addTexCoord(0.5f, 0.5f);
    for(i = 0, j = 0; i < sectorCount; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j+1];
        addVertex(x * baseRadius, y * baseRadius, z);
        addNormal(0, 0, -1);
        addTexCoord(-x * 0.5f + 0.5f, -y * 0.5f + 0.5f); // flip horizontal
    }

    // put indices for base
    for(i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k)
    {
        if(i < sectorCount - 1)
            addIndices(baseVertexIndex, k + 1, k);
        else
            addIndices(baseVertexIndex, baseVertexIndex + 1, k);
    }

    // remember where the top index starts
    topIndex = (unsigned int)indices.size();
    unsigned int topVertexIndex = (unsigned int)vertices.size() / 3;

    // put vertices of top of cylinder
    z = height * 0.5f;
    addVertex(0, 0, z);
    addNormal(0, 0, 1);
    addTexCoord(0.5f, 0.5f);
    for(i = 0, j = 0; i < sectorCount; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j+1];
        addVertex(x * topRadius, y * topRadius, z);
        addNormal(0, 0, 1);
        addTexCoord(x * 0.5f + 0.5f, -y * 0.5f + 0.5f);
    }

    for(i = 0, k = topVertexIndex + 1; i < sectorCount; ++i, ++k)
    {
        if(i < sectorCount - 1)
            addIndices(topVertexIndex, k, k + 1);
        else
            addIndices(topVertexIndex, k, topVertexIndex + 1);
    }

    // generate interleaved vertex array as well
    buildInterleavedVertices();
}

void vag::Cylinder::buildVerticesSmooth()
{
    // clear memory of prev arrays
    clearArrays();

    float x, y, z;                                  // vertex position
    //float s, t;                                     // texCoord
    float radius;                                   // radius for each stack

    // get normals for cylinder sides
    std::vector<float> sideNormals = getSideNormals();

    // put vertices of side cylinder to array by scaling unit circle
    for(int i = 0; i <= stackCount; ++i)
    {
        z = -(height * 0.5f) + (float)i / stackCount * height;      // vertex position z
        radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);     // lerp
        float t = 1.0f - (float)i / stackCount;   // top-to-bottom

        for(int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
        {
            x = unitCircleVertices[k];
            y = unitCircleVertices[k+1];
            addVertex(x * radius, y * radius, z);   // position
            addNormal(sideNormals[k], sideNormals[k+1], sideNormals[k+2]); // normal
            addTexCoord((float)j / sectorCount, t); // tex coord
        }
    }

    // remember where the base.top vertices start
    unsigned int baseVertexIndex = (unsigned int)vertices.size() / 3;

    // put vertices of base of cylinder
    z = -height * 0.5f;
    addVertex(0, 0, z);
    addNormal(0, 0, -1);
    addTexCoord(0.5f, 0.5f);
    for(int i = 0, j = 0; i < sectorCount; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j+1];
        addVertex(x * baseRadius, y * baseRadius, z);
        addNormal(0, 0, -1);
        addTexCoord(-x * 0.5f + 0.5f, -y * 0.5f + 0.5f);    // flip horizontal
    }

    // remember where the base vertices start
    unsigned int topVertexIndex = (unsigned int)vertices.size() / 3;

    // put vertices of top of cylinder
    z = height * 0.5f;
    addVertex(0, 0, z);
    addNormal(0, 0, 1);
    addTexCoord(0.5f, 0.5f);
    for(int i = 0, j = 0; i < sectorCount; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j+1];
        addVertex(x * topRadius, y * topRadius, z);
        addNormal(0, 0, 1);
        addTexCoord(x * 0.5f + 0.5f, -y * 0.5f + 0.5f);
    }

    // put indices for sides
    unsigned int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // bebinning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 trianles per sector
            addIndices(k1, k1 + 1, k2);
            addIndices(k2, k1 + 1, k2 + 1);

            // vertical lines for all stacks
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            // horizontal lines
            lineIndices.push_back(k2);
            lineIndices.push_back(k2 + 1);
            if(i == 0)
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    // remember where the base indices start
    baseIndex = (unsigned int)indices.size();

    // put indices for base
    for(int i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k)
    {
        if(i < (sectorCount - 1))
            addIndices(baseVertexIndex, k + 1, k);
        else    // last triangle
            addIndices(baseVertexIndex, baseVertexIndex + 1, k);
    }

    // remember where the base indices start
    topIndex = (unsigned int)indices.size();

    for(int i = 0, k = topVertexIndex + 1; i < sectorCount; ++i, ++k)
    {
        if(i < (sectorCount - 1))
            addIndices(topVertexIndex, k, k + 1);
        else
            addIndices(topVertexIndex, k, topVertexIndex + 1);
    }

    // generate interleaved vertex array as well
    buildInterleavedVertices();
}

void vag::Cylinder::setBaseRadius(float radius)
{
    if(this->baseRadius != radius)
        this->baseRadius = radius;
}

void vag::Cylinder::setTopRadius(float radius)
{
    if(this->topRadius != radius)
        this->topRadius = radius;
}

void vag::Cylinder::setHeight(float height)
{
    if(this->height != height)
        this->height = height;
}

void vag::Cylinder::setSectorCount(int sectors)
{
    if(this->sectorCount != sectors)
        this->sectorCount = sectors;
}

void vag::Cylinder::setStackCount(int stacks)
{
    if(this->stackCount != stacks)
        this->stackCount = stacks;
}
