package koneu.rootapp;

import android.app.Activity;
import android.os.Bundle;
import koneu.rootapp.R;

public class MainActivity extends Activity {

	static {
		System.loadLibrary("sudo");
	}

	public native boolean sudo(String cmd);

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		if (sudo("true")) {
			setContentView(R.layout.activity_main);
		} else {
			setContentView(R.layout.no_root);
		}
	}

}
