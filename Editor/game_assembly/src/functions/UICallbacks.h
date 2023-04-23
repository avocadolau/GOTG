#include <Wiwa/utilities/Reflection.h>

// Declare
void GoToMilanoHub();

void GoMainMenu();

void Quit();

void Resume_PauseGame();

void ActivateOptionsCanvas();

void DeActivateOptionsCanvas();

void SetFullScreen(bool ret);

void SetVsync(bool ret);

void SetVolumeMusic(float level);

void SetVolumeFX(float level);

void PlaceHolderButton();

void ActivateOptionsMenu();

void DeActivateOptionsMenu();
// Register
REGISTER_FUNCTION(GoToMilanoHub);
REGISTER_FUNCTION(GoMainMenu);
REGISTER_FUNCTION(Quit);
REGISTER_FUNCTION(Resume_PauseGame);
REGISTER_FUNCTION(ActivateOptionsCanvas);
REGISTER_FUNCTION(DeActivateOptionsCanvas);
REGISTER_FUNCTION(SetFullScreen);
REGISTER_FUNCTION(SetVsync);
REGISTER_FUNCTION(SetVolumeMusic);
REGISTER_FUNCTION(SetVolumeFX);
REGISTER_FUNCTION(PlaceHolderButton);
REGISTER_FUNCTION(ActivateOptionsMenu);
REGISTER_FUNCTION(DeActivateOptionsMenu);