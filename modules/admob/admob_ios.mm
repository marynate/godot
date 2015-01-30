#import "AdMobBanner.h"
#import "AdMobInterstitial.h"
#import "admob_ios.h"
#include "core/globals.h"

AdMob_iOS* AdMob_iOS::instance = NULL;

AdMob_iOS::AdMob_iOS()
{ 
  ERR_FAIL_COND(instance != NULL);
  instance = this;
  initialized = false;

  banner_id = GLOBAL_DEF("admob/banner_id", "");
  interstitial_id = GLOBAL_DEF("admob/interstitial_id", "");
  smart_banner = GLOBAL_DEF("admob/smart_banner", "false");
    print_line("smart_banner: " + smart_banner.to_lower());
  test_mode = GLOBAL_DEF("admob/test_mode", "true");
  test_devices = GLOBAL_DEF("admob/test_devices", "");
  banner_pos = GLOBAL_DEF("admob/banner_pos", "bottomcenter");
  banner_pos = banner_pos.strip_edges().to_lower();

  Initialize();
};

AdMob_iOS::~AdMob_iOS()
{
};

AdMob_iOS* AdMob_iOS::get_singleton() {
  return instance;
};

void AdMob_iOS::_bind_methods() 
{
	ObjectTypeDB::bind_method(_MD("LoadBanner"),&AdMob_iOS::LoadBanner);
	ObjectTypeDB::bind_method(_MD("ShowBanner"),&AdMob_iOS::ShowBanner);
	ObjectTypeDB::bind_method(_MD("HideBanner"),&AdMob_iOS::HideBanner);
	ObjectTypeDB::bind_method(_MD("SetBannerTopLeft"),&AdMob_iOS::SetBannerTopLeft);
	ObjectTypeDB::bind_method(_MD("SetBannerTopRight"),&AdMob_iOS::SetBannerTopRight);
	ObjectTypeDB::bind_method(_MD("SetBannerTopCenter"),&AdMob_iOS::SetBannerTopCenter);
	ObjectTypeDB::bind_method(_MD("SetBannerBottomLeft"),&AdMob_iOS::SetBannerBottomLeft);
	ObjectTypeDB::bind_method(_MD("SetBannerBottomRight"),&AdMob_iOS::SetBannerBottomRight);
	ObjectTypeDB::bind_method(_MD("SetBannerBottomCenter"),&AdMob_iOS::SetBannerBottomCenter);

	ObjectTypeDB::bind_method(_MD("HasReceiveAd"),&AdMob_iOS::HasReceiveAd);
	ObjectTypeDB::bind_method(_MD("HasDismissScreen"),&AdMob_iOS::HasDismissScreen);
	ObjectTypeDB::bind_method(_MD("HasFailedToReceive"),&AdMob_iOS::HasFailedToReceive);
	ObjectTypeDB::bind_method(_MD("HasLeaveApplication"),&AdMob_iOS::HasLeaveApplication);
	ObjectTypeDB::bind_method(_MD("HasPresentScreen"),&AdMob_iOS::HasPresentScreen);

	ObjectTypeDB::bind_method(_MD("LoadInterstitial"),&AdMob_iOS::LoadInterstitial);
	ObjectTypeDB::bind_method(_MD("ShowInterstitial"),&AdMob_iOS::ShowInterstitial);

	ObjectTypeDB::bind_method(_MD("HasInterstitialReceiveAd"),&AdMob_iOS::HasInterstitialReceiveAd);
	ObjectTypeDB::bind_method(_MD("HasInterstitialDismissScreen"),&AdMob_iOS::HasInterstitialDismissScreen);
	ObjectTypeDB::bind_method(_MD("HasInterstitialFailedToReceive"),&AdMob_iOS::HasInterstitialFailedToReceive);
	ObjectTypeDB::bind_method(_MD("HasInterstitialLeaveApplication"),&AdMob_iOS::HasInterstitialLeaveApplication);
	ObjectTypeDB::bind_method(_MD("HasInterstitialPresentScreen"),&AdMob_iOS::HasInterstitialPresentScreen);

};

