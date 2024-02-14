#include <skia/include/core/SkImage.h>
#include <skia/include/core/SkPaint.h>
#include <skia/include/core/SkRect.h>
#include <skia/include/core/SkCanvas.h>
#include <skia/include/core/SkStream.h>
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <string>
#include <vector>
#include <map>

class SVGElement {
public:
    virtual ~SVGElement() = default;
    virtual void draw(SkCanvas* canvas) = 0;
    virtual void setAttribute(const std::string& name, const std::string& value) = 0;
    virtual std::string getAttribute(const std::string& name) const = 0;
};

class SVGRect : public SVGElement {
private:
    SkRect rect;
    SkPaint paint;

public:
    SVGRect(const Json::Value& data) {
        rect = SkRect::MakeXYWH(data["x"].asFloat(), data["y"].asFloat(), data["width"].asFloat(), data["height"].asFloat());
        paint.setColor(SkColorSetARGB(data["opacity"].asInt(), data["r"].asInt(), data["g"].asInt(), data["b"].asInt()));
    }

    void draw(SkCanvas* canvas) override {
        canvas->drawRect(rect, paint);
    }

    void setAttribute(const std::string& name, const std::string& value) override {
        if (name == "width") {
            rect.fRight = std::stof(value);
        }
    }

    std::string getAttribute(const std::string& name) const override {
        if (name == "height") {
            return std::to_string(rect.height());
        }
        return "";
    }
};

class SVGCircle : public SVGElement {
private:
    SkPoint center;
    float radius;
    SkPaint paint;

public:
    SVGCircle(const Json::Value& data) {
        center = SkPoint::Make(data["cx"].asFloat(), data["cy"].asFloat());
        radius = data["r"].asFloat();
        paint.setColor(SkColorSetARGB(data["opacity"].asInt(), data["fill"]["r"].asInt(), data["fill"]["g"].asInt(), data["fill"]["b"].asInt()));
    }

    void draw(SkCanvas* canvas) override {
        canvas->drawCircle(center, radius, paint);
    }

    void setAttribute(const std::string& name, const std::string& value) override {
        if (name == "r") {
            radius = std::stof(value);
        }
    }

    std::string getAttribute(const std::string& name) const override {
        if (name == "cx") {
            return std::to_string(center.x());
        }
        return "";
    }
};

std::vector<std::unique_ptr<SVGElement>> parseSVGJSON(const std::string& json_data) {
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(json_data.c_str(), root);

    if (!parsingSuccessful) {
        std::cerr << "Failed to parse JSON data." << std::endl;
        return {};
    }

    std::vector<std::unique_ptr<SVGElement>> svg_elements;

    for (const auto& element : root) {
        if (element.isObject()) {
            const std::string& type = element["type"].asString();
            if (type == "rect") {
                svg_elements.push_back(std::make_unique<SVGRect>(element));
            } else if (type == "circle") {
                svg_elements.push_back(std::make_unique<SVGCircle>(element));
            }
        }
    }

    return svg_elements;
}

void saveSVGAsImage(const std::vector<std::unique_ptr<SVGElement>>& svg_elements, const std::string& output_path, const std::string& format) {
    auto surface = SkSurface::MakeRasterN32Premul(800, 600);
    auto canvas = surface->getCanvas();

    for (const auto& element : svg_elements) {
        element->draw(canvas);
    }

    SkPixmap pixmap;
    if (surface->peekPixels(&pixmap)) {
        SkBitmap bitmap;
        bitmap.installPixels(pixmap);
        auto image = SkImage::MakeFromBitmap(bitmap);
        SkStream stream(output_path.c_str(), SkStream::WriteMode::kWriteOnly);
        if (format == "png") {
            image->encodeToPngStream(&stream);
        } else if (format == "jpg") {
            image->encodeToJpegStream(&stream, 100);
        }
        stream.close();
    }
}

std::string loadSVGFromFile(const std::string& file_path) {
    std::ifstream file(file_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    const std::string json_data = loadSVGFromFile("input.json");
    const std::vector<std::unique_ptr<SVGElement>> svg_elements = parseSVGJSON(json_data);
    saveSVGAsImage(svg_elements, "output.png", "png");
    saveSVGAsImage(svg_elements, "output.jpg", "jpg");

    return 0;
}
