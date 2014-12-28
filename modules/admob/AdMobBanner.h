#import "sdk/GADBannerView.h"

@interface AdMobBanner : UIViewController {
	GADBannerView *bannerView_;
	UIView *rootView_;

	NSString* bannerID;
	BOOL smartBanner;
	BOOL testMode;
	NSArray* testDevices;
	NSString* bannerPos;

	BOOL hasReceiveAd;
	BOOL hasDismissScreen;
	BOOL hasFailedReceiveAd;
	BOOL hasLeaveApp;
	BOOL hasPresetScreen;

	NSLayoutConstraint* topConstraint;
	NSLayoutConstraint* bottomConstraint;
	NSLayoutConstraint* leftConstraint;
	NSLayoutConstraint* centerConstraint;
	NSLayoutConstraint* rightConstraint;

	NSLayoutConstraint* verticalConstraint;
	NSLayoutConstraint* horizontalConstraint;
}

- (void)initialize:(NSString*)p_bannerID smartBanner:(BOOL)p_smartBanner testMode:(BOOL)p_testMode  testDevices:(NSArray*)p_testDevices initialPosition:(NSString*)p_initialPosition;

- (void)loadBanner;
- (void)showBanner;
- (void)hideBanner;
- (void)setBannerTopLeft;
- (void)setBannerTopCenter;
- (void)setBannerTopRight;
- (void)setBannerBottomLeft;
- (void)setBannerBottomCenter;
- (void)setBannerBottomRight;

- (BOOL)HasReceiveAd;
- (BOOL)HasDismissScreen;
- (BOOL)HasFailedToReceive;
- (BOOL)HasLeaveApplication;
- (BOOL)HasPresentScreen;

@end
