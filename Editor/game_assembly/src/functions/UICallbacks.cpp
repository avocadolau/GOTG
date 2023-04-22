#include "UICallbacks.h"
#include <iostream>

void MusicSliderCallback(float perc)
{
	std::cout << "Music slider callback: " << perc << std::endl;
}