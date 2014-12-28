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
	
	String banner_id;
	String interstitial_id;
	String test_mode;
	String smart_banner;
	String test_devices;
	String banner_pos;
	
	static AdMob_iOS* instance;
	static void _bind_methods();
	
	bool initialized;
	void Initialize();

public:
	static AdMob_iOS* get_singleton();

	void LoadBanner();
	void ShowBanner();
	void HideBanner();
	void SetBannerTopLeft();
	void SetBannerTopRight();
	void SetBannerTopCenter();
	void SetBannerBottomLeft();
	void SetBannerBottomRight();
	void SetBannerBottomCenter();

	bool HasReceiveAd();
	bool HasDismissScreen();
	bool HasFailedToReceive();
	bool HasLeaveApplication();
	bool HasPresentScreen();

	void LoadInterstitial();
	void ShowInterstitial();

	bool HasInterstitialReceiveAd();
	bool HasInterstitialDismissScreen();
	bool HasInterstitialFailedToReceive();
	bool HasInterstitialLeaveApplication();
	bool HasInterstitialPresentScreen();
			
	
	AdMob_iOS();
	~AdMob_iOS();
};
