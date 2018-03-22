package matala04;

import com.jcraft.jsch.JSchException;
import com.jcraft.jsch.SftpException;

public class Main {

	public static void main(String[] args) throws InterruptedException{
		PortScanner.portScan();
		BruteHackingSSH.ssh();
	
		
		/*try {
			FTPClient.MyUserInfo.steal();
		} catch (SftpException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (JSchException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} */
	}
	
}
