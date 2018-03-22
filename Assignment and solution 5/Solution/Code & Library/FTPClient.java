package matala04;

import java.util.Scanner;

import javax.swing.JOptionPane;
import javax.swing.JPasswordField;
import javax.swing.JTextField;
import com.jcraft.jsch.ChannelSftp;
import com.jcraft.jsch.JSch;
import com.jcraft.jsch.JSchException;
import com.jcraft.jsch.Session;
import com.jcraft.jsch.SftpException;
import com.jcraft.jsch.UserInfo;

/**
 * Connect to the user using known IP, SSH port 22, and hacks the password to steal a file from the user.
 * Using FTP - file transport protocol.
 * @author Dvir & Shir & Kineret
 *
 */
public class FTPClient {

	private static final String USER_PROMPT = "Enter username@hostname:port";
	private static final boolean USE_GUI = true;

	/**
	 * 
	 * @author Dvir & Shir & Kineret
	 *
	 */
	public static class MyUserInfo implements UserInfo {
		private String password;

		@Override
		public String getPassword() {
			return password;
		}

		@Override
		public boolean promptYesNo(final String str) {
			final Object[] options = { "yes", "no" };
			final boolean yesNo = (USE_GUI)
					? JOptionPane.showOptionDialog(null, str, "Warning", JOptionPane.DEFAULT_OPTION,
							JOptionPane.WARNING_MESSAGE, null, options, options[0]) == 0
					: System.console().readLine("Enter y or n: ").equals("y");
			return yesNo;
		}

		@Override
		public String getPassphrase() {
			return null;
		}

		@Override
		public boolean promptPassphrase(final String message) {
			return true;
		}
		/**
		 * Asks for a password.
		 */
		@Override
		public boolean promptPassword(final String message) {
			if (!USE_GUI) {
				password = new String(System.console().readPassword("Password: "));
				return true;
			} else {
				final JTextField passwordField = new JPasswordField(20);
				final Object[] ob = { passwordField };
				final int result = JOptionPane.showConfirmDialog(null, ob, message, JOptionPane.OK_CANCEL_OPTION);
				if (result == JOptionPane.OK_OPTION) {
					password = passwordField.getText();
					return true;
				} else {
					return false;
				}
			}
		}

		/**
		 * This function tries to connect to the user using a SSH 22 port, a list of
		 * passwords a known IP and if connection succeeded - after trying to hack its password, it will steal a file
		 * from the user's computer.
		 * 
		 * @param args
		 * @throws SftpException
		 * @throws JSchException
		 */
		public static void main(String args[])/*steal()*/ throws SftpException, JSchException {
			Session session = null;
			ChannelSftp channelSftp = null;
			Scanner sc = new Scanner(System.in);
			try {
				final JSch jsch = new JSch();
				final String defaultInput = System.getProperty("user.name") + "@10.9.0.116:22";
				String input = (USE_GUI) ? JOptionPane.showInputDialog(USER_PROMPT, defaultInput)
						: sc.next();//("%s (%s): ", USER_PROMPT, defaultInput);
				sc.close();
				if (input == null || input.trim().length() == 0) {
					input = defaultInput;
				}
				final int indexOfAt = input.indexOf('@');
				final int indexOfColon = input.indexOf(':');
				final String user = input.substring(0, indexOfAt);
				final String host = input.substring(indexOfAt + 1, indexOfColon);
				// final int port =Integer.parseInt(input.substring(indexOfColon + 1));
				jsch.setKnownHosts("/path/to/known_hosts");
				session = jsch.getSession(user, host, 22);
				final UserInfo ui = new MyUserInfo();
				session.setUserInfo(ui);
				session.connect();
				System.out.println("Starting to steal data....");
				channelSftp = (ChannelSftp) session.openChannel("sftp");
				channelSftp.connect();
				channelSftp.get("MyBankData.txt", "StolenFile.txt");
				System.out.println("Process of stealth finished");
			} finally {
				// Couldn't connect
				if (channelSftp != null) {
					channelSftp.exit();
				}
				if (session != null) {
					session.disconnect();
				}
			}
		}

		@Override
		public void showMessage(String arg0) {
		}
	}
}