void AdMob_iOS::Initialize()
{
	if (initialized) return;

	NSLog(@"AdMob_iOS: Initialize()");
	initialized = true;
  
	NSString *banner_id_str = [NSString stringWithCString:banner_id.utf8().get_data() encoding:NSUTF8StringEncoding];
	NSString *interstitial_id_str = [NSString stringWithCString:interstitial_id.utf8().get_data() encoding:NSUTF8StringEncoding];
	BOOL is_smart_banner = ("true" == smart_banner.to_lower()) ? YES : NO;
	BOOL is_test_mode = ("true" == test_mode.to_lower() )? YES : NO;

	NSString *test_devices_str			= [[NSString stringWithCString:test_devices.utf8().get_data() encoding:NSUTF8StringEncoding]
													stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
	NSArray *test_devices_arr			= [test_devices_str componentsSeparatedByString:@","];
	NSMutableArray *test_devices_marr	= [NSMutableArray array];

	[test_devices_marr addObject:GAD_SIMULATOR_ID];
	[test_devices_marr addObjectsFromArray:test_devices_arr];
    
	NSString *banner_pos_str = [NSString stringWithCString:banner_pos.utf8().get_data() encoding:NSUTF8StringEncoding];

    NSLog(@"banner_id: %@", banner_id_str);
    NSLog(@"interstitial_id: %@", interstitial_id_str);
    NSLog(@"is_smart_banner: %@", is_smart_banner?@"YES":@"NO");
    NSLog(@"is_test_mode: %@", is_test_mode?@"YES":@"NO");
    NSLog(@"test_devices_marr: %@", test_devices_marr);
    
	banner = [AdMobBanner alloc];
	[banner initialize:banner_id_str smartBanner:is_smart_banner testMode:is_test_mode testDevices:[NSArray arrayWithArray:test_devices_marr] initialPosition:banner_pos_str];

	interstitial = [AdMobInterstitial alloc];
	[interstitial initialize:interstitial_id_str testMode:is_test_mode testDevices:[NSArray arrayWithArray:test_devices_marr]];
};

void AdMob_iOS::LoadBanner()
{
  if (initialized)
  {
	NSLog(@"AdMob_iOS: LoadBanner()");
	[banner loadBanner];
  }
};

void AdMob_iOS::ShowBanner()
{
  if (initialized)
  {
	NSLog(@"AdMob_iOS: ShowBanner()");
	[banner showBanner];
  }
};

void AdMob_iOS::HideBanner()
{
  if (initialized)
  {
    NSLog(@"AdMob_iOS: HideBanner()");
    [banner hideBanner];
  }
};

void AdMob_iOS::SetBannerTopLeft()
{
	if (initialized)
	{
		NSLog(@"AdMob_iOS: SetBannerTopLeft()");
		[banner setBannerTopLeft];
	}
};

void AdMob_iOS::SetBannerTopRight()
{
	if (initialized)
	{
		NSLog(@"AdMob_iOS: SetBannerTopRight()");
		[banner setBannerTopRight];
	}
};

void AdMob_iOS::SetBannerTopCenter()
{
	if (initialized)
	{
		NSLog(@"AdMob_iOS: SetBannerTopCenter()");
		[banner setBannerTopCenter];
	}
};

void AdMob_iOS::SetBannerBottomLeft()
{
	if (initialized)
	{
		NSLog(@"AdMob_iOS: SetBannerBottomLeft()");
		[banner setBannerBottomLeft];
	}
};

void AdMob_iOS::SetBannerBottomRight()
{
	if (initialized)
	{
		NSLog(@"AdMob_iOS: SetBannerBottomRight()");
		[banner setBannerBottomRight];
	}
};

void AdMob_iOS::SetBannerBottomCenter()
{
	if (initialized)
	{
		NSLog(@"AdMob_iOS: SetBannerBottomCenter()");
		[banner setBannerBottomCenter];
	}
};

bool AdMob_iOS::HasReceiveAd()
{
  if (initialized)
  {
	return [banner HasReceiveAd];
  }
  return false;
};

bool AdMob_iOS::HasDismissScreen()
{
  if (initialized)
  {
	return [banner HasDismissScreen];
  }
  return false;
};

bool AdMob_iOS::HasFailedToReceive()
{
  if (initialized)
  {
	return [banner HasFailedToReceive];
  }
  return false;
};

bool AdMob_iOS::HasLeaveApplication()
{
  if (initialized)
  {
	return [banner HasLeaveApplication];
  }
  return false;
};

bool AdMob_iOS::HasPresentScreen()
{
  if (initialized)
  {
	return [banner HasPresentScreen];
  }
  return false;
};

void AdMob_iOS::LoadInterstitial()
{
  if (initialized)
  {
	NSLog(@"AdMob_iOS: LoadInterstitial()");
	[interstitial loadInterstitial];
  }
};

void AdMob_iOS::ShowInterstitial()
{
  if (initialized)
  {
    NSLog(@"AdMob_iOS: ShowInterstitial()");  
    [interstitial showInterstitial];
  }    
};

bool AdMob_iOS::HasInterstitialReceiveAd()
{
  if (initialized)
  {
	return [interstitial HasReceiveAd];
  }   
  return false;
};

bool AdMob_iOS::HasInterstitialDismissScreen()
{
  if (initialized)
  {
	return [interstitial HasDismissScreen];
  }    
  return false;  
};

bool AdMob_iOS::HasInterstitialFailedToReceive()
{
  if (initialized)
  {
	return [interstitial HasFailedToReceive];
  }    
  return false;  
};

bool AdMob_iOS::HasInterstitialLeaveApplication()
{
  if (initialized)
  {
	return [interstitial HasLeaveApplication];
  }    
  return false;  
};

bool AdMob_iOS::HasInterstitialPresentScreen()
{
  if (initialized)
  {
	return [interstitial HasPresentScreen];
  }    
  return false;  
};
