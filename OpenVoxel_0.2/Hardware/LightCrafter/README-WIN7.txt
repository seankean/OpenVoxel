If either the LightCrafter or the PC sleeps or becomes unplugged after operation - this process was required to allow the LightCrafter software to make a connection - although the driver had previously been installed, and the device was identified by the operating system as a functioning NDIS network adapter.

Windows 7 specific:
~	Power down the LightCrafter and unplug the usb cable from the PC and the LightCrafter.  
~	Close the LightCrafter application as well.
~	Power up the LightCrafter, wait till its finishes its boot and displays the test pattern.
~	Connect the usb cable first to the LightCrafter and then to the PC.
~	Wait for the PC to finish identifying the device.
~	Open the device manager: START->Rclick on Computer->Manage.  Yes to the permissions dialog and then select Device manager.
~	Navigate the tree to Network Adapters and expand by clicking the arrow to the left.
~	There should be an entry for a NDIS adapter. If not, go to the usb devices branch, expand and look for the yellow 'i' icon next to an unidentified usb device.  
~	Rclick the device and select update driver.
~	Select Browse my computer for driver software->Let me pick from a list of device drivers.
~	Select Microsoft Corporation (not Microsoft) and select 'Remote NDIS Compatable Device'.  OK to the warning that the driver is unverified to work with this hardware.
~	Start the LightCrafter application and do a 'get version' to test communication is sucessfull!!!
