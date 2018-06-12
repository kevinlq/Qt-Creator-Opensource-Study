package io.qt.dummy;

import android.app.Activity;
import android.os.Build;
import io.qbs.lib3.lib3;
import java.util.Arrays;
import java.util.List;

public class Dummy extends Activity
{
    static {
        List<String> abis = Arrays.asList(Build.SUPPORTED_ABIS);
        if (abis.contains("armeabi")) {
            System.loadLibrary("lib1");
            System.loadLibrary("lib2");
        }
        lib3.foo();
    }
}
