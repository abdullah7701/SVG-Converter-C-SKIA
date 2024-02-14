# SVG-Converter-C-SKIA
This C++ code utilizes the Skia graphics library to parse SVG data from a JSON file, create SVG elements (rectangles or circles), and render them onto an image
Start:

Include Libraries: The code begins by including necessary Skia, C++ standard, and JSON-related libraries.

Define SVGElement Class: An abstract base class SVGElement is defined, containing virtual functions for drawing, setting attributes, and getting attributes.

Define SVGRect and SVGCircle Classes: Concrete classes SVGRect and SVGCircle are defined, inheriting from SVGElement. These classes represent SVG rectangles and circles, respectively, and implement the required virtual functions.

Parse SVG JSON Data: The parseSVGJSON function takes a JSON-formatted SVG data string, parses it using the JSON library, and creates a vector of unique pointers to SVGElement objects based on the parsed data.

Save SVG Elements as Image: The saveSVGAsImage function creates an image canvas using Skia, draws the parsed SVG elements onto the canvas, and saves the resulting image in PNG or JPEG format based on the specified output format.

Load SVG Data from File: The loadSVGFromFile function reads SVG data from a file and returns it as a string.

Main Function: The main function loads SVG data from "input.json," parses it into SVG elements, and saves two images, "output.png" and "output.jpg," using the saveSVGAsImage function.

End:

Return 0: The program concludes by returning 0 from the main function.
Overall Working:

The code processes SVG data from a JSON file, creates corresponding SVG elements (rectangles or circles) based on the data, renders these elements onto an image using Skia, and saves the resulting image in both PNG and JPEG formats. The main function orchestrates these steps, serving as a simple SVG-to-image converter.
