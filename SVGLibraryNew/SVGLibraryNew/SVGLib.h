#ifndef SVGLIB_H
#define SVGLIB_H

#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <memory>

using namespace std;

namespace Svg {
    void RenderColor(ostream& out, monostate);
    void RenderColor(ostream& out, const string& value);
    void RenderColor(ostream& out, Rgb rgb);
    void RenderColor(ostream& out, const Color& color);

    class Object {
    public:
        virtual void Render(ostream& out) const = 0;
        virtual ~Object() = default;
    };

    template <typename Owner>
    class PathProps {
    public:
        Owner& SetFillColor(const Color& color);
        Owner& SetStrokeColor(const Color& color);
        Owner& SetStrokeWidth(double value);
        Owner& SetStrokeLineCap(const string& value);
        Owner& SetStrokeLineJoin(const string& value);
        void RenderAttrs(ostream& out) const;

    private:
        Color fill_color_;
        Color stroke_color_;
        double stroke_width_ = 1.0;
        optional<string> stroke_line_cap_;
        optional<string> stroke_line_join_;

        Owner& AsOwner();
    };
}

#endif SVGLIB_H

