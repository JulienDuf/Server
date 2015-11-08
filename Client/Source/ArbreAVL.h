#pragma once
#include <algorithm>
#include <iostream>

template <class Clef, class T>

class NoeudArbreAVL {
private:

	NoeudArbreAVL<Clef, T>* parent;
	NoeudArbreAVL<Clef, T>* EG;
	NoeudArbreAVL<Clef, T>* ED;
	T element;
	Clef ID;

	int indiceEquilibre;

public:

	NoeudArbreAVL(Clef ID, T element, NoeudArbreAVL<Clef, T>* _Parent){

		parent = _Parent;
		ED = nullptr;
		EG = nullptr;

		this->element = element;
		this->ID = ID;
		indiceEquilibre = 0;
	}

	NoeudArbreAVL(Clef ID, NoeudArbreAVL<Clef, T>* _Parent){

		parent = _Parent;
		ED = nullptr;
		EG = nullptr;

		this->element = NULL;
		this->ID = ID;
		indiceEquilibre = 0;
	}

	void defenirParent(NoeudArbreAVL<Clef, T>* parent) {

		this->parent = parent;
	}

	void defED(NoeudArbreAVL<Clef, T>* ED) {

		this->ED = ED;
	}

	void defEG(NoeudArbreAVL<Clef, T>* EG) {

		this->EG = EG;
	}

	void defElement(T element) {

		this->element = element;
	}

	void defIndice(int indice) {

		indiceEquilibre = indice;
	}

	NoeudArbreAVL<Clef, T>* obtParent() {

		return parent;
	}

	NoeudArbreAVL<Clef, T>* obtED() {

		return ED;
	}

	NoeudArbreAVL<Clef, T>* obtEG() {

		return EG;
	}

	T& obtElement() {

		return element;
	}

	Clef& obtID() {

		return ID;
	}

	int obtIndice() {

		return indiceEquilibre;
	}

};

template <class Clef, class T>

class ArbreAVL {
private:

    NoeudArbreAVL<Clef, T>* racine; 
    unsigned int compte; 


	NoeudArbreAVL<Clef, T>* rechercher(Clef ID, NoeudArbreAVL<Clef, T>* noeud){

		 if (noeud->obtID() == ID)
			 return noeud;

		 else {

			 if (noeud->obtID() > ID) {
				 if (noeud->obtEG() != nullptr)
					 return rechercher(ID, noeud->obtEG());
				 
				 else
					 return noeud;
			 }

			 if (noeud->obtID() < ID) {
				 if (noeud->obtED() != nullptr)
					 return rechercher(ID, noeud->obtED());
				 
				 else 
					 return noeud;
			 }
		}
	 }

	T& ajouter(Clef ID){

		if (racine == nullptr) {
			racine = new NoeudArbreAVL<Clef, T>(ID, nullptr);
			modifierIndice(racine, false);
			++compte;
			return racine->obtElement();
		}

		else {

			NoeudArbreAVL<Clef, T>* noeud = rechercher(ID, racine);

			if (noeud->obtID() != ID){

				++compte;
				if (noeud->obtID() > ID) {
					noeud->defEG(new NoeudArbreAVL<Clef, T>(ID, noeud));
					noeud = noeud->obtEG();
				}
				else {
					noeud->defED(new NoeudArbreAVL<Clef, T>(ID, noeud));
					noeud = noeud->obtED();
				}

				modifierIndice(rechercher(ID, racine), false);

				return noeud->obtElement();
			}
		}
	}

	void obtMin(NoeudArbreAVL<Clef, T>* racineSousArbre, Clef& ID, T& element ) {

		 while (racineSousArbre->obtED() != nullptr) 
			 racineSousArbre = racineSousArbre->obtED();

		 Clef IDretrait = racineSousArbre->obtID();
		 element = racineSousArbre->obtElement();
		 retrait(IDretrait);
		 ID = IDretrait;
	 }

	void parcoursPrefixe(NoeudArbreAVL<Clef, T>* noeud, std::string* parcours) {

		 char chrTampon[50];
		 _itoa_s(noeud->obtElement(),chrTampon,10);
		 parcours->append(chrTampon);
		 parcours->append(" ");

		 if (noeud->obtEG() != nullptr) 
			 parcoursPrefixe(noeud->obtEG(), parcours);
		 
		 if (noeud->obtED() != nullptr) 
			 parcoursPrefixe(noeud->obtED(), parcours);
	 }

