package matala04;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Scanner;

public class PortScanner {
	/**
	 * @throws InterruptedExceptionwhen a thread is waiting, sleeping, or otherwise occupied,
	 *  and the thread is interrupted, either before or during the activity.
	 */
	public static void portScan() throws InterruptedException{
//		String ip="127.0.0.1";
		String ip="192.168.43.194";
		//Will be a list of all  the open ports
		ArrayList<String> arr=new ArrayList<String>();
		long millis = 0;
		int startPortRange, stopPortRange, userChoose = 0, timeout = 0;
		Scanner sc = new Scanner(System.in);
		System.out.println("Please select your scan option:");
		System.out.println("1. Slow scan of ports");
		System.out.println("2. Random scan of ports");
		System.out.println("3. Effective scan of ports");
		userChoose = sc.nextInt();
		switch (userChoose) {
		case 1: { 
			// slow scan
			System.out.print("Please select start range to scan: ");
			startPortRange = sc.nextInt();
			System.out.println();
			System.out.print("Please select stop range to stop the scan: ");
			stopPortRange = sc.nextInt();
			System.out.println();
			System.out.print("Select (Seconds-Not milli please) period of time to wait between group of ports: ");
			timeout = sc.nextInt();
			System.out.println();
			millis = System.currentTimeMillis();
			int counter = 0;
			for (int i = startPortRange; i <= stopPortRange; i++) {
				try {
					String temp=null;
					Socket serverSok = new Socket();
					//timeout of 100/1000 m/s
					serverSok.connect(new InetSocketAddress(ip, i), 100); 
					System.out.println("Port in use: " + i);
					temp="Port in use: " + i;
					arr.add(temp);
					counter++;
					// default parameter to "group" of ports (10)
					if (timeout!=0 && counter == 10) { 
						Thread.sleep(timeout * 1000);
						counter = 0;
					}
					serverSok.close();
				} catch (UnknownHostException e) {
					System.out.println("Exception occured: Unknown Host or Bad IP");
					break;
				} catch (Exception e) {
					String temp=null;
					System.out.println("Port not in use: " + i);
					temp="Port not in use: " + i;
					arr.add(temp);
					counter++;
					 // default parameter to "group" of ports (10)
					if (timeout!=0 && counter == 10) {
						Thread.sleep(timeout * 1000);
						counter = 0;
					}
				}
			}
			writeTXT(arr,ip);
			break;
		}

		case 2: { 
			// random scan
			ArrayList<Integer> used=new ArrayList<Integer>();
			System.out.print("Please select start range to scan: ");
			startPortRange = sc.nextInt();
			System.out.println();
			System.out.print("Please select stop range to stop the scan: ");
			stopPortRange = sc.nextInt();
			System.out.println();
			System.out.print("Select (Seconds-Not milli please) period of time to wait between group of ports: ");
			timeout = sc.nextInt();
			System.out.println();
			millis = System.currentTimeMillis();
			int counter = 0;
			int random=0;
			for (int i = startPortRange; i <= stopPortRange; i++) {
				random=startPortRange + (int)(Math.random() * ((stopPortRange - startPortRange) + 1));
				while(used.contains(random))
					random=startPortRange + (int)(Math.random() * ((stopPortRange - startPortRange) + 1));
				try {
					String temp=null;
					Socket serverSok = new Socket();
					serverSok.connect(new InetSocketAddress(ip, random), 100);
					System.out.println("Port in use: " + random);
					temp="Port in use: " + random;
					arr.add(temp);
					used.add(random);
					counter++;
					// default parameter to "group" of ports (10)
					if (timeout!=0 && counter == 10) { 
						Thread.sleep(timeout * 1000);
						counter = 0;
					}
					serverSok.close();
				} catch (UnknownHostException e) {
					System.out.println("Exception occured: Unknown Host or Bad IP");
					break;
				} catch (Exception e) {
					String temp=null;
					System.out.println("Port not in use: " + random);
					temp="Port not in use: " + random;
					arr.add(temp);
					used.add(random);
					counter++;
					// default parameter to "group" of ports (10)
					if (timeout!=0 && counter == 10) { 
						Thread.sleep(timeout * 1000);
						counter = 0;
					}
				}
			}
			writeTXT(arr,ip);
			break;
		}

		case 3: { 
			/**
			 * Effective scanning with all the well-known ports
			 */
			int[] ports={1,5,7,18,20,21,22,23,25,29,37,42,43,49,53,69,70,79,
					80,103,108,109,110,115,118,119,137,139,143,150,156,161,179,
					190,194,197,389,396,443,444,445,458,546,547,563,569,1080};      
			
			System.out.print("Select (Seconds-Not milli please) period of time to wait between group of ports: ");
			timeout = sc.nextInt();
			System.out.println();
			millis = System.currentTimeMillis();
			int counter = 0;
			for (int i = 0; i < ports.length; i++) {
				try {
					String temp=null;
					Socket serverSok = new Socket();
					//timeout of 100/1000 m/s
					serverSok.connect(new InetSocketAddress(ip, ports[i]), 100); 
					System.out.println("Port in use: " + ports[i]);
					temp="Port in use: " + ports[i];
					arr.add(temp);
					counter++;
					// default parameter to "group" of ports (10)
					if (timeout!=0 && counter == 10) { 
						Thread.sleep(timeout * 1000);
						counter = 0;
					}
					serverSok.close();
				} catch (UnknownHostException e) {
					System.out.println("Exception occured: Unknown Host or Bad IP");
					break;
				} catch (Exception e) {
					String temp=null;
					System.out.println("Port not in use: " + ports[i]);
					temp="Port not in use: " + ports[i];
					arr.add(temp);
					counter++;
					// default parameter to "group" of ports (10)
					if (timeout!=0 && counter == 10) { 
						Thread.sleep(timeout * 1000);
						counter = 0;
					}
				}
			}
			writeTXT(arr,ip);
			break;
		}
		
		default:
			System.out.println("Invalid Choose, Bye Bye!");
		}

		sc.close();
		final long endTime = System.currentTimeMillis();
		System.out.println();
		System.out.println("Port scanning finished");
		System.out.println("It took us approximately " + (endTime - millis) / 1000 + " seconds to scan all the range of ports");
		System.out.println("Data file had been created (portScanning.txt)");
	}
	/**
	 * the function write to text file
	 * @param arr - the arraylist that with the open ports values
	 * @param ip -the ip address of my computer
	 */
	public static void writeTXT(ArrayList<String> arr, String ip){		
		//write the data on ports to txt file
		PrintWriter file = null;
		StringBuilder writer = new StringBuilder();
		try {
			file = new PrintWriter(new File("portScanning.txt"));
			writer.append("Ports report on ip "+ip+":\n");
			for (int i = 0; i < arr.size(); i++) {
				writer.append(arr.get(i) + "\n");
			}
			file.write(writer.toString());
			file.close();
		}
		catch (FileNotFoundException e){
			e.printStackTrace();
		}
	}
}