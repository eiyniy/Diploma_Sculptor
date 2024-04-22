#include <Triangle.hpp>

#include <EarClipper.hpp>
#include <Enums.hpp>
#include <Math.hpp>
#include <ObjParser.hpp>

#include <geometric.hpp>
#include <qualifier.hpp>
#include <type_vec4.hpp>
#include <vector_float4.hpp>

#include <array>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

Triangle::Triangle(const std::vector<VertexIds>& indexes)
    : vertexIndexesCount((int)indexes.size())
    , values { indexes[0], indexes[1], indexes[2] }
{
}

int Triangle::cGetVertexIdsCount() const { return vertexIndexesCount; }
