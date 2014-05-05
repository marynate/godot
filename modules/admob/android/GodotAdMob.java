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
    
    public void InitializeUIThread(String p_key, boolean p_test_mode, String p_test_devices) {
        
        adRequestBuilder = new AdRequest.Builder();
        
        if (p_test_mode) {
            adRequestBuilder.addTestDevice(AdRequest.DEVICE_ID_EMULATOR);
            
            if (p_test_devices != null && p_test_devices.trim() != "") {
                for (String device : p_test_devices.split(",")) {
                    adRequestBuilder.addTestDevice(device);
                }
            }
        }
		
		// Create the interstitial
		interstitial = new InterstitialAd(activity);
        interstitial.setAdUnitId(p_key);
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
		adView.setAdUnitId(p_key);
		adView.setAdSize(AdSize.BANNER);
        
		RelativeLayout layout = ((Godot)activity).layout;
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
			AdRequest adRequest = adRequestBuilder().build();

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
		    adView.loadAd(request);
		    adView.setVisibility(View.VISIBLE);
	        
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
	
	public void SetBannerBottomLeft()
	{
		if (!initialized) return;

		if (layoutRule1 != RelativeLayout.ALIGN_PARENT_BOTTOM || 
			layoutRule2 != RelativeLayout.ALIGN_PARENT_LEFT)
		{
			activity.runOnUiThread(new Runnable() {
				public void run() {
					layoutRule1 = RelativeLayout.ALIGN_PARENT_BOTTOM; 
					layoutRule2 = RelativeLayout.ALIGN_PARENT_LEFT;
					
					Log.d("godot", "AdMob: Moving Banner to bottom left");
					
					if (adView != null)
					{
						SetBannerView();
					}			
				}
			});
		}
	}
	
	public void SetBannerTopLeft()
	{
		if (!initialized) return;

		if (layoutRule1 != RelativeLayout.ALIGN_PARENT_TOP || 
			layoutRule2 != RelativeLayout.ALIGN_PARENT_LEFT)
		{
			activity.runOnUiThread(new Runnable() {
				public void run() {
					layoutRule1 = RelativeLayout.ALIGN_PARENT_TOP; 
					layoutRule2 = RelativeLayout.ALIGN_PARENT_LEFT;
					
					Log.d("godot", "AdMob: Moving Banner to top left");
					
					if (adView != null)
					{
						SetBannerView();
					}
				}
			});
		}
	}
	
	void SetBannerView()
	{
		RelativeLayout layout = ((Godot)activity).layout;
		RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT,
                LayoutParams.WRAP_CONTENT);
		
		layoutParams.addRule(layoutRule1);
		layoutParams.addRule(layoutRule2);
		
		layout.removeView(adView);
		layout.addView(adView, layoutParams);		
	}
	
	public void Initialize(final String p_key, boolean p_test_mode, String p_test_devices) {

		activity.runOnUiThread(new Runnable() {
			public void run() {
				InitializeUIThread(p_key, p_test_mode, p_test_devices);
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
	
	public void ShowBanner() {
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

	static public Godot.SingletonBase initialize(Activity p_activity) {

		return new GodotAdMob(p_activity);
	}

	public GodotAdMob(Activity p_activity) {

		registerClass("AdMob", new String[] {"ShowBanner", "HideBanner",
				"SetBannerBottomLeft", "SetBannerTopLeft", "ShowInterstitial", "HasReceiveAd",
				"HasDismissScreen", "HasFailedToReceive","HasLeaveApplication","HasPresentScreen"});
		
		activity=p_activity;
		initialized = false;
		
		layoutRule1 = RelativeLayout.ALIGN_PARENT_BOTTOM;
		layoutRule2 = RelativeLayout.ALIGN_PARENT_LEFT;	

		activity.runOnUiThread(new Runnable() {
			public void run() {
				String key = GodotLib.getGlobal("admob/api_key");
                boolean test_mode = GodotLib.getGlobal("admob/test_mode");
                String test_devices = GodotLib.getGlobal("admob/test_devices");
				InitializeUIThread(key, test_mode, test_devices);
			}
		});	
	}	
}
