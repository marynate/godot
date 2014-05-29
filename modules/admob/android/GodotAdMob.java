/*
 *
 * <meta-data android:name="com.google.android.gms.version"
 *       android:value="@integer/google_play_services_version"/>
 *
 * Activity: * 	
 * <activity android:name="com.google.android.gms.ads.AdActivity"
 *       android:configChanges="keyboard|keyboardHidden|orientation|screenLayout|uiMode|screenSize|smallestScreenSize"/>
 *  
 *  Target API level at least 13
 */

package com.android.godot;

import android.app.Activity;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.RelativeLayout;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdRequest.Builder;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;

public class GodotAdMob extends Godot.SingletonBase {

	InterstitialAd interstitial;
	AdView adView;
	
	private Activity activity;
    private AdRequest.Builder adRequestBuilder;
	
	boolean initialized;
	
	boolean adReceived;
	boolean screenDismissed;
	boolean failedToReceiveAd;
	boolean applicationLeaved;
	boolean presentScreen;
	
	int layoutRule1;
	int layoutRule2;

	public void InitializeUIThread(String p_banner_id, String p_interstitial_id, boolean p_test_mode, String p_test_devices, boolean p_smart_banner) {

		adRequestBuilder = new AdRequest.Builder();

		if (p_test_mode) {
            adRequestBuilder.addTestDevice(AdRequest.DEVICE_ID_EMULATOR);
            
            if (p_test_devices != null && p_test_devices.trim() != "") {
                for (String device : p_test_devices.split(",")) {
                    adRequestBuilder.addTestDevice(device);
					Log.d("godot", "AdMob: addTestDevice: " + device);
                }
            }
        }
		
		// Create the interstitial
		interstitial = new InterstitialAd(activity);
		interstitial.setAdUnitId(p_interstitial_id);
		interstitial.setAdListener(new AdListener() {
			@Override
			public void onAdLoaded() {
				Log.d("godot", "AdMob: onAdLoaded");
				interstitial.show();
				adReceived = true;
			}

			@Override
			public void onAdClosed() {
				Log.d("godot", "AdMob: onAdClosed");
				screenDismissed = true;
			}

			@Override
			public void onAdFailedToLoad(int errorCode) {
				Log.w("godot", "AdMob: onAdFailedToLoad-> " + errorCode);
				failedToReceiveAd = true;
			}

			@Override
			public void onAdLeftApplication() {
				Log.d("godot", "AdMob: onAdLeftApplication");
				applicationLeaved = true;
			}

			@Override
			public void onAdOpened() {
                Log.d("godot", "AdMob: onAdOpened");
                presentScreen = true;
            }
        });

		
		// Create banner
		adView = new AdView(activity);
		if (p_smart_banner) {
			adView.setAdSize(AdSize.SMART_BANNER);
		} else {
			adView.setAdSize(AdSize.BANNER);
		}

		adView.setAdUnitId(p_banner_id);

		RelativeLayout layout = ((Godot)activity).adLayout;
		RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT,
	        LayoutParams.WRAP_CONTENT);
	
		layoutParams.addRule(layoutRule1);
		layoutParams.addRule(layoutRule2);
		layout.addView(adView, layoutParams);
		layout.invalidate();
	
        adView.setAdListener(new AdListener() {
            @Override
            public void onAdLoaded() {
                Log.d("godot", "AdMob: onAdLoaded");
                adReceived = true;
            }
            
            @Override
            public void onAdClosed() {
                Log.d("godot", "AdMob: onAdClosed");
                screenDismissed = true;
            }
            
            @Override
            public void onAdFailedToLoad(int errorCode) {
                Log.w("godot", "AdMob: onAdFailedToLoad-> " + errorCode);
                failedToReceiveAd = true;
            }
            
            @Override
            public void onAdLeftApplication() {
                Log.d("godot", "AdMob: onAdLeftApplication");
                applicationLeaved = true;
            }
            
            @Override
            public void onAdOpened() {
                Log.d("godot", "AdMob: onAdOpened");
                presentScreen = true;
            }
        });
        
