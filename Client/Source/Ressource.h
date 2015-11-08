#pragma once

class Ressource{

public:

	virtual ~Ressource() {

	}

	virtual bool load() = 0;

};