namespace line_effects {
    constexpr unsigned none = 0;
    constexpr unsigned gradient = 1;
};

namespace line_fillness {
    constexpr unsigned solid = 0;
    constexpr unsigned dotted = 1;
    constexpr unsigned dashed = 2;
    constexpr unsigned dotdash = 3;
};

namespace line_antialiasing {
    constexpr unsigned none = 0;
    constexpr unsigned wu = 1;
};

namespace circle_effects {
    constexpr unsigned none = 0;
    constexpr unsigned gradient = 1;
};

namespace circle_antialiasing {
    constexpr unsigned none = 0;
    constexpr unsigned wu = 1;
};

namespace rect_effects {
    constexpr unsigned none = 0;
    constexpr unsigned gradient = 1;
    constexpr unsigned inner_gradient = 2;
    constexpr unsigned border_gradient = 4;
};

namespace rect_antialiasing {
    constexpr unsigned none = 0;
    constexpr unsigned wu = 1;
};