AdMob
-----

This is the AdMob module for Godot Engine (https://github.com/okamstudio/godot)

How to use
----------
Drop the "admob" directory inside the "modules" directory on the Godot source. Recompile for your platform.

Add the SDKs
------------
Because we're not sure if we can distribute the Admob library, it was removed fromt this repository, you need to download it and add it yourself.

On Android: put the .jar file inside android/
On iOS: put the AdMob sdk inside inside sdk/

Configuring your game
---------------------

To enable the module on Android, add the path to the module to the "modules" property on the [android] section of your engine.cfg file. It should look like this:

[android]
modules="com/android/godot/GodotAdMob"

Note, this is a comma separated list inside a string, you can have other modules there.

On iOS, the module is enabled by defailt.

To configure your API key, set the "api_key" property on the [admob] section on your engine.cfg. Example:

[admob]
api_key="0123456789abcdef"

The singleton "AdMob" will be available on gdscript. Note that you can have override for specific platforms, example:


[admob.Android]
api_key="0123456789abcdef"

[admob.iOS]
api_key="0123456789abcdf0"


API Reference
-------------

The following methods are available:

        void ShowBanner();
        void HideBanner();
        void SetBannerBottomLeft();
        void SetBannerTopLeft();
        void ShowInterstitial();
        bool HasReceiveAd();
        bool HasDismissScreen();
        bool HasFailedToReceive();
        bool HasLeaveApplication();
        bool HasPresentScreen();

On Android, all of them are implemented (on the file android/GodotAdMob.java)
On iOS, the Banner methods might not be impelemented (check "admob_ios.mm")




