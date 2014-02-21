import com.ideaworks3d.marmalade.LoaderAPI;
import com.ideaworks3d.marmalade.LoaderActivity;


import java.io.File;
import java.io.IOException;

import android.content.res.Configuration;
import android.os.StatFs;
import android.os.Environment;
import android.util.DisplayMetrics;
import android.util.Log;

class lsUtilities implements Cloneable
{
	public static final String TAG = "lsUtilities";

	private int m_TabletThreshold = 461;
	private int m_CurrentThreshold =  0;
	
	private File m_CurrentDirectory = Environment.getExternalStorageDirectory();
	private StatFs m_StatFs = new StatFs(m_CurrentDirectory.getAbsolutePath());
	private DisplayMetrics m_DeviceMetrics = new DisplayMetrics();

	//-----------------------------------------------------------------------
	// lsUtilIOSPreventBackup
	//-----------------------------------------------------------------------
    public void lsUtilIOSPreventBackup(String file)
    {
        
    }

	//-----------------------------------------------------------------------
	// lsUtilInstallExceptionHandler
	//-----------------------------------------------------------------------

    public void lsUtilInstallExceptionHandler()
    {
        
    }

	//-----------------------------------------------------------------------
	// lsUtilUninstallExceptionHandler
	//-----------------------------------------------------------------------
    public void lsUtilUninstallExceptionHandler()
    {
        
    }

	//-----------------------------------------------------------------------
	// UpdateDeviceClass
	//-----------------------------------------------------------------------
	private void UpdateDeviceClass()
	{
	   LoaderActivity.m_Activity.getWindowManager().getDefaultDisplay().getMetrics(m_DeviceMetrics);
	   float xWidth  = m_DeviceMetrics.widthPixels / m_DeviceMetrics.xdpi; 
	   float xHeight = m_DeviceMetrics.heightPixels / m_DeviceMetrics.ydpi;
	   m_CurrentThreshold = (int)((xWidth * xWidth) * (xHeight * xHeight));
	}

	//-----------------------------------------------------------------------
	// lsUtilGetDeviceClassInt
	//-----------------------------------------------------------------------
    public int lsUtilGetDeviceClassInt()
    {
		UpdateDeviceClass(); 

	   if(m_CurrentThreshold >=  m_TabletThreshold)
	   { 
		 return 3;  // tablet
	   }
	   else if(m_CurrentThreshold > 0)
	   {
			return 2; // smartphone
	   }

		// How did we endup here?
		return 1;	// generic
    }

	//-----------------------------------------------------------------------
	// lsUtilGetDeviceClassString
	//-----------------------------------------------------------------------
    public String lsUtilGetDeviceClassString()
    {
       UpdateDeviceClass();

	   if(m_CurrentThreshold >=  m_TabletThreshold)
	   { 
		 return "tablet";
	   }
	   else if(m_CurrentThreshold > 0)
	   {
			return "smartphone";
	   }

	   return "generic";
    }
	//-----------------------------------------------------------------------
	// lsDeviceSetTabletThreshold
	//-----------------------------------------------------------------------
    public void lsDeviceSetTabletThreshold(int threshold)
    {
        m_CurrentThreshold = threshold;
    }
	//-----------------------------------------------------------------------
	// lsUtilGetDeviceStorageInformation
	//-----------------------------------------------------------------------
    public void lsUtilGetDeviceStorageInformation()
    {
        
    }

	
	//-----------------------------------------------------------------------
    public Object clone() throws CloneNotSupportedException {
		throw new CloneNotSupportedException();
	}

}
