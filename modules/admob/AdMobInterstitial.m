#import "AdMobInterstitial.h"
#import "app_delegate.h"

@implementation AdMobInterstitial

@synthesize interstitial = interstitial_;

- (void)dealloc {
  interstitial_.delegate = nil;
  [interstitial_ release];
  [super dealloc];
}

- (void)viewDidLoad {
  [super viewDidLoad];
  NSLog(@"AdMobInterstitial: viewDidLoad()");
}

- (void)initialize:(NSString*)admobId
{
  admobID = admobId;
  NSLog(@"AdMobInterstitial: initialize()");
}

- (void)showInterstitial
{
  self.interstitial = [[[GADInterstitial alloc] init] autorelease];
  self.interstitial.delegate = self;  
  self.interstitial.adUnitID = admobID;  

  GADRequest *request = [GADRequest request];
    
  request.testDevices = [NSArray arrayWithObjects:
      GAD_SIMULATOR_ID, // Simulator
      nil];
    
  [self.interstitial loadRequest: request];  
  
  hasReceiveAd = NO;
  hasDismissScreen = NO;
  hasFailedReceiveAd = NO;
  hasLeaveApp = NO;
  hasPresetScreen = NO;  
  NSLog(@"AdMobInterstitial: showInterstitial()");
}

- (void)interstitialDidReceiveAd:(GADInterstitial *)interstitial
{
  [interstitial_ presentFromRootViewController:[AppDelegate getViewController]];
  hasReceiveAd = YES;
  NSLog(@"AdMobInterstitial: interstitialDidReceiveAd()");
}

- (void)interstitial:(GADInterstitial *)interstitial didFailToReceiveAdWithError:(GADRequestError *)error
{
  hasFailedReceiveAd = YES;
  NSLog(@"AdMobInterstitial: didFailToReceiveAdWithError() %@",error);
}

- (void)interstitialWillPresentScreen:(GADInterstitial *)interstitial
{
  hasPresetScreen = YES;
  NSLog(@"AdMobInterstitial: interstitialWillPresentScreen()");
}

- (void)interstitialDidDismissScreen:(GADInterstitial *)interstitial
{
  hasDismissScreen = YES;
  NSLog(@"AdMobInterstitial: interstitialDidDismissScreen()");
}

- (void)interstitialWillLeaveApplication:(GADInterstitial *)interstitial
{
  hasLeaveApp = YES;
  NSLog(@"AdMobInterstitial: interstitialWillLeaveApplication()");
}

- (BOOL)HasReceiveAd
{
  BOOL tmp = hasReceiveAd;
  hasReceiveAd = NO;
  return tmp;
}

- (BOOL)HasDismissScreen
{
  BOOL tmp = hasDismissScreen;
  hasDismissScreen = NO;
  return tmp;
}

- (BOOL)HasFailedToReceive
{
  BOOL tmp = hasFailedReceiveAd;
  hasFailedReceiveAd = NO;
  return tmp;
}

- (BOOL)HasLeaveApplication
{
  BOOL tmp = hasLeaveApp;
  hasLeaveApp = NO;
  return tmp;
}

- (BOOL)HasPresentScreen
{
  BOOL tmp = hasPresetScreen;
  hasPresetScreen = NO;
  return tmp;
}

@end