		adView.setVisibility(View.VISIBLE);
		
		initialized = true;
		
		adReceived = false;
		screenDismissed = false;
		failedToReceiveAd = false;
		applicationLeaved = false;
		presentScreen = false;

		Log.d("godot", "AdMob: Initialized");
	}	
	
	public void ShowInterstitialUIThread()
	{
		if (initialized)
		{
			// Create ad request
			AdRequest adRequest = adRequestBuilder.build();

			// Begin loading your interstitial
			interstitial.loadAd(adRequest);

			// Set Ad Listener to use the callbacks below
			//interstitial.setAdListener((AdListener) this);
			
            adReceived = false;
		    screenDismissed = false;
		    failedToReceiveAd = false;
		    applicationLeaved = false;
		    presentScreen = false;
			
			Log.d("godot", "AdMob: Show Interstitial");
		}
		else
		{
			Log.e("godot", "AdMob: Calling \"ShowInterstitial()\" but AdMob not initilized");
		}
	} 	

	public void ShowBannerUIThread()
	{
		if (initialized)
		{
			AdRequest request = adRequestBuilder.build();
			//adView.setVisibility(View.VISIBLE);
			adView.loadAd(request);

			Log.d("godot", "AdMob: Show Banner");
		}
		else
		{
			Log.e("godot", "AdMob: Calling \"ShowBanner()\" but AdMob not initilized");
		}
	}
	
	public void HideBannerUIThread()
	{
		if (initialized)
		{
		    adView.setVisibility(View.GONE);
    		    Log.d("godot", "AdMob: Hide Banner");
		}	
	}

	void ShowBannerAt(final int p_rule1, final int p_rule2)
	{
		if (!initialized) return;
		if (adView == null) return;

		activity.runOnUiThread(new Runnable() {
			public void run() {

				if (layoutRule1 != p_rule1 || layoutRule2 != p_rule2 || adView.getVisibility() != View.VISIBLE)
				{
					layoutRule1 = p_rule1;
					layoutRule2 = p_rule2;
					SetBannerView();
					adView.setVisibility(View.VISIBLE);
				}
			}
		});
	}
	
	public void ShowBannerBottomLeft()
	{
		ShowBannerAt(RelativeLayout.ALIGN_PARENT_BOTTOM, RelativeLayout.ALIGN_PARENT_LEFT);
		Log.d("godot", "AdMob: Moving Banner to bottom left");
	}
	
	public void ShowBannerBottomRight()
	{
		ShowBannerAt(RelativeLayout.ALIGN_PARENT_BOTTOM, RelativeLayout.ALIGN_PARENT_RIGHT);
		Log.d("godot", "AdMob: Moving Banner to bottom right");
	}

	public void ShowBannerBottomCenter()
	{
		ShowBannerAt(RelativeLayout.ALIGN_PARENT_BOTTOM, RelativeLayout.CENTER_HORIZONTAL);
		Log.d("godot", "AdMob: Moving Banner to bottom center");
	}

	public void ShowBannerTopLeft()
	{
		ShowBannerAt(RelativeLayout.ALIGN_PARENT_TOP, RelativeLayout.ALIGN_PARENT_LEFT);
		Log.d("godot", "AdMob: Moving Banner to top left");
	}

	public void ShowBannerTopRight()
	{
		ShowBannerAt(RelativeLayout.ALIGN_PARENT_TOP, RelativeLayout.ALIGN_PARENT_RIGHT);
		Log.d("godot", "AdMob: Moving Banner to top right");
	}
	
	public void ShowBannerTopCenter()
	{
		ShowBannerAt(RelativeLayout.ALIGN_PARENT_TOP, RelativeLayout.CENTER_HORIZONTAL);
		Log.d("godot", "AdMob: Moving Banner to top center");
	}

	void SetBannerView()
	{
		RelativeLayout layout = ((Godot)activity).adLayout;
		RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT,
                LayoutParams.WRAP_CONTENT);
		
		layoutParams.addRule(layoutRule1);
		layoutParams.addRule(layoutRule2);
		
		layout.removeView(adView);
		layout.addView(adView, layoutParams);		
	}
	
	public void Initialize(final String p_banner_id, final String p_interstitial_id, final boolean p_test_mode, final String p_test_devices, final boolean p_smart_banner) {

		activity.runOnUiThread(new Runnable() {
			public void run() {
				InitializeUIThread(p_banner_id, p_interstitial_id, p_test_mode, p_test_devices, p_smart_banner);
			}
		});
	}
	
	public void ShowInterstitial() {
		activity.runOnUiThread(new Runnable() {
			public void run() {
				ShowInterstitialUIThread();
			}
		});
	}	
	
	public void LoadBanner() {
		activity.runOnUiThread(new Runnable() {
			public void run() {
				ShowBannerUIThread();
			}
		});
	}
	
	public void HideBanner() {
		activity.runOnUiThread(new Runnable() {
			public void run() {
				HideBannerUIThread();
			}
		});
	}	
	
	public boolean HasReceiveAd() {
		boolean ret = adReceived;
		adReceived = false;
		return ret;
	}

	public boolean HasDismissScreen() {
		boolean ret = screenDismissed;
		screenDismissed = false;
		return ret;		
	}

	public boolean HasFailedToReceive() {
		boolean ret = failedToReceiveAd;
		failedToReceiveAd = false;
		return ret;			
	}

	public boolean HasLeaveApplication() {
		boolean ret = applicationLeaved;
		applicationLeaved = false;
		return ret;
	}

	public boolean HasPresentScreen() {
		boolean ret = presentScreen;
		presentScreen = false;
		return ret;
	} 	

	@Override
	protected void onMainPause() {
		if (adView != null) {
			adView.pause();
			Log.d("godot", "AdMob: adView paused");
		}
		super.onMainPause();
	}

	@Override
	protected void onMainResume() {
		super.onMainResume();
		if (adView != null) {
			adView.resume();
			Log.d("godot", "AdMob: adView resumed");
		}
	}

	@Override
	protected void onMainDestroy() {
		if (adView != null) {
			adView.destroy();
			Log.d("godot", "AdMob: adView destroyed");
		}
		super.onMainDestroy();

	}

	static public Godot.SingletonBase initialize(Activity p_activity) {

		return new GodotAdMob(p_activity);
	}

	public GodotAdMob(Activity p_activity) {

		registerClass("AdMob", new String[] {"LoadBanner", "HideBanner",
				"ShowBannerBottomLeft", "ShowBannerBottomRight", "ShowBannerBottomCenter", "ShowBannerTopLeft", "ShowBannerTopRight", "ShowBannerTopCenter", "ShowInterstitial", "HasReceiveAd",
				"HasDismissScreen", "HasFailedToReceive","HasLeaveApplication","HasPresentScreen"});
		
		activity=p_activity;
		initialized = false;
		
		layoutRule1 = RelativeLayout.ALIGN_PARENT_BOTTOM;
		layoutRule2 = RelativeLayout.ALIGN_PARENT_LEFT;	

		activity.runOnUiThread(new Runnable() {
			public void run() {
				String banner_id = GodotLib.getGlobal("admob/banner_id");
				String interstitial_id = GodotLib.getGlobal("admob/interstitial_id");
				boolean test_mode = GodotLib.getGlobal("admob/test_mode").toLowerCase().equals("true");
				boolean smart_banner = GodotLib.getGlobal("admob/smart_banner").toLowerCase().equals("true");
                String test_devices = GodotLib.getGlobal("admob/test_devices");
				if (test_mode) {
					Log.d("godot", "test mode on: " + test_devices);
				} else {
					Log.d("godot", "test mode off!");
				}
				InitializeUIThread(banner_id, interstitial_id, test_mode, test_devices, smart_banner);
			}
		});	
	}	
}
