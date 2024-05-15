#ifndef HALIDE_UNSHARP_HPP
#define HALIDE_UNSHARP_HPP

#include "Halide.h"

using namespace Halide;
using namespace Halide::ConciseCasts;

class HalideUnsharpen : public Generator<HalideUnsharpen>
{
public:
    Input<Buffer<uint8_t>> img_input{"img_input", 3};
    Input<float> amount{"amount"};

    Output<Buffer<uint8_t>> img_output{"img_output", 3};

    void generate()
    {
        img_bound = BoundaryConditions::mirror_image(Func(img_input));
        img_bound_i16(x, y, c) = i16(img_bound(x, y, c));

        Expr result;
        result = img_bound_i16(x - 1, y - 1, c) + 2 * img_bound_i16(x, y - 1, c) + img_bound_i16(x + 1, y - 1, c) + 2 * img_bound_i16(x - 1, y, c) + 4 * img_bound_i16(x, y, c) + 2 * img_bound_i16(x + 1, y, c) + img_bound_i16(x - 1, y + 1, c) + 2 * img_bound_i16(x, y + 1, c) + img_bound_i16(x, y + 1, c);
        result = result / 16;

        Expr blurred = u8(clamp(result, 0, 255));
        img_output(x, y, c) = u8(clamp(img_input(x, y, c) + amount * (img_input(x, y, c) - blurred), 0, 255));
    }

private:
    Var x{"x"}, y{"y"}, c{"c"};
    Func img_bound{"img_bound"}, img_bound_i16{"img_bound_i16"};
};

#endif
