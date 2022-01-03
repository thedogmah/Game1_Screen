#pragma once
#include "Population.h"
class PopulationScooter : public Population
{

public:
    PopulationScooter();

    bool populate();

    bool drawPeople(float, float, float);

};

