#include <engine.hpp>

class Level : public Behaviour
{
private:
	/* data */
public:
	Level ( )
	{
		Informations = {
			"Level",
			1.0,
			"This describes a level layout: setting, caracters, donors, must be hinerited"
		};
	}
};