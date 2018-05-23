package com.example.yllds.androidopengldemo;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.ByteBuffer;

public class MainActivity extends Activity {



	private Button go_btn;
	private ImageView tv;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		go_btn = (Button) findViewById(R.id.go_btn);
		tv=findViewById(R.id.title);
		go_btn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(MainActivity.this, PngPreviewActivity.class);
				startActivity(intent);
			}
			
		});
		try {
			FileInputStream is=new FileInputStream("/mnt/sdcard/ic_launchergrayrgb.rgb");
			byte[] bytes=new byte[720*720*4];
			is.read(bytes);
			is.close();
			ByteBuffer bytebuffer = ByteBuffer.wrap(bytes);
			Bitmap b= Bitmap.createBitmap(720,720, Bitmap.Config.ARGB_8888);
			b.copyPixelsFromBuffer(bytebuffer);
			tv.setImageBitmap(b);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	
	public void goFbo(View view) {
		Intent intent = new Intent(MainActivity.this, PngFboActivity.class);
		startActivity(intent);
	}
}
