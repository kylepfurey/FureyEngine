// .h
// Static Math Functions Script
// by Kyle Furey

#pragma once
#include "../Transform/Transform.h"

namespace FureyEngine {
    /** A collection of useful static mathematical functions. */
    class Math final {
        // ABSTRACT CLASS

        /** Prevents instantiation of this class. */
        virtual void Abstract() = 0;

    public:
        // LERP

        /** Returns T's linear interpolation between A and B. */
        template<typename NumberType = double>
        static NumberType Lerp(const NumberType &A, const NumberType &B, const NumberType &T) {
            return (B - A) * T + A;
        }

        /** Returns T's percentage between A and B. */
        template<typename NumberType = double>
        static NumberType InverseLerp(const NumberType &A, const NumberType &B, const NumberType &T) {
            if (A == B) {
                return 0;
            }
            return (T - A) / (B - A);
        }

        // VECTORS

        /** Returns the magnitude of the given direction. */
        template<typename VectorType = glm::vec2>
        static double Magnitude(const VectorType &Direction) {
            return SDL_sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
        }

        /** Normalizes the given direction. */
        template<typename VectorType = glm::vec2>
        static VectorType Normalize(VectorType Direction, const float &Catch = 0) {
            const float Mag = Magnitude(Direction);
            if (Mag <= Catch) {
                return {0, 0};
            }
            Direction.x /= Mag;
            Direction.y /= Mag;
            return Direction;
        }

        /** Returns the given direction rotated by the given degrees. */
        template<typename VectorType = glm::vec2>
        static VectorType Rotate(const VectorType &Direction, const float &Degrees) {
            const float Cos = glm::cos(glm::radians(-Degrees));
            const float Theta = glm::sin(glm::radians(-Degrees));
            VectorType Rotation = {};
            Rotation.x = Direction.x * Cos - Direction.y * Theta;
            Rotation.y = Direction.x * Theta + Direction.y * Cos;
            return Rotation;
        }

        /** Calculates the squared distance between the given two points. */
        template<typename VectorType = glm::vec2>
        static double DistanceSquared(const VectorType &A, const VectorType &B) {
            return (B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y);
        }

        /** Calculates the exact distance between the given two points. */
        template<typename VectorType = glm::vec2>
        static double Distance(const VectorType &A, const VectorType &B) {
            return SDL_sqrt(DistanceSquared(A, B));
        }

        /** Calculates the Manhattan distance between the given two points. */
        template<typename VectorType = glm::vec2>
        static double ManhattanDistance(const VectorType &A, const VectorType &B) {
            return SDL_fabs(B.x - A.x) + SDL_fabs(B.y - A.y);
        }

        // BITFLAGS

        /** Combines and returns the given enum with the given flag. */
        template<typename EnumType, typename NumberType = unsigned char>
        static EnumType AddEnumFlag(const EnumType &Enum, const EnumType &Flag) {
            const int NewEnum = static_cast<NumberType>(Enum) | static_cast<NumberType>(Flag);
            return static_cast<EnumType>(NewEnum);
        }

        /** Removes the given flag from the given enum. */
        template<typename EnumType, typename NumberType = unsigned char>
        static EnumType RemoveEnumFlag(const EnumType &Enum, const EnumType &Flag) {
            const int NewEnum = static_cast<NumberType>(Enum) & ~static_cast<NumberType>(Flag);
            return static_cast<EnumType>(NewEnum);
        }

        /** Returns whether the given enum has the given flag. */
        template<typename EnumType, typename NumberType = unsigned char>
        static bool EnumHasFlag(const EnumType &Enum, const EnumType &Flag) {
            return static_cast<NumberType>(Enum) & static_cast<NumberType>(Flag);
        }

        // POINT AND LINE

        /** Returns whether the given point overlaps the given line. */
        template<typename VectorType = glm::vec2>
        [[nodiscard]] static bool DoesPointOverlapLine(const VectorType &Point,
                                                       const VectorType &LinePointA, const VectorType &LinePointB) {
            const bool Collinear = (Point.x - LinePointA.x) * (LinePointA.y - LinePointB.y) ==
                                   (Point.y - LinePointA.y) * (LinePointA.x - LinePointB.x);

            const bool InBoundingBox = SDL_min(LinePointA.x, LinePointB.x) <= Point.x &&
                                       Point.x <= SDL_max(LinePointA.x, LinePointB.x) &&
                                       SDL_min(LinePointA.y, LinePointB.y) <= Point.y &&
                                       Point.y <= SDL_max(LinePointA.y, LinePointB.y);

            return Collinear && InBoundingBox;
        }

        /** Returns whether the given lines overlap. */
        template<typename VectorType = glm::vec2>
        [[nodiscard]] static bool DoesLineOverlapLine(const VectorType &LineAPointA, const VectorType &LineAPointB,
                                                      const VectorType &LineBPointA, const VectorType &LineBPointB) {
            if (LineAPointA == LineBPointA || LineAPointA == LineBPointB ||
                LineAPointB == LineBPointA || LineAPointB == LineBPointB) {
                return true;
            }

            const VectorType LineADirection = LineAPointB - LineAPointA;
            const VectorType LineBDirection = LineBPointB - LineBPointA;

            const float Denominator = LineADirection.x * LineBDirection.y - LineADirection.y * LineBDirection.x;

            // Check for parallel lines (handling floating point precision)
            if (SDL_fabs(Denominator) < 0.00001) {
                return false;
            }

            const float IntersectionA = ((LineBPointA.x - LineAPointA.x) * LineBDirection.y -
                                         (LineBPointA.y - LineAPointA.y) * LineBDirection.x) / Denominator;
            const float IntersectionB = ((LineBPointA.x - LineAPointA.x) * LineADirection.y -
                                         (LineBPointA.y - LineAPointA.y) * LineADirection.x) / Denominator;

            if (IntersectionA >= 0 && IntersectionA <= 1 && IntersectionB >= 0 && IntersectionB <= 1) {
                return true;
            }

            return false;
        }
    };
}
