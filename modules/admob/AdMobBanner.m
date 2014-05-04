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
  [super dealloc];
}

- (void)initialize:(NSString*)admobId
{
  admobID = admobId;
  bannerView_ = [[GADBannerView alloc] initWithAdSize:kGADAdSizeBanner];

  bannerView_.adUnitID = admobID;

  bannerView_.rootViewController = [AppDelegate getViewController];
  [[AppDelegate getViewController].view addSubview:bannerView_];  
  NSLog(@"AdMobBanner: initialize()");
}

- (void)showBanner
{
  [bannerView_ loadRequest:[GADRequest request]];
  bannerView_.hidden = NO;
  NSLog(@"AdMobBanner: showBanner()");
}

- (void)hideBanner
{
  bannerView_.hidden = YES;
  NSLog(@"AdMobBanner: hideBanner()");
}
@end
