def can_build(plat):
	return plat =="iphone" or plat =="isim" or plat=="android"

def configure(env):

	if (env['platform'] == "iphone" or env['platform'] == "isim"):
		
		env.Append(LINKFLAGS=['-ObjC','-framework','AdSupport','-framework','AudioToolbox','-framework','AVFoundation','-framework','CoreGraphics','-framework','CoreTelephony','-framework','MessageUI','-framework','StoreKit','-framework','SystemConfiguration','-framework','EventKit','-framework','EventKitUI'])
		env.Append(LIBPATH=['#modules/admob/sdk'])
		env.Append(LIBS=['GoogleAdMobAds'])

	if (env['platform'] == 'android'):
		
		#env.android_module_library("android/GoogleAdMobAdsSdk-6.4.1.jar")
		env.android_module_file("android/GodotAdMob.java")
		env.android_module_manifest("android/AndroidManifestChunk.xml")
		env.disable_module()

