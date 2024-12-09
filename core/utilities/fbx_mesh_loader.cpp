#include "fbx_mesh_loader.h"





#include <filesystem>
#include <map>

fbx_mesh_loader::fbx_mesh_loader()
{
}

bool fbx_mesh_loader::load_model_from_file(const std::string& filepath)
{
    std::filesystem::path dir(std::filesystem::current_path());
    std::filesystem::path file(filepath);
    std::filesystem::path fullPath = dir / file;

    std::cout << "[INFO] Loading FBX file " << fullPath.filename() << std::endl;

    auto fullAbsPath = std::filesystem::absolute(fullPath);
    if (!std::filesystem::exists(fullAbsPath)) {
        std::cerr << "[ERROR] FBX file not found: " << fullAbsPath << std::endl;
        return false;
    }

    std::string inputFile = fullAbsPath.generic_string();
    
	// Ignoring certain nodes will only stop them from being processed not tokenised (i.e. they will still be in the tree)
	//ofbx::LoadFlags flags =
	//	//		ofbx::LoadFlags::IGNORE_MODELS |
	//	ofbx::LoadFlags::IGNORE_BLEND_SHAPES |
	//	//ofbx::LoadFlags::IGNORE_CAMERAS |
	//	//ofbx::LoadFlags::IGNORE_LIGHTS |
	//	////		ofbx::LoadFlags::IGNORE_TEXTURES |
	//	//ofbx::LoadFlags::IGNORE_SKIN |
	//	//ofbx::LoadFlags::IGNORE_BONES |
	//	//ofbx::LoadFlags::IGNORE_PIVOTS |
	//	////		ofbx::LoadFlags::IGNORE_MATERIALS |
	//	//ofbx::LoadFlags::IGNORE_POSES |
	//	//ofbx::LoadFlags::IGNORE_VIDEOS |
	//	////ofbx::LoadFlags::IGNORE_LIMBS |
	//	////		ofbx::LoadFlags::IGNORE_MESHES |
	//	//ofbx::LoadFlags::IGNORE_ANIMATIONS;

	////g_scene = ofbx::load((ofbx::u8*)content, file_size, (ofbx::u16)flags);

    return true;
}
