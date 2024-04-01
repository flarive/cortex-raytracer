#include "renderParameters.h"
#include "cameras/target_camera.h"
#include "primitives/hittable_list.h"
#include "worldbuilder.h"
#include "primitives/hittable_list.h"
#include "timer.h"
#include "singleton.h"
#include "misc/scene.h"
#include "Renderer.h"
#include "Random.h"

using namespace std;


bool quietMode;

Singleton* Singleton::singleton_ = nullptr;

/**
 * Static methods should be defined outside the class.
 */
Singleton* Singleton::getInstance()
{
    /**
     * This is a safer way to create an instance. instance = new Singleton is
     * dangerous in case two instance threads wants to access at the same time
     */
   /* if (singleton_ == nullptr) {
        singleton_ = new Singleton(value);
    }*/
    return singleton_;
}




/// <summary>
/// https://github.com/Drummersbrother/raytracing-in-one-weekend
/// https://github.com/essentialblend/weekend-raytracing
/// https://github.com/EmreDogann/Software-Ray-Tracer
/// https://github.com/aleksi-kangas/raytracing
/// https://github.com/optozorax/ray-tracing?tab=readme-ov-file
/// https://github.com/lconn-dev/MinimalRT
/// https://github.com/TheVaffel/weekend-raytracer/tree/master
/// https://github.com/mgaillard/Renderer
/// https://github.com/boksajak/raytracingthenextweek
/// https://github.com/NickSpyker/RayTracer
/// https://github.com/tylermorganwall/rayrender
/// https://github.com/kdridi/raytracer
/// https://github.com/define-private-public/PSRayTracing/
/// https://github.com/VladChira/ray-tracer
/// https://github.com/Friduric/ray-tracer/blob/master/src/Rendering/Materials/OrenNayarMaterial.cpp
/// https://bunjevac.net/blog/parallelizing-raytracer-in-a-weekend/
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main(int argc, char* argv[])
{
    renderParameters params = renderParameters::getArgs(argc, argv);

    Singleton::singleton_ = new Singleton(params);
    


    // Init camera and render world
    target_camera cam;
    cam.aspect_ratio = params.ratio;
    cam.image_width = params.width;
    cam.samples_per_pixel = params.samplePerPixel; // antialiasing quality
    cam.max_depth = params.recursionMaxDepth; // max nbr of bounces a ray can do
    cam.background = color(0.70, 0.80, 1.00);

    // Create world
    worldbuilder builder;
    hittable_list lights;

    //scene world = builder.random_spheres(cam);
    //scene world = builder.two_spheres(cam);
    //scene world = builder.earth(cam);
    //scene world = builder.wood_sphere(cam);
    //scene world = builder.two_perlin_spheres(cam);
    //scene world = builder.quads(cam);
    //scene world = builder.simple_light(cam);
    //scene world = builder.advanced_lights(cam);
    //scene world = builder.cornell_box(cam);
    //scene world = builder.cornell_box_smoke(cam);
    //scene world = builder.cornell_box_custom(cam);
    //scene world = builder.final_scene(cam);
    //scene world = builder.three_spheres(cam);
    //scene world = builder.gradient_texture_demo(cam);
    //scene world = builder.alpha_texture_demo(cam);
    //scene world = builder.cow_scene(cam);
    //scene world = builder.lambertian_spheres(cam);
    scene world = builder.phong_spheres(cam);
    //scene world = builder.oren_nayar_spheres(cam);
    //scene world = builder.cornell_box_phong(cam);
    
    timer renderTimer;

    // Start measuring time
    renderTimer.start();

    //cam.initialize(params);
    cam.render(world, params);

    Renderer renderer(std::make_unique<scene>(world));
    renderer.setSamplesPerPixels(params.samplePerPixel);
    const auto floatImage = renderer.render(params.width, params.height);
    renderer.saveAsPPM(floatImage, "output.ppm");

    // Stop measuring time
    renderTimer.stop();

    
    renderTimer.displayTime();

    exit(EXIT_SUCCESS);
}