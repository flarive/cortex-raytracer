// https://fr.wikipedia.org/wiki/Path_tracing

#include "misc/renderParameters.h"
#include "cameras/target_camera.h"
#include "primitives/hittable_list.h"
#include "scenes/scene_manager.h"
#include "primitives/hittable_list.h"
#include "misc/timer.h"
#include "misc/singleton.h"
#include "misc/scene.h"
#include "renderer.h"

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
/// https://github.com/JasperJeuken/CppRayTracer
/// https://github.com/iceman201/RayTracing/blob/master/Ray%20tracing/Cone.cpp
/// https://github.com/kamiyo/RayTra/blob/master/RayTra/Torus.cpp
/// https://github.com/aromanro/RayTracer
/// 
/// https://medium.com/@dbildibay/ray-tracing-adventure-part-iv-678768947371
/// https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/#computing-the-tangents-and-bitangents
/// 
/// CUDA
/// https://developer.nvidia.com/blog/accelerated-ray-tracing-cuda/
/// https://github.com/rogerallen/raytracinginoneweekendincuda?tab=readme-ov-file
/// Github with both CPU & GPU : https://github.com/DveloperY0115/RTFoundation
/// 
/// https://github.com/AdilRabbani/Rendering-Lab/blob/main/light.h
/// https://github.com/elp2/ray-tracer-challenge
/// https://github.com/AkeelMedina22/Ray-Tracer/blob/main/raytracer/light/Light.cpp
/// https://github.com/define-private-public/PSRayTracing // black speckles fix !!!!
/// 
/// https://github.com/danoli3/Multithreaded-Ray-Tracer/blob/master/wxRaytracer/raytracer/Samplers/Jittered.cpp // more samplers !
/// https://github.com/gbrsouza/RayTracer/blob/master/src/orthographic_camera.cpp

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
    cam.background_color = color(0.70, 0.80, 1.00);

    // Create world
    scene_manager builder;
    //hittable_list lights;

    //scene world = builder.random_spheres(cam);
    //scene world = builder.two_spheres(cam);
    //scene world = builder.two_perlin_spheres(cam);
    //scene world = builder.advanced_lights(cam);
    //scene world = builder.cornell_box(cam);
    //scene world = builder.cornell_box_smoke(cam);
    //scene world = builder.cornell_box_custom(cam); // 1m26s mono thread, 40s multi thread 512x512
    //scene world = builder.cornell_box_phong(cam);
    
    //scene world = builder.all_materials_spheres(cam);
    //scene world = builder.alpha_texture_demo(cam);
    //scene world = builder.cow_scene(cam);
    //scene world = builder.nautilus_scene(cam);
    
    //scene world = builder.lambertian_spheres(cam);
    //scene world = builder.phong_spheres(cam);
    //scene world = builder.oren_nayar_spheres(cam);
    //scene world = builder.isotropic_anisotropic_spheres(cam);

    //scene world = builder.extended_primitives(cam);
    //scene world = builder.transparency_materials_spheres(cam);

    //scene world = builder.simple_sphere(cam);
    //scene world = builder.final_scene(cam);


    //scene world = builder.advanced_lights(cam);

   
    scene world = builder.load_scene(cam, params.sceneName);




    
    std::cout << "[INFO] Ready !" << std::endl;
    
    timer renderTimer;

    // Start measuring time
    renderTimer.start();

    renderer render;
    render.render(world, cam, params, true);

    // Stop measuring time
    renderTimer.stop();

    if (!params.quietMode)
        renderTimer.displayTime();

    std::cout << "[INFO] Finished !" << std::endl;

    exit(EXIT_SUCCESS);
}