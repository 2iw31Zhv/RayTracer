# RayTracer
This is a ray tracing software. Unlike most ray tracing softwares, it aims to utilize Bezier curve to model the object and directly handle the intersection between ray and Bezier curve and Bezier surface instead of using triangulated mesh. It is expected that such techniques would be more efficient than the conventional methods.

Currently, it is only supported to be built on Windows 10. In future we may provide the Linux version.

## Dependency

### [Qt 5.7.0]
This is a basic GUI library. We use it to construct the main UI interface.
### [FreeImage]
A light-weighted library for constructing and saving images.
### [Easylogging++]
A light-weighted library for logging. 



Also, it uses a single file produced by a mathematician [Sergey Khashin] to calculate the root of low-order polymial equations. (with a slight modification of the error in the quadratic equations)

[Qt 5.7.0]:	https://www1.qt.io/qt5-7/	"Qt 5.7.0"
[FreeImage]:	https://sourceforge.net/projects/freeimage/	"FreeImage"
[Easylogging++]:	https://github.com/muflihun/easyloggingpp	"Easylogging++"
[Sergey Khashin]: http://math.ivanovo.ac.ru/dalgebra/Khashin/index.html "Sergey Khashin"

## Licenses

Well, it is not determined currently:)
