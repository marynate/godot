#import "sdk/GADInterstitial.h"

@interface AdMobInterstitial: UIViewController <GADInterstitialDelegate> {
  GADInterstitial *interstitial_;
  NSString* interstitialID;
  BOOL testMode;
  NSArray* testDevices;
  BOOL hasReceiveAd;
  BOOL hasDismissScreen;
  BOOL hasFailedReceiveAd;
  BOOL hasLeaveApp;
  BOOL hasPresetScreen;
}

@property(nonatomic, strong) GADInterstitial *interstitial;

- (void)initialize:(NSString*)p_interstitialID testMode:(BOOL)p_testMode  testDevices:(NSArray*)p_testDevices;
- (void)loadInterstitial;
- (void)showInterstitial;
- (BOOL)HasReceiveAd;
- (BOOL)HasDismissScreen;
- (BOOL)HasFailedToReceive;
- (BOOL)HasLeaveApplication;
- (BOOL)HasPresentScreen;
@end
