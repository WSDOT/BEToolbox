Spectra {#spectra}
=====================
Spectra computes a design response spectrum using the Three-Point method defined in the AASHTO Guide Specifications for LRFD Seismic Bridge Design, 2nd Edition, 2011.

Spectral accelerations are based on the 2014 USGS Seismic Hazard Map with 7% probability of exceedance in 75 years.

Input Parameters
----------------

Item | Description
-----|------------
Specification | Select the applicable specification for your project.
Site Coordinates | Enter that location of the site with decimal values of latitude and longitude. Site coordinates are limited to 24.6&deg; - 50.0&deg; N and 65&deg; - 125&deg; W.
Site Soil Classification | Use the drop down list to select the site soil classification.
[Update] | Press to update the design response spectrum.
[Export] | Exports the design response spectrum in a format that can be used by CSiBridge.

Importing Design Response Spectrum into CSiBridge
-------------------------------------------------
To import the design response spectra into CSiBridge:
1. Press [Export] to export the spectra. Select a file name and location.
2. Start CSi Bridge and create a new project or open an existing project.
3. Select **Loads > Functions Type > Response Spectrum > Expand arrow**
4. Use the *Choose Function Type to Add* drop down and select **From File**
5. Press [Add New Function...]
6. Press [Browse...] and select the file you created in Step 1.

> NOTE: The exported response spectrum file does not contain header lines

> NOTE: The exported response spectrum file is in Frequency vs Value format (the default setting for CSiBridge)