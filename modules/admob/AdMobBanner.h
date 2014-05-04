#import "sdk/GADBannerView.h"

@interface AdMobBanner : UIViewController {
  GADBannerView *bannerView_;
  NSString* admobID;
}

- (void)initialize:(NSString*)admobId;
- (void)showBanner;
- (void)hideBanner;
@end
