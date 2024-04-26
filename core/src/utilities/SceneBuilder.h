#pragma once

#include "../primitives/hittable_list.h"
#include "../materials/material.h"
#include "../primitives/hittable.h"
#include "types.h"
#include "../textures/texture.h"
#include "../cameras/camera.h"
#include <ostream>
#include <vector>
#include <string>
#include <map>




typedef struct {
    int height;
    int width;
    int depth;
    int spp;
    color backgroundColor;
} imageConfig_t;

typedef struct {
    double aspectRatio;
    double openingTime;
    point3 lookFrom;
    point3 lookAt;
    point3 upAxis;
    double aperture;
    double focus;
    double fov;
} cameraConfig_t;




class SceneBuilder
{
    public:
        SceneBuilder();
        ~SceneBuilder() = default;

        // Getters:
        //[[nodiscard]] camera getCamera() const;
        //[[nodiscard]] hittable_list getScene() const;
        [[nodiscard]] imageConfig_t getImageConfig() const;
        //[[nodiscard]] cameraConfig_t getCameraConfig() const;
        //[[nodiscard]] std::map<std::string, std::shared_ptr<texture>> getTextures() const;
        //[[nodiscard]] std::map<std::string, std::shared_ptr<material>> getMaterials() const;

        // Image:
        SceneBuilder& imageConfig(const imageConfig_t& config);
        SceneBuilder& imageSize(int width, int height);
        SceneBuilder& imageWidth(int width);
        SceneBuilder& imageHeight(int height);
        SceneBuilder& imageWidthWithAspectRatio(double aspectRatio);
        SceneBuilder& imageHeightWithAspectRatio(double aspectRatio);
        SceneBuilder& imageDepth(int depth);
        SceneBuilder& imageSamplesPerPixel(int samplesPerPixel);
        SceneBuilder& imageBackgroundColor(color c);

        // Camera:
        //SceneBuilder& cameraConfig(const cameraConfig_t& config);
        //SceneBuilder& cameraAspectRatio(double aspectRatio);
        //SceneBuilder& cameraOpeningTime(double time);
        //SceneBuilder& cameraLookFrom(point3 point);
        //SceneBuilder& cameraLookAt(point3 lookAt);
        //SceneBuilder& cameraUpAxis(point3 vUp);
        //SceneBuilder& cameraAperture(double aperture);
        //SceneBuilder& cameraFocus(double focus);
        //SceneBuilder& cameraFOV(double fov);

        //// Textures:
        //SceneBuilder& addTexture(const std::string& setTextureName, const std::shared_ptr<texture>& texture);
        //SceneBuilder& addSolidColorTexture(const std::string& setTextureName, color rgb);
        //SceneBuilder& addChessBoardTexture(const std::string& setTextureName, color rgbLight, color rgbDark);
        //SceneBuilder& addChessBoardTexture(const std::string& setTextureName, const std::string& textureLight, const std::string &textureDark);
        //SceneBuilder& addNoiseTexture(const std::string &setTextureName, double scale = 1.0);

        //// Materials:
        //SceneBuilder& addMaterial(const std::string& materialName, const std::shared_ptr<material>& material);
        //SceneBuilder& addGlassMaterial(const std::string& materialName, double refraction);
        //SceneBuilder& addSolidMaterial(const std::string& materialName, color rgb);
        //SceneBuilder& addSolidMaterial(const std::string& materialName, const std::string& textureName);
        //SceneBuilder& addMetalMaterial(const std::string& materialName, color rgb, double fuzz);

        //// Lights:
        //SceneBuilder& addDirectionalLightMaterial(const std::string& materialName, color rgb);
        //SceneBuilder& addDirectionalLightMaterial(const std::string& materialName, const std::string& textureName);
        //SceneBuilder& setAmbianceLight(color rgb);

        //// Primitives
        //SceneBuilder& addObject(const std::shared_ptr<hittable>& obj);
        SceneBuilder& addSphere(point3 pos, double radius, const std::string& material);
        //SceneBuilder& addPlane(point3 p0, point3 p1, const std::string& material);
        //SceneBuilder& addBox(point3 p0, point3 p1, const std::string& material);
        //SceneBuilder& addCylinder(point3 pos, double radius, double height, const std::string& material);
        //SceneBuilder& addCone(point3 pos, double radius, double height, const std::string& material);

        //// Transform utils:
        //SceneBuilder& translate(point3 trs);
        //SceneBuilder& rotateY(double angle);

    private:
		imageConfig_t _config{};
		cameraConfig_t _camera{};

		std::map<std::string, std::shared_ptr<texture>> _textures{};
		std::map<std::string, std::shared_ptr<material>> _materials{};
		hittable_list _objects{};
};

std::ostream& operator<<(std::ostream& os, const SceneBuilder& builder);
