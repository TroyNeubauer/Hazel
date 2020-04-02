package com.hazel.main;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends Activity
{
	public static native int hello_cpp(); // declare c or c++ function (synonymous with c's extern)

	static 
	{
		System.loadLibrary("Hazel"); // load c or c++ lib
	}

	@Override
	protected void onCreate(Bundle arg0) 
	{
		super.onCreate(arg0);

		Log.d("Hello world!", "I'm Java");

		hello_cpp();

	}
}
