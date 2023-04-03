#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <unordered_map>
#include <string>

using namespace std;

//Singleton class
class ResourceManager {
private:
	ResourceManager();
	ResourceManager(ResourceManager const&);
	ResourceManager operator=(ResourceManager const&);

	unordered_map<string, SDL_Surface*> mSurfaces;
	
public:
	static ResourceManager& GetInstance();

	SDL_Surface* GetSurface(string path);

};


#endif
