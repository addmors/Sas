#include <SasPre.h>


class Sendbox : public Sas::Application {
public:

	Sendbox() {
 	}
	
	~Sendbox() {
	}

};

Sas::Application* Sas::CreateApplication() {
	return new Sendbox();
}