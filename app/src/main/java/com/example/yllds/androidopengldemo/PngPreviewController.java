package com.example.yllds.androidopengldemo;

import android.content.res.AssetManager;
import android.view.Surface;

public class PngPreviewController {

	public native void init(String picPath, AssetManager assetManager);

	public native void setSurface(Surface surface);

	public native void resetSize(int width, int height);

	public native void stop();

}
