#ifndef CAMERA_H
#define CAMERA_H

#include "constants.h"
#include "color.h"
#include "hittable.h"
#include "materials/material.h"

#include <iostream>


class camera
{
public:
    /* Public Camera Parameters Here */

    double  aspect_ratio = 1.0;             // Ratio of image width over height
    int     image_width = 400;              // Rendered image width in pixel count
    int     samples_per_pixel = 10;         // Count of random samples for each pixel (antialiasing)
    int     max_depth = 10;                 // Maximum number of ray bounces into scene

    double  vfov = 90;                      // Vertical view angle (field of view) (90 is for wide-angle view for exemple)
    point3  lookfrom = point3(0, 0, -1);    // Point camera is looking from
    point3  lookat = point3(0, 0, 0);       // Point camera is looking at
    vec3    vup = vec3(0, 1, 0);            // Camera-relative "up" direction

    // Depth of field
    double  defocus_angle = 0;              // Variation angle of rays through each pixel
    double  focus_dist = 10;                // Distance from camera lookfrom point to plane of perfect focus

    color   background;                     // Scene background color


    /// <summary>
    /// Camera render logic
    /// </summary>
    /// <param name="world"></param>
    void render(const hittable& world, renderParameters params)
    {
        initialize(params);

        // write ppm file header
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j)
        {
            if (!params.quietMode)
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

            for (int i = 0; i < image_width; ++i)
            {
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; ++sample)
                {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                // write ppm file color entry
                write_color(std::cout, pixel_color, samples_per_pixel);
            }
        }

        if (!params.quietMode)
            std::clog << "\rDone.                 \n";
    }

    /// <summary>
    /// Get a randomly-sampled camera ray for the pixel at location i,j, originating from the camera defocus disk
    /// </summary>
    /// <param name="i"></param>
    /// <param name="j"></param>
    /// <returns></returns>
    const ray get_ray(int i, int j)
    {
        vec3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);

        // Apply antialiasing
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;
        auto ray_time = random_double(); // for motion blur

        return ray(ray_origin, ray_direction, ray_time);

    }

    /// <summary>
    /// Returns a random point in the square surrounding a pixel at the origin (usefull for antialiasing)
    /// </summary>
    /// <returns></returns>
    const vec3 pixel_sample_square()
    {
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }


private:
    /* Private Camera Variables Here */

    int    image_height;    // Rendered image height
    point3 center;          // Camera center
    point3 pixel00_loc;     // Location of pixel 0, 0
    vec3   pixel_delta_u;   // Offset to pixel to the right
    vec3   pixel_delta_v;   // Offset to pixel below
    vec3   u, v, w;         // Camera frame basis vectors
    vec3   defocus_disk_u;  // Defocus disk horizontal radius
    vec3   defocus_disk_v;  // Defocus disk vertical radius

    void initialize(renderParameters params)
    {
        // Calculate the image height, and ensure that it's at least 1.
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        if (!params.quietMode)
            std::clog << "Image : " << image_width << " x " << image_height << "\n";


        center = lookfrom;

        // Determine viewport dimensions.
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);


        

        // Calculate the u, v, w unit basis vectors for the camera coordinate frame.
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge


  
        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;


        // Calculate the location of the upper left pixel.
        vec3 viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        double defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    point3 defocus_disk_sample() const
    {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    /// <summary>
    /// Fire a given ray and get the hit record
    /// </summary>
    /// <param name="r"></param>
    /// <param name="world"></param>
    /// <returns></returns>
    color ray_color(const ray& r, int depth, const hittable& world) const
    {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
        {
            // return black color
            return color(0, 0, 0);
        }
        
        
        hit_record rec;

        // If the ray hits nothing, return the background color.
        // 0.001 is to fix shadow acne interval
        if (!world.hit(r, interval(0.001, infinity), rec))
        {
            return background;
        }

        // ray hit a world object

        ray scattered;
        color attenuation;
        color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

        if (!rec.mat->scatter(r, rec, attenuation, scattered))
            return color_from_emission;

        color color_from_scatter = attenuation * ray_color(scattered, depth - 1, world);

        return color_from_emission + color_from_scatter;
    }
};

#endif