	void parcoursInfixe(NoeudArbreAVL<Clef, T>* noeud, std::string* parcours) {

		 if (noeud->obtEG() != nullptr)
			 parcoursInfixe(noeud->obtEG(), parcours);

		 char chrTampon[50];
		 _itoa_s(noeud->obtElement(), chrTampon, 10);
		 parcours->append(chrTampon);
		 parcours->append(" ");

		 if (noeud->obtED() != nullptr)
			 parcoursInfixe(noeud->obtED(), parcours);
	  }

	void parcoursPostfixe(NoeudArbreAVL<Clef, T>* noeud, std::string* parcours) {

		 if (noeud->obtEG() != nullptr)
			 parcoursPostfixe(noeud->obtEG(), parcours);

		 if (noeud->obtED() != nullptr)
			 parcoursPostfixe(noeud->obtED(), parcours);

		 char chrTampon[50];
		 _itoa_s(noeud->obtElement(), chrTampon, 10);
		 parcours->append(chrTampon);
		 parcours->append(" ");
	  }

	void rotationG(NoeudArbreAVL<Clef, T>* noeudPivot, bool doubleRotation) {

		 if (noeudPivot->obtED()->obtIndice() < 0 && !doubleRotation)
			 rotationD(noeudPivot->obtED(), true);

		 NoeudArbreAVL<Clef, T>* tmp = noeudPivot->obtED();
		 if (tmp == racine)
			 tmp->defenirParent(nullptr);

		 else {

			 tmp->defenirParent(noeudPivot->obtParent());
			 if (tmp->obtParent() != nullptr) {
				 if (tmp->obtParent()->obtID() < tmp->obtID())
					 tmp->obtParent()->defED(tmp);
				 else
					 tmp->obtParent()->defEG(tmp);
			 }
		 }

		 if (tmp->obtEG() != nullptr) {
			 noeudPivot->defED(tmp->obtEG());
			 noeudPivot->obtED()->defenirParent(noeudPivot);
		 }

		 else
			 noeudPivot->defED(nullptr);

		 tmp->defEG(noeudPivot);
		 noeudPivot->defenirParent(tmp);

		 if (noeudPivot == racine)
			 racine = tmp;

		 noeudPivot->defIndice(noeudPivot->obtIndice() - 1 - std::max(0, tmp->obtIndice()));
		 tmp->defIndice(tmp->obtIndice() - 1 + std::min(noeudPivot->obtIndice(), 0));
	 }

	void rotationD(NoeudArbreAVL<Clef, T>* noeudPivot, bool doubleRotation) {
		 
		 if (noeudPivot->obtEG()->obtIndice() > 0 && !doubleRotation)
			 rotationG(noeudPivot->obtEG(), true); 

		 NoeudArbreAVL<Clef, T>* tmp = noeudPivot->obtEG();

		 if (tmp == racine)
		    tmp->defenirParent(nullptr);
		 
		 else {

			 tmp->defenirParent(noeudPivot->obtParent());
			 if (tmp->obtParent() != nullptr) {
				 if (tmp->obtParent()->obtID() < tmp->obtID())
					 tmp->obtParent()->defED(tmp);
				 else
					 tmp->obtParent()->defEG(tmp);
			 }
		 }

		 if (tmp->obtED() != nullptr) {
			 noeudPivot->defEG(tmp->obtED());
			 noeudPivot->obtEG()->defenirParent(noeudPivot);
		 }

		 else
			 noeudPivot->defEG(nullptr);

		 tmp->defED(noeudPivot);
		 noeudPivot->defenirParent(tmp);

		 if (noeudPivot == racine)
			 racine = tmp;

		 noeudPivot->defIndice(noeudPivot->obtIndice() + 1 - std::min(tmp->obtIndice(), 0));
		 tmp->defIndice(tmp->obtIndice() + 1 + std::max(0, noeudPivot->obtIndice()));
	 }

	bool modifierIndice(NoeudArbreAVL<Clef, T>* noeud, bool _boretrait) {

		 if (noeud != nullptr && noeud->obtParent() != nullptr) {

			 if (noeud->obtIndice() == 2) {
				 rotationG(noeud, false);
				 return true;
			 }

			 if (noeud->obtIndice() == -2) {
				 rotationD(noeud, false);
				 return true;
			 }

			 if (noeud->obtIndice() != 0 && _boretrait)
				 return true;

			 if (noeud->obtID() > noeud->obtParent()->obtID()) {
				 if (!_boretrait)
					noeud->obtParent()->defIndice(noeud->obtParent()->obtIndice() + 1);
				 else
					 noeud->obtParent()->defIndice(noeud->obtParent()->obtIndice() - 1);
			 }
			 else {
				 if (!_boretrait)
					 noeud->obtParent()->defIndice(noeud->obtParent()->obtIndice() - 1);
				 else
					 noeud->obtParent()->defIndice(noeud->obtParent()->obtIndice() + 1);
			 }

			 if (noeud->obtParent()->obtIndice() == 0 && !_boretrait)
				 return true;

			 modifierIndice(noeud->obtParent(), _boretrait);
		 }

		 if (noeud->obtIndice() == 2) {
			 rotationG(noeud, false);
			 return true;
		 }

		 if (noeud->obtIndice() == -2) {
			 rotationD(noeud, false);
			 return true;
		 }
	 }

public:

	ArbreAVL(void){

		racine = nullptr;
		compte = 0;

	}

	~ArbreAVL() {

		//int iNombre = 0;
		//int iNombre2 = -1;

		//while (racine != nullptr) {

		//	retrait(iNombre);
		//	retrait(iNombre2);
		//	iNombre++;
		//	iNombre2--;
		//}
	}

	T& operator[](Clef ID) {

		if (rechercher(ID))
			return rechercher(ID, racine)->obtElement();

		else {
			return ajouter(ID);
		}
	}

	bool rechercher(Clef ID){
	 
		if (racine == nullptr)
			return false;

		if (rechercher(ID, racine)->obtID() == ID)
			return true;

		else return false;
	 }

	void retrait(Clef ID) {

		 if (rechercher(ID)) {

			 NoeudArbreAVL<Clef, T>* pRunner = rechercher(ID, racine);

			 if ((pRunner->obtED() == nullptr) && (pRunner->obtEG() == nullptr)) {

				 modifierIndice(pRunner, true);

				 if (pRunner->obtParent() != nullptr) {

					 if (pRunner->obtID() < pRunner->obtParent()->obtID())
						 pRunner->obtParent()->defEG(nullptr);

					 else
						 pRunner->obtParent()->defED(nullptr);
				 }

				 delete pRunner;
				 compte--;
			 }
			 else {

				 if ((pRunner->obtED() != nullptr) ^ (pRunner->obtEG() != nullptr)) {

					 modifierIndice(pRunner, true);

					 if (pRunner->obtEG() != nullptr) {

						 if (pRunner->obtParent() != nullptr) {

							 if (pRunner->obtID() < pRunner->obtParent()->obtID())
								 pRunner->obtParent()->defEG(pRunner->obtEG()); 
							 else
								 pRunner->obtParent()->defED(pRunner->obtEG());

							 pRunner->obtEG()->defenirParent(pRunner->obtParent());
						 }

						 else {
							 pRunner->obtEG()->defenirParent(nullptr);
							 racine = pRunner->obtEG();
						 }
					 }

					 if (pRunner->obtED() != nullptr) {

						 if (pRunner->obtParent() != nullptr) {

							 if (pRunner->obtID() < pRunner->obtParent()->obtID())
								 pRunner->obtParent()->defEG(pRunner->obtED()); 
							 else
								 pRunner->obtParent()->defED(pRunner->obtED());

							 pRunner->obtED()->defenirParent(pRunner->obtParent());
						 }

						 else {
							 pRunner->obtED()->defenirParent(nullptr);
							 racine = pRunner->obtED();
						 }
					 }

					 delete pRunner;
					 compte--;
				 }

				 else {

					 obtMin(pRunner->obtEG(), pRunner->obtID(), pRunner->obtElement());
				 }
			 }
		 }
		 if (compte == 0)
			 racine = nullptr;
	 }

	std::string* parcoursPrefixe(void) {

		 string* strParcours = new string();

		 if (racine == nullptr) {
			 strParcours->append("");
			 return strParcours;
		 }
		 else {
			 parcoursPrefixe(racine, strParcours);
			 return strParcours;
		 }
	 }

	std::string* parcoursInfixe(void) {

		 string* strParcours = new string();

		 if (racine == nullptr) {
			 strParcours->append("");
			 return strParcours;
		 }
		 else {
			 parcoursInfixe(racine, strParcours);
			 return strParcours;
		 }
	 
	 }

	std::string* parcoursPostfixe(void) {

		 string* strParcours = new string();

		 if (racine == nullptr) {
			 strParcours->append("");
			 return strParcours;
		 }
		 else {
			 parcoursPostfixe(racine, strParcours);
			 return strParcours; 
		 }
	 }

	unsigned int obtenirCompte(void) {

		 return compte;
	 }

};