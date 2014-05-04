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
  ERR_FAIL_COND(!Globals::get_singleton()->has("admob/api_key"));
  AdMobId = GLOBAL_DEF("admob/api_key", "");
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
  ObjectTypeDB::bind_method(_MD("ShowBanner"),&AdMob_iOS::ShowBanner);
  ObjectTypeDB::bind_method(_MD("HideBanner"),&AdMob_iOS::HideBanner);
  ObjectTypeDB::bind_method(_MD("SetBannerBottomLeft"),&AdMob_iOS::SetBannerBottomLeft);
  ObjectTypeDB::bind_method(_MD("SetBannerTopLeft"),&AdMob_iOS::SetBannerTopLeft);
  ObjectTypeDB::bind_method(_MD("ShowInterstitial"),&AdMob_iOS::ShowInterstitial);
  ObjectTypeDB::bind_method(_MD("HasReceiveAd"),&AdMob_iOS::HasReceiveAd);
  ObjectTypeDB::bind_method(_MD("HasDismissScreen"),&AdMob_iOS::HasDismissScreen);
  ObjectTypeDB::bind_method(_MD("HasFailedToReceive"),&AdMob_iOS::HasFailedToReceive);
  ObjectTypeDB::bind_method(_MD("HasLeaveApplication"),&AdMob_iOS::HasLeaveApplication);
  ObjectTypeDB::bind_method(_MD("HasPresentScreen"),&AdMob_iOS::HasPresentScreen);
};

void AdMob_iOS::Initialize()
{
  if (initialized) return;
  
  NSLog(@"AdMob_iOS: Initialize()");
  initialized = true;
  
  NSString *idStr = [NSString stringWithCString:AdMobId.utf8().get_data() encoding:NSUTF8StringEncoding];

  banner = [AdMobBanner alloc];
  [banner initialize:idStr];
  
  interstitial = [AdMobInterstitial alloc];
  [interstitial initialize:idStr];
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

void AdMob_iOS::SetBannerBottomLeft()
{
};

void AdMob_iOS::SetBannerTopLeft()
{
};

void AdMob_iOS::ShowInterstitial()
{
  if (initialized)
  {
    NSLog(@"AdMob_iOS: ShowInterstitial()");  
    [interstitial showInterstitial];
  }    
};

bool AdMob_iOS::HasReceiveAd()
{
  if (initialized)
  {
    NSLog(@"AdMob_iOS: HasReceiveAd()");
    return [interstitial HasReceiveAd];
  }   
  return false;
};

bool AdMob_iOS::HasDismissScreen()
{
  if (initialized)
  {
    NSLog(@"AdMob_iOS: HasDismissScreen()");
    return [interstitial HasDismissScreen];
  }    
  return false;  
};

bool AdMob_iOS::HasFailedToReceive()
{
  if (initialized)
  {
    NSLog(@"AdMob_iOS: HasFailedToReceive()");
    return [interstitial HasFailedToReceive];
  }    
  return false;  
};

bool AdMob_iOS::HasLeaveApplication()
{
  if (initialized)
  {
    NSLog(@"AdMob_iOS: HasLeaveApplication()");
    return [interstitial HasLeaveApplication];
  }    
  return false;  
};

bool AdMob_iOS::HasPresentScreen()
{
  if (initialized)
  {
    NSLog(@"AdMob_iOS: HasPresentScreen()");
    return [interstitial HasPresentScreen];
  }    
  return false;  
};
