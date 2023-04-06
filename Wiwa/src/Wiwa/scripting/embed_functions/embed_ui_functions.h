#pragma once
#include "../MonoDefinitions.h"

void ActivateGuiCanvas(int id);

void SwapToNewTexture(MonoString* path,int id_canvas,int id_gui);

void SwapText(MonoString* word, int id_canvas, int id_gui);

void SetValueForUIbar(float value,int id_canvas,int id_gui);

void SetNextFrame(int value, int id_canvas, int id_gui);