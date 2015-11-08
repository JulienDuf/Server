#pragma once
template<class T>
class Vecteur3{
public:
	T x, y, z;

	Vecteur3(T x = 0, T y = 0, T z = 0) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	T norme() {
		return sqrt((x * x) + (y * y) + (z * z));
	}
	
	void normaliser() {
		T module = norme();
		x /= module;
		y /= module;
		z /= module;
	}

	void inverser() {
		x = -x;
		y = -y;
		z = -z;
	}
	
	bool operator==(Vecteur3<T> autre) {
		if (x == autre.x && y == autre.y && z == autre.z)
			return true;
		return false;
	}

	void operator+=(Vecteur3<T> autre) {
		x += autre.x;
		y += autre.y;
		z += autre.z;
	}
	
	void operator-=(Vecteur3<T> autre) {
		x -= autre.x;
		y -= autre.y;
		z -= autre.z;
	}
	
	void operator/=(Vecteur3<T> autre) {
		x /= autre.x;
		y /= autre.y;
		z /= autre.z;
	}

	void operator/=(double scalaire) {
		x /= scalaire;
		y /= scalaire;
		z /= scalaire;
	}

	void operator*=(Vecteur3<T> autre) {
		x *= autre.x;
		y *= autre.y;
		z *= autre.z;
	}

	void operator*=(double scalaire) {
		x *= scalaire;
		y *= scalaire;
		z *= scalaire;
	}

	void soustraire(double scalaire) {
		x -= scalaire;
		y -= scalaire;
		y -= scalaire;
	}

	void produitParUnVecteur(Vecteur3<T> autre) {
		x *= autre.x;
		y *= autre.y;
		z *= autre.z;
	}

	T produitScalaire(Vecteur3<T> autre) {
		return ((this->x * autre.x) + (this->y * autre.y) + (this->z * autre.z));
	}

	Vecteur3<T> produitVectoriel(Vecteur3<T> autre) {
		Vecteur3<T> vectP;
		vectP.x = (this->y * autre.z) - (this->z * autre.y);
		vectP.y = (this->z * autre.x) - (this->x * autre.z);
		vectP.z = (this->x * autre.y) - (this->y * autre.x);
		return vectP;
	}

	double angleEntreVecteurs(Vecteur3<T> autre) {

		return acos(this->produitScalaire(autre) / (this->norme() * autre.norme()));

	}

	Vecteur3<T> operator*(double scalaire) {
		return Vecteur3<T>(x * scalaire, y * scalaire, z * scalaire);
	}

	Vecteur3<T> operator*(Vecteur3<T> vect) {
		return Vecteur3<T>(x * vect.x, y * vect.y, z * vect.z);
	}

	Vecteur3<T> operator/(double scalaire) {
		return Vecteur3<T>(x / scalaire, y / scalaire, z / scalaire);
	}

	Vecteur3<T> operator+(Vecteur3<T> autre) {
		return Vecteur3<T>(x + autre.x, y + autre.y, z + autre.z);
	}

	Vecteur3<T> operator-(Vecteur3<T> autre) {
		return Vecteur3<T>(x - autre.x, y - autre.y, z - autre.z);
	}
};
	
typedef Vecteur3<int> Vecteur3i;
typedef Vecteur3<unsigned int> Vecteur3ui;
typedef Vecteur3<float> Vecteur3f;
typedef Vecteur3<double> Vecteur3d;
