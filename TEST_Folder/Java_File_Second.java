///////////////////////////////////////////////////////////////////////////////////////////////
//NAME :AVIK DAS																			 //
//Date : 03/05/2014																			 //				
///////////////////////////////////////////////////////////////////////////////////////////////

import java.io.*;
import java.net.URL;

import com.vmware.vim25.*;
import com.vmware.vim25.mo.*;

import java.text.SimpleDateFormat;
import java.util.*;
public class MYVM {
	
	//--------------------------------------To check the CPU,Memory usage for VM and Server--------------// 
	public static void CheckUsage(ServiceInstance si,String VMname,String ip)
	{
		BufferedWriter writer = null;
		String timeLog = new SimpleDateFormat("yyyyMMdd_HHmmss").format(Calendar.getInstance().getTime());
		File logFile = new File(timeLog);
		
		
		int cputhreshhold=10;
		int memthreshhold=50;
		Folder rootFlder = si.getRootFolder();
		 InventoryNavigator rootNavig = new InventoryNavigator(rootFlder);
		 try {
			System.out.println(logFile.getCanonicalPath());
			writer = new BufferedWriter(new FileWriter(logFile));
			
			ManagedEntity vme = rootNavig.searchManagedEntity("VirtualMachine",VMname);
			ManagedEntity hoste = rootNavig.searchManagedEntity("HostSystem",ip);
			VirtualMachine virm = (VirtualMachine) vme;
			HostSystem VMhost = (HostSystem)hoste;
			int counter=0;
			String timestamp = new SimpleDateFormat("dd_HHmmss").format(Calendar.getInstance().getTime());
			int maxprintcount=0;
			while(maxprintcount<100)
			{
			
				writer.write("\n");
				writer.write(timestamp);
			//	System.out.println("Virtual Machine Memory:"+virm.getSummary().quickStats.getGuestMemoryUsage());
				writer.write("Virtual Machine Memory:"+virm.getSummary().quickStats.getGuestMemoryUsage());
				writer.write("\n");
			//	System.out.println("Virtual Machine CPU Usage:"+virm.getSummary().quickStats.getOverallCpuUsage());
				writer.write("Virtual Machine CPU Usage:"+virm.getSummary().quickStats.getOverallCpuUsage());
				writer.write("\n");
			//	System.out.println(" Server Memory:"+VMhost.getSummary().quickStats.getOverallMemoryUsage());
				writer.write(" Server Memory:"+VMhost.getSummary().quickStats.getOverallMemoryUsage());
				writer.write("\n");
			//	System.out.println("Server USage:"+VMhost.getSummary().quickStats.getOverallCpuUsage());
			    writer.write("Server USage:"+VMhost.getSummary().quickStats.getOverallCpuUsage());
			    writer.write("\n");
			    Integer VMCPU= VMhost.getSummary().quickStats.getOverallCpuUsage();
			    Integer VMMem=VMhost.getSummary().quickStats.getOverallMemoryUsage();
			    
			    Thread.sleep(10000);  // Timer to check  after 10 sec
			    
			    //After 2 min this will execute
			    if(counter>12) 
			    {	
			    	
			    	if(VMCPU>cputhreshhold || VMMem >memthreshhold)
			          {
			        	 // System.out.println("Move Virtual Machine");
			        	  writer.write("Move Virtual Machine \n");
			        	  MoveVM(si,VMname);   //Method to move
			        	  break;
			          }
			    }
			else if(counter>12)
			{
				counter=0;
			}
			 counter++;   
			 maxprintcount++;
			}
			
		} 
		 catch (Exception e) {
			
			e.printStackTrace();
		}
		 finally
		 {
			 try {
	                
	                writer.close();
	            } catch (Exception e) {
	            }
		 }
	}
	
	//----------------------------------Method to move VM ------------------------------------------//
	public static void MoveVM(ServiceInstance si,String VMname)
	{
		String newHostName="128.230.96.111";//128.230.96.111 //128.230.96.41
		Folder rootFlder = si.getRootFolder();
		 
		 try {
			 HostSystem newHost = (HostSystem) new InventoryNavigator(rootFlder).searchManagedEntity("HostSystem",newHostName);
             ComputeResource cr = (ComputeResource) newHost.getParent();
             InventoryNavigator rootNavig = new InventoryNavigator(rootFlder);
             ManagedEntity vme = rootNavig.searchManagedEntity("VirtualMachine",VMname);
             VirtualMachine virm = (VirtualMachine) vme;
			 Task task = virm.migrateVM_Task(cr.getResourcePool(), newHost, VirtualMachineMovePriority.highPriority, VirtualMachinePowerState.poweredOn);
			 if(task.waitForTask()==Task.SUCCESS)
			 {
				 System.out.println("VM Moved");
				 
			 }
			 else
			 {
				 System.out.println("VM Failed!");
				 TaskInfo info = task.getTaskInfo();
				 System.out.println(info.getError().getFault());
			 }
			 si.getServerConnection().logout();
		 }
		 catch(Exception e)
		 {e.printStackTrace();}
	}

	public static void main(String[] args) {
		
		
		try{
			
		//Creation of service instance
			ServiceInstance serviceInstance = new ServiceInstance(
			          new URL("https://128.230.247.52/sdk"), "AD\\apandey", "Hanu2Man!", true);
			// ServiceInstance serviceInstance = new ServiceInstance(
			 		//new URL(args[0]), args[1], args[2], true);
			 CheckUsage(serviceInstance,"apandey_vm","128.230.247.52");
			// CheckUsage(serviceInstance,args[3],"128.230.96.41");
			
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}

	}

}
