// .h
// OpenGL Quad Script
// by Kyle Furey

#pragma once
namespace FureyEngine {
    /**
     * Data that comprises a quad used in OpenGL rendering.<br/>
     * This quad fills the whole screen and should be scaled based on window and texture size.
     */
    struct Quad final {
        // GEOMETRY

        /** Each of the vertices that make up a quad. */
        static constexpr float Vertices[20] = {
        //   X   Y   Z      U   V
            -1, -1,  0,     0,  0,   // Bottom Left
             1, -1,  0,     1,  0,   // Bottom Right
            -1,  1,  0,     0,  1,   // Top Left
             1,  1,  0,     1,  1    // Top Right
        //   X   Y   Z      U   V
        };

        /** Each set of three vertices that comprise a triangle in a quad. */
        static constexpr unsigned int Indices[6] = {
        //   0   1   2
             0,  2,  1,     // Bottom Left Triangle
             3,  1,  2      // Top Right Triangle
        //   0   1   2
       };
    };
}
