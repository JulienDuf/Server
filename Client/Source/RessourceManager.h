#pragma once
#include <map>
#include "Ressource.h"
#include "Singleton.h"
#include "Window.h"

class RessourceManager : public Singleton<RessourceManager>{

private:

	std::map<std::string, Ressource*> ressources;
	std::list<Ressource*> loadingQueue;

public:

	void addRessource(std::string name, Ressource* ressource) {
		
		if (ressources.find(name) == ressources.end())
			ressources[name] = ressource;
	}

	void removeRessource(std::string name) {
		if (ressources.find(name) != ressources.end())
			ressources.erase(name);
	}

	Ressource* getRessource(std::string name) {
		if (ressources.find(name) != ressources.end())
			return ressources[name];
		
		return nullptr;
	}

	void putRessourceLoading(Ressource* ressource) {

		loadingQueue.push_back(ressource);
	}

	void laodRessources() {

		for (auto it : loadingQueue)
			it->load();

		loadingQueue.clear();
	}
};