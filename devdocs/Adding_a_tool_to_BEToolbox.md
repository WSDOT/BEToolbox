Adding a tool to BEToolbox {#BEToolbox_NewTool}
===========================
All BEToolbox tools are runtime discoverable plug-in objects. These plug-in objects implement the IBEToolboxTool interface. 

This section describes how to add a new tool to the BEToolbox application. These tools are compiled into the BEToolbox application by the primary software development team.

BEToolbox tools can also be added by third-party developers. Third-party developers are not likely to want to compile their tools directly into the BEToolbox application itself. Third-party developers will typically use a BEToolbox Extension to add their tools. This section does not describe BEToolbox Extensions, however the basic concepts are the same. See the BEToolboxToolExample project for an example of a BEToolbox Extension tool.

CToolImpl is generic template implementation of this interface.

Use the following steps to create a new BEToolbox tool.

1. Create a document class derived from CBEToolboxDoc
2. Create a document template class derived from CEAFDocTemplate
3. Create a child frame class derived from CEAFChildFrame
4. Create a view class derived from any CView-dervied class. CEAFReportView is a commonly used base because it supports view of WBFL reports
5. Create a RGS file with the COM registration information and add it to the application resources.
6. Create a tool class that inherits from CToolImpl.

It is recommended that you use a previously created BEToolbox tool as a guide. Make copies of the document, document template, child frame, and view classes (h, cpp, and rgs), makes for an easy way to get started. This may be obvious, but be sure to rename the classes and update the RGS file.

**Do not reuse the GUID of a previously created tool**

Steps 1 - 4 are straight forward.

Creating the tool class
------------------------
The RGS file contains COM registration information. This includes the GUID for the BEToolbox tool. Before this file can be created, you need the tool's GUID. It is easiest to take care of this by creating the tool class.

Create a new CToolImpl derived class in Tools.h. This code snippet is for the Spectra tool.
~~~~~~~~~~~~~~
// {91F9BC47-B245-4E2E-B642-9F926C5A5E14}
DEFINE_GUID(CLSID_SpectraTool,
   0x91f9bc47, 0xb245, 0x4e2e, 0xb6, 0x42, 0x9f, 0x92, 0x6c, 0x5a, 0x5e, 0x14);
class CSpectraTool : public CToolImpl<CSpectraTool, &CLSID_SpectraTool, IDR_SPECTRATOOL, CSpectraDoc, CSpectraChildFrame, CSpectraRptView, CSpectraDocTemplate, IDR_SPECTRA>
{
public:
   virtual CString GetName() const override { return _T("Spectra"); }
};
OBJECT_ENTRY_AUTO(CLSID_SpectraTool, CSpectraTool)
~~~~~~~~~~~~~~

Using the CreateGuid tool found on the Tools menu in Visual Studio, get a new GUID of the form `DEFINE_GUID(...)`. This is option 2 in the GUID format list.

Copy this GUID code into Tools.h above your new class. Replace `<<name>>` with `CLSID_YourClass`.

Also replace CSpectraTool, CLSID_SpectraTool, IDR_SPECTRATOOL, CSpectraDoc, CSpectraChildFrame, CSpectraRptView, CSpectraDocTemplate, and IDR_SPECTRA with your classes and constants. Replace IDR_SPECTATOOL with IDR_YourClassTOOL and IDR_SPECTRA with IDR_YourClass. Use all CAPS for this constants.

Complete the definition of your tool class as shown in the CSpectraTool example.

Creating the COM Registrition Information (RGS) File
-----------------------------------------
This is easiest if you Copy/Paste/Rename a previously created file. Here is the content of SpectraTool.rgs.

~~~~~~~~~~~~~~~~~~
HKCR
{
	BEToolbox.Spectra.1 = s 'Spectra'
	{
		CLSID = s '{91F9BC47-B245-4E2E-B642-9F926C5A5E14}'
	}
	BEToolbox.Spectra = s 'Spectra'
	{
		CLSID = s '{91F9BC47-B245-4E2E-B642-9F926C5A5E14}'
		CurVer = s 'BEToolbox.Spectra.1'
	}
	NoRemove CLSID
	{
		ForceRemove {91F9BC47-B245-4E2E-B642-9F926C5A5E14} = s 'Spectra'
		{
			ProgID = s 'BEToolbox.Spectra.1'
			VersionIndependentProgID = s 'BEToolbox.Spectra'
			InprocServer32 = s '%MODULE%'
			{
				val ThreadingModel = s 'Apartment'
			}
			'TypeLib' = s '{}'
		}
	}
}
~~~~~~~~~~~~~~~~~~

Replace Spectra with YourTool. Replace the GUID with the GUID for your class that you created in Tools.h. **Getting this GUID correct is extremely important**.

Adding the RGS file to the application resources
------------------------------------------------
Open the resources.h file and define two macros, IDR_YourClassTOOL and IDR_YourClass, using all CAPs.

Open the BEToolbox.rc file using a text editor, not the GUI resource editor. To do this, find the BEToolbox.rc file in the Solution Explorer, right click on it and select Open With, and choose the C++ Source Code Editor option.

Find the section for REGISTRY data. Add a record for your RGS file. An example for the Spectra tool is
~~~~~~~~~~
IDR_SPECTRATOOL         REGISTRY                ".\\Spectra\\SpectraTool.rgs"
~~~~~~~~~~

Defining other key MFC resources
--------------------------------
While editing RC file, it is a good time to create the menu and other key resources. Again, it is easiest to find the resources for the Spectra tool and Copy/Paste/Modify them. Once the resource is created, it can subsequently be edited using the GUI resource editor.

Here is the coding for a menu. Just Copy/Paste and change IDR_SPECTRA to IDR_YourTool
~~~~~~~~~~~~
IDR_SPECTRA MENU
BEGIN
    POPUP "&File"
    BEGIN
        MENUITEM "&New\tCtrl+N",                ID_FILE_NEW
        MENUITEM "&Open...\tCtrl+O",            ID_FILE_OPEN
        MENUITEM "&Close",                      ID_FILE_CLOSE
        MENUITEM SEPARATOR
        MENUITEM "&Save\tCtrl+S",               ID_FILE_SAVE
        MENUITEM "Save &As...",                 ID_FILE_SAVE_AS
        MENUITEM SEPARATOR
        MENUITEM "P&rint Setup...",             ID_FILE_PRINT_SETUP
        MENUITEM "&Print...\tCtrl+P",           ID_FILE_PRINT
        MENUITEM SEPARATOR
        MENUITEM "Recent File",                 ID_FILE_MRU_FILE1
        MENUITEM SEPARATOR
        MENUITEM "E&xit",                       ID_APP_EXIT
    END
    POPUP "&Options"
    BEGIN
        MENUITEM "AutoSave...",                 EAFID_OPTIONS_AUTOSAVE
    END
    POPUP "&Help"
    BEGIN
        MENUITEM "&Help Topics",                ID_HELP_FINDER
        MENUITEM "Help Source",                 EAFID_HELP_SOURCE
        MENUITEM SEPARATOR
        MENUITEM "&About BridgeLink...",        ID_APP_ABOUT
        MENUITEM "About &BEToolbox...",         ID_HELP_ABOUTBETOOLBOX
    END
END
~~~~~~~~~~~~

Add your icon by Copy/Paste/Modify. Copy/Paste/Rename the ICO file (you can edit it later with the GUI editor). Change the IDR_SPECTRA macro to IDR_YourTool
~~~~~~~~~~~
IDR_SPECTRA             ICON                    "res\\Spectra.ico"
~~~~~~~~~~~

Finally, add a string table for your tool. Replace IDR_SPECTRA with IDR_YourTool and change Spectra to the name of your tool. This specially coded string provides information to the File Open/New dialog and other built-in MFC functions that deal with creating, opening, and saving files and displaying file information.
~~~~~~~~~~~~~~~~
STRINGTABLE
BEGIN
    IDR_SPECTRA             "Spectra\nSpectra\nBEToolbox\nSpectra (*.Spectra)\n.Spectra\nSpectra.Document\nSpectra Document"
END
~~~~~~~~~~~~~~~~

Using a Dialog Bar Interface
------------------------------
Some tools use a MFC Dialog Bar as the primary interface for input parameters. This is the case with the Spectra tool. If you plan to use a dialog bar, copy the IDD_SPECTRA dialog resource, renaming IDD_SPECTRA to IDD_YourTool and define IDD_YourTool in resource.h. You will be able to edit the dialog bar with the GUI editor once the framework for your tool is complete.

To use the dialog bar, you will need to create your own CDialogBar subclass. Copy/Paste/Rename SpectraDlg.h and .cpp, rename CSpectraDialogBar to CYourToolDialogBar, and rename IDD_SPECTRA to IDD_YourTool.

Make your dialog bar a member of your child frame class and create the dialog bar window in your child frame class's OnCreate method. Use CSpectrChildFrame as a guide.

Registering your tool
---------------------
As described above, all BEToolbox are runtime discoverable objects. They must be registered with the Windows registry to be discovered. This is accomplished in BEToolbox.cpp in the Register function.

Again, Copy/Paste/Modify will make this easy. Open BEToolbox.cpp and go to the end of the Register function. Copy/Paste the following code, and replace CLSID_SpectraTool with the CLSID defined for your tool from Tools.h.

~~~~~~~~~~~~~~~~~~~~
   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_SpectraTool, CATID_BEToolboxTool, bRegister);
   if (FAILED(hr))
   {
      return hr;
   }
~~~~~~~~~~~~~~~~~~~~

Add files to the project
------------------------
The last step before compiling your new BEToolbox tool is to add your new source files to the BEToolbox project. While not required, it helps to keep files organized by creating a filter folder for each tool and then having filter folders for Header Files, Resource Files, and Source Files. Create these filters, then add your new files to them based on file type.

Test your tool
--------------
At this point, you should be able to compile your tool. It will show up with BEToolbox in the New Project dialog.