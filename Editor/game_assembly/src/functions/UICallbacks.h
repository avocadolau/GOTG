#include <Wiwa/utilities/Reflection.h>
enum class GoToMilanoHub_ {hola};
enum class GoMainMenu_ { hola };
enum class Quit_ { hola };
enum class Resume_PauseGame_ { hola };
enum class ActivateOptionsCanvas_ { hola };
enum class DeActivateOptionsCanvas_ { hola };
enum class SetFullScreen_ { hola };
enum class SetVsync_ { hola };
enum class SetVolumeMusic_ { hola };
enum class SetVolumeFX_ { hola };
enum class PlaceHolderButton_ { hola };
enum class ActivateOptionsMenu_ { hola };
enum class DeActivateOptionsMenu_ { hola };

// Declare
GoToMilanoHub_ GoToMilanoHub();

GoMainMenu_ GoMainMenu();

Quit_ Quit();

Resume_PauseGame_ Resume_PauseGame();

ActivateOptionsCanvas_ ActivateOptionsCanvas();

DeActivateOptionsCanvas_ DeActivateOptionsCanvas();

SetFullScreen_ SetFullScreen(bool ret);

SetVsync_ SetVsync(bool ret);

SetVolumeMusic_ SetVolumeMusic(float level);

SetVolumeFX_ SetVolumeFX(float level);

PlaceHolderButton_ PlaceHolderButton();

ActivateOptionsMenu_ ActivateOptionsMenu();

DeActivateOptionsMenu_ DeActivateOptionsMenu();

//// Register
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