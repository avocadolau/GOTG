#pragma once

void SetFullscreenIntr(bool value);
bool GetFullscreenIntr();

void SetVsyncIntr(bool value);
bool GetVsyncIntr();

void SetResizableIntr(bool value);
bool GetResizableIntr();

void SetResolutionIntr(int width, int height);
int GetWidthIntr();
int GetHeightIntr();