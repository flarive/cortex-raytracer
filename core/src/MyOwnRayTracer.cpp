#include "renderParameters.h"
#include "cameras/camera.h"
#include "primitives/hittable_list.h"
#include "worldbuilder.h"
#include "primitives/hittable_list.h"
#include "timer.h"

#include <iostream>



using namespace std;


bool quietMode;




/// <summary>
/// https://github.com/Drummersbrother/raytracing-in-one-weekend
/// https://github.com/essentialblend/weekend-raytracing
/// https://github.com/EmreDogann/Software-Ray-Tracer
/// https://github.com/aleksi-kangas/raytracing
/// https://github.com/optozorax/ray-tracing?tab=readme-ov-file
/// https://github.com/lconn-dev/MinimalRT
/// https://github.com/TheVaffel/weekend-raytracer/tree/master
/// https://github.com/mgaillard
/// https://github.com/boksajak/raytracingthenextweek
/// https://github.com/NickSpyker/RayTracer
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main(int argc, char* argv[])
{
    renderParameters params = renderParameters::getArgs(argc, argv);

    // Init camera and render world
    camera cam;
    cam.aspect_ratio = params.ratio;
    cam.image_width = params.width;
    cam.samples_per_pixel = params.samplePerPixel; // antialiasing quality
    cam.max_depth = params.recursionMaxDepth; // max nbr of bounces a ray can do
    cam.background = color(0.70, 0.80, 1.00);

    // Create world
    worldbuilder builder;
    hittable_list lights;

    //hittable_list world = builder.random_spheres(cam);
    //hittable_list world = builder.two_spheres(cam);
    //hittable_list world = builder.earth(cam, lights);
    //hittable_list world = builder.wood_sphere(cam);
    //hittable_list world = builder.two_perlin_spheres(cam);
    //hittable_list world = builder.quads(cam);
    //hittable_list world = builder.simple_light(cam);
    //hittable_list world = builder.cornell_box(cam, lights);
    //hittable_list world = builder.cornell_box_smoke(cam);
    //hittable_list world = builder.final_scene(cam);
    hittable_list world = builder.three_spheres(cam, lights);
    //hittable_list world = builder.cow_scene(cam);


    // calculate bounding boxes to speed up ray computing
    world = hittable_list(make_shared<bvh_node>(world));


    timer renderTimer;

    // Start measuring time
    renderTimer.start();

    cam.render(world, lights, params);

    // Stop measuring time
    renderTimer.stop();

    
    renderTimer.displayTime();

    exit(EXIT_SUCCESS);
}