#include <engine.hpp>

class Donator : public Behaviour
{
private:
	/* data */
public:
	Donator ( )
	{
		Informations = { "Donator", 1.0, "This is the single donator manager, it describes how a donor behaves" };
	}
};