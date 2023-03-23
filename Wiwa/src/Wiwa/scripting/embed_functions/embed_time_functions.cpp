#include "wipch.h"
#include "embed_time_functions.h"
#include <Wiwa/utilities/time/Time.h>

float GetDeltaTimeIntr()
{
    return Wiwa::Time::GetDeltaTime();
}

float GetTimeIntr()
{
    return Wiwa::Time::GetTime();
}

void SetFPSIntr(int fps)
{
    Wiwa::Time::SetTargetFPS(fps);
}

void SetTimeScaleIntr(float scale)
{
    Wiwa::Time::SetTimeScale(scale);
}

float GetTimeScaleIntr()
{
    return Wiwa::Time::GetTimeScale();
}

void PlayIntr()
{
    Wiwa::Time::Play();
}

void StopIntr()
{
    Wiwa::Time::Stop();
}

void PauseUnPauseIntr()
{
    Wiwa::Time::PauseUnPause();
}

void PauseIntr()
{
    Wiwa::Time::Pause();
}

void UnPauseIntr()
{
    Wiwa::Time::UnPause();
}

bool IsPausedIntr()
{
    return Wiwa::Time::IsPaused();
}

bool IsPlayingIntr()
{
    return Wiwa::Time::IsPlaying();
}
