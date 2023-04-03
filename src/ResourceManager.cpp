#include "ResourceManager.h"
#include <iostream>

ResourceManager::ResourceManager() {

}

ResourceManager::ResourceManager(ResourceManager const&) {

}

ResourceManager ResourceManager::operator=(ResourceManager const& rm)
{
	return *this;
}

ResourceManager& ResourceManager::GetInstance() {
	static ResourceManager* sInstance = new ResourceManager;
	return *sInstance;
}

SDL_Surface* ResourceManager::GetSurface(string path)
{
	//Look through our surfaces map.
	//If surface has been loaded into map, get surface stored in map, else load new surface.
	auto search = mSurfaces.find(path);
	if (search != mSurfaces.end()) {
		return mSurfaces[path];
	}
	else {
		SDL_Surface* surface_tmp = IMG_Load(path.c_str());
		mSurfaces.insert(make_pair(path, surface_tmp));
		return mSurfaces[path];
	}
	return nullptr;
}


