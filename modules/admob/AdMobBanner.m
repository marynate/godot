#import "AdMobBanner.h"
#import "app_delegate.h"

@implementation AdMobBanner

- (void)viewDidLoad {
	[super viewDidLoad];
	NSLog(@"AdMobBanner: viewDidLoad()");
}

- (void)viewDidUnload {
  [bannerView_ release];
  NSLog(@"AdMobBanner: viewDidUnload()");
}

- (void)dealloc {
    [bannerID release];
    [bannerPos release];
    [testDevices release];
    [topConstraint release];
	[bottomConstraint release];
	[leftConstraint release];
	[centerConstraint release];
	[rightConstraint release];
    [super dealloc];
}

- (void)initialize:(NSString*)p_bannerID smartBanner:(BOOL)p_smartBanner testMode:(BOOL)p_testMode  testDevices:(NSArray*)p_testDevices initialPosition:(NSString*)p_initialPosition
{
	bannerID = [p_bannerID copy];
	smartBanner = p_smartBanner;
	testMode = p_testMode;
    testDevices = [p_testDevices copy];
	bannerPos = [p_initialPosition copy];
    
    NSLog(@"bannerPos: %@", bannerPos);
    
	if (smartBanner)
	{
		bannerView_ = [[GADBannerView alloc] initWithAdSize:kGADAdSizeSmartBannerLandscape];
	} else {
		bannerView_ = [[GADBannerView alloc] initWithAdSize:kGADAdSizeBanner];
	}

	bannerView_.adUnitID = bannerID;

	bannerView_.rootViewController = [AppDelegate getViewController];
	rootView_ = [AppDelegate getViewController].view;
	[rootView_ addSubview:bannerView_];

	topConstraint = [[NSLayoutConstraint constraintWithItem:bannerView_
												  attribute:NSLayoutAttributeTop
												  relatedBy:NSLayoutRelationEqual
													 toItem:rootView_
												  attribute:NSLayoutAttributeTop
												 multiplier:1.0
												   constant:0] retain];

	bottomConstraint = [[NSLayoutConstraint constraintWithItem:bannerView_
												  attribute:NSLayoutAttributeBottom
												  relatedBy:NSLayoutRelationEqual
													 toItem:rootView_
												  attribute:NSLayoutAttributeBottom
												 multiplier:1.0
												   constant:0] retain];

	leftConstraint = [[NSLayoutConstraint constraintWithItem:bannerView_
																	attribute:NSLayoutAttributeLeft
																	relatedBy:NSLayoutRelationEqual
																		toItem:rootView_
																	  attribute:NSLayoutAttributeLeft
																	 multiplier:1.0
																	   constant:0] retain];
	centerConstraint = [[NSLayoutConstraint constraintWithItem:bannerView_
																	  attribute:NSLayoutAttributeCenterX
																	  relatedBy:NSLayoutRelationEqual
																		 toItem:rootView_
																	  attribute:NSLayoutAttributeCenterX
																	 multiplier:1.0
																	   constant:0] retain];
	rightConstraint = [[NSLayoutConstraint constraintWithItem:bannerView_
																	  attribute:NSLayoutAttributeRight
																	  relatedBy:NSLayoutRelationEqual
																		 toItem:rootView_
																	  attribute:NSLayoutAttributeRight
																	 multiplier:1.0
																	   constant:0] retain];
	
    if ([bannerPos isEqualToString:@"topleft"])
	{
		verticalConstraint = topConstraint;
		horizontalConstraint = leftConstraint;

	} else if ([bannerPos isEqualToString:@"topcenter"])
	{
		verticalConstraint = topConstraint;
		horizontalConstraint = centerConstraint;

	} else if ([bannerPos isEqualToString:@"topright"])
	{
		verticalConstraint = topConstraint;
		horizontalConstraint = rightConstraint;

	} else if ([bannerPos isEqualToString:@"bottomleft"])
	{
		verticalConstraint = bottomConstraint;
		horizontalConstraint = leftConstraint;

	} else if ([bannerPos isEqualToString:@"bottomcenter"])
	{
		verticalConstraint = bottomConstraint;
		horizontalConstraint = centerConstraint;

	} else if ([bannerPos isEqualToString:@"bottomright"])
	{
		verticalConstraint = bottomConstraint;
		horizontalConstraint = rightConstraint;
	} else {
        verticalConstraint = bottomConstraint;
		horizontalConstraint = centerConstraint;
    }

	bannerView_.translatesAutoresizingMaskIntoConstraints = NO;
	[rootView_ addConstraint:verticalConstraint];
	[rootView_ addConstraint:horizontalConstraint];

	hasReceiveAd = NO;
	hasDismissScreen = NO;
	hasFailedReceiveAd = NO;
	hasLeaveApp = NO;
	hasPresetScreen = NO;
    
    bannerView_.hidden = YES;

	NSLog(@"AdMobBanner: initialize()");
}

