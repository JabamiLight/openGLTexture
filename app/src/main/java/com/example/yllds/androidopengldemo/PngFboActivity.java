package com.example.yllds.androidopengldemo;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;
import android.widget.RelativeLayout;

public class PngFboActivity extends Activity {

	private SurfaceView surfaceView;
	private RelativeLayout preview_parent_layout;

	private String picPath = "/mnt/sdcard/ic_launcher.png";
	
	private PngPreviewController pngPreviewController;
	private Callback previewCallback = new Callback() {

		public void surfaceCreated(SurfaceHolder holder) {
			pngPreviewController = new PngPreviewController();
			pngPreviewController.init(picPath,getAssets(),1);
			pngPreviewController.setSurface(holder.getSurface());
		}

		public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
//			pngPreviewController.resetSize(width, height);
		}
		

		public void surfaceDestroyed(SurfaceHolder holder) {
		}
	};

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.texture_pic_preview_layout);
		findView();
	}

	private void findView() {
		preview_parent_layout = (RelativeLayout) findViewById(R.id.preview_parent_layout);
		surfaceView = new SurfaceView(PngFboActivity.this);
		SurfaceHolder mSurfaceHolder = surfaceView.getHolder();
		mSurfaceHolder.addCallback(previewCallback);
		mSurfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		preview_parent_layout.addView(surfaceView, 0);
		surfaceView.getLayoutParams().width = getWindowManager().getDefaultDisplay().getWidth();
		surfaceView.getLayoutParams().height = getWindowManager().getDefaultDisplay().getWidth();
	}

	/**
	 * On stop.
	 */
	protected void onStop() {
		super.onStop();
		// Free the native renderer
		stopPlay();
	}

	protected void stopPlay() {
		Log.i("problem", "playerController.stop()...");
		if (null != pngPreviewController) {
			pngPreviewController.stop();
			pngPreviewController = null;
		}
	}

	private Handler handler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case 0:
				break;
			default:
				break;
			}
		}

	};

}