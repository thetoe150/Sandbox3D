#pragma once
#include <vector>
#include <functional>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          Base class Interface                                              //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace vag
{

    enum class Type
    {
        Mesh,
        Sphere,
        Cylinder
    };

    class Object
    {
    public:
        // ctor/dtor
        Object() = default;
        Object(const Object&) = default;
        ~Object() {}

        // getters/setters
        void setSmooth(bool smooth);

        int getUpAxis() const                   { return upAxis; }
        void setUpAxis(int up);
        void reverseNormals();

        // for vertex data
        unsigned int getVertexCount() const     { return (unsigned int)vertices.size() / 3; }
        unsigned int getIndexCount() const      { return (unsigned int)indices.size(); }
        unsigned int getTriangleCount() const   { return getIndexCount() / 3; }

        unsigned int getVertexSize() const      { return (unsigned int)vertices.size() * sizeof(float); }
        unsigned int getColorSize() const      { return (unsigned int)colors.size() * sizeof(float); }
        unsigned int getNormalSize() const      { return (unsigned int)normals.size() * sizeof(float); }
        unsigned int getTexCoordSize() const    { return (unsigned int)texCoords.size() * sizeof(float); }
        unsigned int getIndexSize() const       { return (unsigned int)indices.size() * sizeof(unsigned int); }

        const float* getVertices() const        { return vertices.data(); }
        const float* getNormals() const         { return normals.data(); }
        const float* getTexCoords() const       { return texCoords.data(); }
        const unsigned int* getIndices() const  { return indices.data(); }


        unsigned int getInterleavedVertexCount() const  { return getVertexCount(); }    // # of vertices
        unsigned int getInterleavedVertexSize() const   { return (unsigned int)interleavedVertices.size() * sizeof(float); }    // # of bytes
        const float* getInterleavedVertices() const     { return interleavedVertices.data(); }

        // debug, not a pure virtual class
        virtual void printLog() const;

    protected:
        virtual void buildVerticesSmooth() = 0;
        virtual void buildVerticesFlat() = 0;
        virtual void buildInterleavedVertices();
        virtual void buildInterleavedColorVertices();
        virtual void changeUpAxis(int from, int to);
        void clearArrays();
        void addVertex(float x, float y, float z);
        void addColor(float x, float y, float z);
        void addNormal(float x, float y, float z);
        void addTexCoord(float s, float t);
        void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
        std::vector<float> computeFaceNormal(float x1, float y1, float z1,
                                            float x2, float y2, float z2,
                                            float x3, float y3, float z3);

        // memeber vars
        bool smooth;
        int upAxis = 2;                             // +X=1, +Y=2, +z=3 (default)
        std::vector<float> vertices;
        std::vector<float> colors;
        std::vector<float> normals;
        std::vector<float> texCoords;
        std::vector<unsigned int> indices;
        std::vector<unsigned int> lineIndices;

        // interleaved
        std::vector<float> interleavedVertices;
        int interleavedStride;                  // # of bytes to hop to the next vertex (should be 32 bytes)

    private:
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                    Mesh Vertices Data Interface                                            //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class Mesh : public Object
    {
    public:
        Mesh(std::function<float(float, float)> zCorFunc, float x_min = 0.0f, float x_max = 1.0f, float y_min = 0.0f, float y_max = 1.0f,
            int widthStepCount = 36, int heightStepCount = 36, bool smooth = false, int up = 2);
        ~Mesh();

        // getters/setters
        float getWidth() const                 { return x_min - x_max; }
        float getHeight()  const              { return y_min - y_max; }
        int getWidthCount() const                 { return widthStepCount; }
        int getHeightCount()  const              { return heightStepCount; }

        void setX(float x_min, float x_may);
        void setY(float y_min, float y_may);
        void setHeight(float height);
        void setWidthCount(int widthCount);
        void setHeightCount(int heightCount);

    private:
        // member function
        void buildVerticesFlat() override;
        void buildVerticesSmooth() override;
        void changeUpAxis(int from, int to) override;
        void generateColor();
        void storeMinMax(float value);

        void printLog() const override;

        // member var
        std::function<float(float, float)> zCorFunc;
        float x_min;
        float x_max;
        float y_min;
        float y_max;
        float z_min = 9999;
        float z_max = -9999;

        unsigned int widthStepCount;
        unsigned int heightStepCount;
    };


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                  Sphere Vertices Data Interface                                            //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class Sphere : public Object
    {
    public:
        // ctor/dtor
        Sphere(float radius=1.0f, int sectorCount= 128, int stackCount= 128, bool smooth=true, int up=2);
        ~Sphere() {}

        // getters/setters
        float getRadius() const                 { return radius; }
        int getSectorCount() const              { return sectorCount; }
        int getStackCount() const               { return stackCount; }

        void setRadius(float radius);
        void setSectorCount(int sectorCount);
        void setStackCount(int stackCount);

    protected:

    private:
        // member functions
        void buildVerticesSmooth() override;
        void buildVerticesFlat() override;

        void printLog() const override;

        // memeber vars
        float radius;
        int sectorCount;                        // longitude, # of slices
        int stackCount;                         // latitude, # of stacks
    };


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                Cylinder Vertices Data Interface                                            //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class Cylinder : public Object
    {
    public:
        // ctor/dtor
        Cylinder(float baseRadius=1.0f, float topRadius=1.0f, float height=1.0f,
                int sectorCount=36, int stackCount=2, bool smooth=true, int up=2);
        ~Cylinder() {}

        // getters/setters
        float getBaseRadius() const             { return baseRadius; }
        float getTopRadius() const              { return topRadius; }
        float getHeight() const                 { return height; }
        int getSectorCount() const              { return sectorCount; }
        int getStackCount() const               { return stackCount; }
        void set(float baseRadius, float topRadius, float height,
                int sectorCount, int stackCount, bool smooth=true, int up=3);
        void setBaseRadius(float radius);
        void setTopRadius(float radius);
        void setHeight(float radius);
        void setSectorCount(int sectorCount);
        void setStackCount(int stackCount);


    private:
        // memeber vars
        float baseRadius;
        float topRadius;
        float height;
        int sectorCount;                        // # of slices
        int stackCount;                         // # of stacks
        unsigned int baseIndex;                 // starting index of base
        unsigned int topIndex;                  // starting index of top

        std::vector<float> unitCircleVertices;

        void buildVerticesSmooth() override;
        void buildVerticesFlat() override;
        void printLog() const override;

        void buildUnitCircleVertices();
        std::vector<float> getSideNormals();
    };
}