- (void)loadBanner
{
	GADRequest *request = [GADRequest request];
	if (testMode)
	{
		request.testDevices = testDevices;
	}

	[bannerView_ loadRequest:request];
	NSLog(@"AdMobBanner: loadBanner()");
}

- (void)showBanner
{
	bannerView_.hidden = NO;
	NSLog(@"AdMobBanner: showBanner()");
}

- (void)setBannerPos:(NSLayoutConstraint*)p_verticalConstraint horizontalConstraint:(NSLayoutConstraint*)p_horizontalConstraint
{
    NSLog(@"old verticalConstraint: %@:", verticalConstraint);
    NSLog(@"old horizontalConstraint: %@:", horizontalConstraint);
    
    NSLog(@" => verticalConstraint: %@:", p_verticalConstraint);
    NSLog(@" => horizontalConstraint: %@:", p_horizontalConstraint);

    [rootView_ removeConstraint:verticalConstraint];
	[rootView_ removeConstraint:horizontalConstraint];
    
	verticalConstraint = p_verticalConstraint;
	horizontalConstraint = p_horizontalConstraint;
    
	[rootView_ addConstraint:verticalConstraint];
	[rootView_ addConstraint:horizontalConstraint];
}

- (void)setBannerTopLeft
{
    [self setBannerPos:topConstraint horizontalConstraint:leftConstraint];
	//bannerView_.hidden = NO;
    NSLog(@"AdMobBanner: setBannerTopLeft()");
}

- (void)setBannerTopCenter
{
	[self setBannerPos:topConstraint horizontalConstraint:centerConstraint];
	//bannerView_.hidden = NO;
	NSLog(@"AdMobBanner: setBannerTopCenter()");
}

- (void)setBannerTopRight
{
	[self setBannerPos:topConstraint horizontalConstraint:rightConstraint];
	//bannerView_.hidden = NO;
	NSLog(@"AdMobBanner: setBannerTopRight()");
}

- (void)setBannerBottomLeft
{
	[self setBannerPos:bottomConstraint horizontalConstraint:leftConstraint];
	//bannerView_.hidden = NO;
	NSLog(@"AdMobBanner: setBannerBottomLeft()");
}

- (void)setBannerBottomCenter
{
    [self setBannerPos:bottomConstraint horizontalConstraint:centerConstraint];
	//bannerView_.hidden = NO;
	NSLog(@"AdMobBanner: setBannerBottomCenter()");
}

- (void)setBannerBottomRight
{
	[self setBannerPos:bottomConstraint horizontalConstraint:rightConstraint];
	//bannerView_.hidden = NO;
	NSLog(@"AdMobBanner: setBannerBottomRight()");
}

- (void)hideBanner
{
  bannerView_.hidden = YES;
  NSLog(@"AdMobBanner: hideBanner()");
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


-(void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInt
							   duration:(NSTimeInterval)duration {
  if (smartBanner && UIInterfaceOrientationIsLandscape(toInt)) {
	bannerView_.adSize = kGADAdSizeSmartBannerLandscape;
  } else {
	bannerView_.adSize = kGADAdSizeSmartBannerPortrait;
  }
}

#pragma mark GADBannerViewDelegate implementation

- (void)adViewDidReceiveAd:(GADBannerView *)view
{
	hasReceiveAd = YES;
	NSLog(@"AdMobl: didReceiveAd()");
}

- (void)adView:(GADBannerView *)view didFailToReceiveAdWithError:(GADRequestError *)error
{
	hasFailedReceiveAd = YES;
	NSLog(@"AdMob: didFailToReceiveAdWithError() %@",error);
}

- (void)adViewWillPresentScreen:(GADBannerView *)adView
{
	hasPresetScreen = YES;
	NSLog(@"AdMob: adViewWillPresentScreen()");
}

- (void)adViewDidDismissScreen:(GADBannerView *)adView
{
	hasDismissScreen = YES;
	NSLog(@"AdMob: adViewDidDismissScreen()");
}

- (void)adViewWillLeaveApplication:(GADBannerView *)adView
{
	hasLeaveApp = YES;
	NSLog(@"AdMob: adViewWillLeaveApplication()");
}

@end
