#import "sdk/GADInterstitial.h"

@interface AdMobInterstitial: UIViewController <GADInterstitialDelegate> {
  GADInterstitial *interstitial_;
  NSString* admobID;
  BOOL hasReceiveAd;
  BOOL hasDismissScreen;
  BOOL hasFailedReceiveAd;
  BOOL hasLeaveApp;
  BOOL hasPresetScreen;
}

@property(nonatomic, retain) GADInterstitial *interstitial;

- (void)initialize:(NSString*)admobId;
- (void)showInterstitial;
- (BOOL)HasReceiveAd;
- (BOOL)HasDismissScreen;
- (BOOL)HasFailedToReceive;
- (BOOL)HasLeaveApplication;
- (BOOL)HasPresentScreen;
@end
