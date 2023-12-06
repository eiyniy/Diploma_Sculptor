#include <Converter.hpp>

Matrix<4, 1> Converter::vertexToMatrix(const Vertex &value) {
    return {value.cGetX(), value.cGetY(), value.cGetZ(), value.cGetW()};
}

DrawableVertex Converter::matrixToDrawableVertex(
        const Matrix<4, 1> &value,
        double w,
        const bool isVisible,
        const bool isWNegative) {
    return {
            value.cGetX(),
            value.cGetY(),
            w,
            isVisible,
            isWNegative};
}

Point Converter::drawableVertexToPoint(const DrawableVertex &value) {
    return {(int) std::round(value.CGetX()), (int) std::round(value.CGetY())};
}

Matrix<4, 1> Converter::nVertexToMatrix(const NormalVertex &value) {
    return {value.getI(), value.getJ(), value.getK(), 0};
}

Vertex Converter::matrixToVertex(const Matrix<4, 1> &value, double w) {
    return {value.cGetX(), value.cGetY(), w};
}
