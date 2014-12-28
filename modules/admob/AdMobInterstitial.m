#import "AdMobInterstitial.h"
#import "app_delegate.h"

@implementation AdMobInterstitial

@synthesize interstitial = interstitial_;

- (void)dealloc {
	interstitial_.delegate = nil;
	[interstitial_ release];
    [interstitialID release];
	[super dealloc];
}

- (void)viewDidLoad {
  [super viewDidLoad];
  NSLog(@"AdMobInterstitial: viewDidLoad()");
}

- (void)initialize:(NSString*)p_interstitialID testMode:(BOOL)p_testMode  testDevices:(NSArray*)p_testDevices
{
	interstitialID = [p_interstitialID copy];
	testMode = p_testMode;
	testDevices = [p_testDevices copy];
/*
	self.interstitial = [[[GADInterstitial alloc] init] autorelease];
	self.interstitial.delegate = self;
	self.interstitial.adUnitID = interstitialID;
*/
  NSLog(@"AdMobInterstitial: initialize()");
}

- (void)loadInterstitial
{
    NSLog(@"1");
    if (interstitial_)
    {
        NSLog(@"2");
        interstitial_.delegate = nil;
        [interstitial_ release];
    }
    NSLog(@"3");
	interstitial_ = [[GADInterstitial alloc] init];
    NSLog(@"4");
	interstitial_.delegate = self;
    NSLog(@"5");
	interstitial_.adUnitID = interstitialID;
       NSLog(@"6: %@", interstitialID);
	GADRequest *request = [GADRequest request];
    NSLog(@"7 %@", testDevices);
	if (testMode) {
	  request.testDevices = testDevices;
	}
        NSLog(@"8");

	[interstitial_ loadRequest: request];

    NSLog(@"9");
	hasReceiveAd = NO;
	hasDismissScreen = NO;
	hasFailedReceiveAd = NO;
	hasLeaveApp = NO;
	hasPresetScreen = NO;
	NSLog(@"AdMobInterstitial: loadInterstitial()");
}

- (void)showInterstitial
{
	if (interstitial_ && interstitial_.isReady)
	{
		[interstitial_ presentFromRootViewController:[AppDelegate getViewController]];
		NSLog(@"AdMobInterstitial: showInterstitial()");
	}
}

#pragma mark GADInterstitialDelegate implementation

- (void)interstitialDidReceiveAd:(GADInterstitial *)interstitial
{
	//[interstitial_ presentFromRootViewController:[AppDelegate getViewController]];
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
