package koneu.rootapp;

import android.app.Activity;
import android.os.Bundle;
import koneu.rootapp.R;

public class MainActivity extends Activity {
	
	static {
		System.loadLibrary("sudo");
	}
	
	final public native boolean sudo(String cmd); /* This will execute "cmd" in the shell started with startshell() or via "su -c "cmd"". */
	final public native void startshell(); /* This will start a new shell in the background, it WILL NOT DETECT WHETHER PRIVILEGES HAVE BEEN GRANTED. If a shell is running this is a no-op. */
	final public native void closeshell(); /* This will close the shell started with startshell(). */
	
	@Override
	final protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		startshell(); /* THIS IS WRONG. DO NOT DO IT LIKE THIS, do it like shown below. */
		
		if (sudo("true")) { /* Check if we have SU privileges. */
			startshell(); /* If permission has been granted, start a shell in the background. */
			/* From now on, all calls to sudo() will be passed to the shell instead of run individually. */
			
			setContentView(R.layout.activity_main);
			
			startshell(); /* Does nothing, because we already have a shell running. */
			
		} else {
			setContentView(R.layout.no_root);
		}
	}
	
	@Override
	final protected void onDestroy() {
		super.onDestroy();
		closeshell(); /* Close the shell when closing the app. */
	}
	
}
