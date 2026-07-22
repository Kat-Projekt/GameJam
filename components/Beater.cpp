// the component for the timer and kill
#include <engine.hpp>

class Beater : public Behaviour
{
private:
	/* data */
public:
	Beater ( )
	{
		Informations = { "Beater", 1.0, "The Count down / life manager" };
	}
};