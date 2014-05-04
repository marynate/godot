#include "core/object.h"

#ifdef __OBJC__
@class AdMobBanner;
typedef AdMobBanner * BannerPtr;
@class AdMobInterstitial;
typedef AdMobInterstitial * InterstitialPtr;
#else
typedef void * BannerPtr;
typedef void * InterstitialPtr;
#endif

class AdMob_iOS : public Object {
  
	OBJ_TYPE(AdMob_iOS, Object);
	
	BannerPtr banner;
	InterstitialPtr interstitial;
	
	String AdMobId;
	
	static AdMob_iOS* instance;
	static void _bind_methods();
	
	bool initialized;
	void Initialize();

public:
	static AdMob_iOS* get_singleton();

	void ShowBanner();
	void HideBanner();
	void SetBannerBottomLeft();
	void SetBannerTopLeft();
	void ShowInterstitial();
	bool HasReceiveAd();
	bool HasDismissScreen();
	bool HasFailedToReceive();
	bool HasLeaveApplication();
	bool HasPresentScreen();
			
	
	AdMob_iOS();
	~AdMob_iOS();
};
