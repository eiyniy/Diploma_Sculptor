#include <Triangle.hpp>

#include <EntryIds.hpp>

#include <stdexcept>
#include <vector>

Triangle::Triangle(const std::vector<EntryIds>& indexes)
    : values { indexes[0], indexes[1], indexes[2] }
{
}

const EntryIds& Triangle::cGetVertexIds(const std::size_t i) const
{
    if (i > 2) {
        throw std::invalid_argument("Could not get EntryIds");
    }

    return values.at(i);
}
