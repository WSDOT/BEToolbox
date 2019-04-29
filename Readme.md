# BEToolbox

BEToolbox is a BridgeLink application consisting of a collection of bridge engineering utilities or tools. 

The standard BEToolbox tools are:
* BoxGdr - computes the geometric section properties of a box girder including cross sectional area, moment of inertia, and centroid
* Curvel - computes roadway elevations, grade, and crown slope with respect to a given vertical curve and superelevation transition. Alignment information can be transferred between Curvel, PGSuper and PGSplice.
* GenComp - computes the geometric section properties of a generalized composite cross section. Girder cross sections can be exported from PGSuper and PGSplice and subsequently imported into GenComp for further analysis and verification.
* GirComp - computes the geometric section properties of a built-up or rolled steel section with a composite concrete slab.
* PGStable - analyzes precast-prestressed girders for lateral stability.
* Spectra - generates a design response spectra.
* UltCol - computes axial/bending interaction for a circular reinforced concrete column. 

BEToolbox tools have a plug-in architecture. This makes it easy for third-parties to easily add new tools. See BEToolboxToolExample for details.


## Getting Started
BEToolbox extends the capabilities of the [BridgeLink](https://github.com/WSDOT/BridgeLink) application. BridgeLink must be installed on your machine to use the BEToolbox.

BEToolbox extends the capabilities of the [BridgeLink](https://github.com/WSDOT/BridgeLink) bridge engineering suite of tools by adding light weight engineering tools. This repository holds the source code for the BEToolbox application. If you are looking to download and use BEToolbox, download and install the BridgeLink bridge engineering suite from [WSDOT](http://www.wsdot.wa.gov) at [http://www.wsdot.wa.gov/eesc/bridge/software](http://www.wsdot.wa.gov/eesc/bridge/software).


## Authors

* **Richard Brice, PE** - [WSDOT](https://github.com/RickBrice)

## License

This project is licensed under the [Alternate Route Open Source License](http://www.wsdot.wa.gov/eesc/bridge/alternateroute/arosl.htm)
