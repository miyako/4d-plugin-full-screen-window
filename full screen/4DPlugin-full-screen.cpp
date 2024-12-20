/* --------------------------------------------------------------------------------
 #
 #  4DPlugin-full-screen.cpp
 #	source generated by 4D Plugin Wizard
 #	Project : full screen
 #	author : miyako
 #	2024/12/16
 #  
 # --------------------------------------------------------------------------------*/

#include "4DPlugin-full-screen.h"

#pragma mark -

void PluginMain(PA_long32 selector, PA_PluginParameters params) {
    
	try
	{
        switch(selector)
        {
			// --- full screen
            
			case 1 :
                PA_RunInMainProcess((PA_RunInMainProcessProcPtr)Get_window_full_screen, params);
				break;
			case 2 :
                PA_RunInMainProcess((PA_RunInMainProcessProcPtr)SET_WINDOW_FULL_SCREEN, params);
//                SET_WINDOW_FULL_SCREEN(params);
				break;

        }

	}
	catch(...)
	{

	}
}

#pragma mark -

#if VERSIONMAC
static NSWindow *PA_GetWindowRef64(PA_WindowRef winId) {
    //EX_GET_HWND has been fixed in 15R3 to return a NSWindow* on mac 64bit.
    //http://forums.4d.fr/Post/EN/15872830/1/17032044
    PA_ulong32 version = (PA_Get4DVersion() & 0x0000FFFF);
    //    int minor = version & 0x000F;
    int r = (version & 0x00F0) >> 4;
    int major = (version & 0xFF00) >> 8;
    
    if (((major >=0x15) && (r >= 3)) || (major >=0x16))
    {
        return (NSWindow *)PA_GetWindowPtr(winId);
    }
    return 0;
}
static BOOL Is_window_full_screen(PA_WindowRef winId) {
    BOOL isWindowFullScreen = NO;
    NSWindow *window = PA_GetWindowRef64(winId);
    isWindowFullScreen = (([window styleMask] & NSFullScreenWindowMask) == NSFullScreenWindowMask);
    return isWindowFullScreen;
}
#endif

void Get_window_full_screen(PA_PluginParameters params) {
    PA_ReturnLong(params, Is_window_full_screen(reinterpret_cast<PA_WindowRef>(PA_GetLongParameter(params, 1))));
}

#define CMD_GET_WINDOW_RECT (443)

void SET_WINDOW_FULL_SCREEN(PA_PluginParameters params) {

    NSWindow *window = PA_GetWindowRef64(reinterpret_cast<PA_WindowRef>(PA_GetLongParameter(params, 1)));
    if(window) {
        if ((!Is_window_full_screen(reinterpret_cast<PA_WindowRef>(PA_GetLongParameter(params, 1))))) {
            //        [window toggleFullScreen:nil];//doesn't work!
            NSButton *zoomButton = [window standardWindowButton:NSWindowZoomButton];
            //        [zoomButton performClick:nil];//doesn't work!
            int x = window.frame.origin.x;
            NSScreen *s = [window screen];
            int y = s.frame.size.height-window.frame.origin.y;
            ;
            NSRect frame = [zoomButton frame];
            CGPoint p;
            p.x = x + frame.origin.x + (frame.size.width / 2);
            p.y = y - window.frame.size.height + (frame.size.height);
            CGEventSourceRef eventSource = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
            CGEventRef e;
            //remember the current mouse position
            CGEventRef oe = CGEventCreate(nil);
            CGPoint op = CGEventGetLocation(oe);
            CFRelease(oe);
            //post click!
            e = CGEventCreateMouseEvent (eventSource, kCGEventMouseMoved ,p , (CGMouseButton)0);
            CGEventPost(kCGHIDEventTap, e);
            CGEventSetType(e, kCGEventLeftMouseDown);
            CGEventPost(kCGHIDEventTap, e);
            CGEventSetType(e, kCGEventLeftMouseUp);
            CGEventPost(kCGHIDEventTap, e);
            CGEventSetType(e, kCGEventMouseMoved);
            CGEventSetLocation(e, op);
            CGEventPost(kCGHIDEventTap, e);
            CFRelease(e);
        }else{

        }
    }
}
