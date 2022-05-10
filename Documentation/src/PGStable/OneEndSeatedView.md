Lifted from One End View {#pgstable_oneendseated_view}
=====================
The Lifted from One End View provides the parameters for the stability analysis as well as the analysis report. Use the input fields on the left side of the view to define the specific parameters of the analysis. The report on the right side of the view will automatically update as the analysis parameters change.

Analysis is performed for the special case of the girder lifted from one end while seated at the other.

## Copy
Press the [Copy from Lifting and Hauling Tabs] to copy the prestressing, material, analysis parameters and criteria from previously defined analyses. Parameters are copied from the hauling tab with the exception of the Height of lift point above top of girder and Lift Point Tolerance. These two parameters are copied from the Lifting tab. The Seated Support parameters is taken to be the Trailing Bunk distance from the Hauling tab.

## Effective Prestress Force (Fpe)
Enter the effective prestress force. If the Prestress Definition in the Girder View is set to Simplified, enter the effective prestress force in the straight, harped, and temporary strands. If the effective Prestress Definition in the Girder View is set to Detailed, press [Edit Fpe] and enter the prestress force and its location relative to the top or bottom of the girder.

## Material
Enter the girder material at the time of hauling.

Item | Description
-----|---------------
f'c | Concrete strength
Ec  | Modulus of elasticity. If this box is checked, enter a value, otherwise the modulus of elasticity will be computed
fr   | Modulus of rupture. Enter the coefficient for computing the modulus of rupture


## Analysis Parameters
Enter the analysis parameters

Item | Description
-----|-----------------
Seated Support | Distance to the seated support to the end of the girder
Hanging Support | Distance to the lift point to the end of the girder
Impact Up/Down | Dynamic load allowance (impact). 
Support slope | Enter the slope of the support.
Wind Loading | Select the wind loading type and enter the loading parameters. For Wind Speed, enter the wind velocity and wind pressure will be comptued according to LRFD 3.8.1.2. For Wind Pressure, enter a pressure loading applied to the vertical surface of the girder.
Overhang Bracket | Deck form overhang brackets may be attached to the girder prior to transport to facilitate on site construction. Enter an equivalent weight per unit length and eccentricity of the resultant overhang bracket weight from the centerline of the girder. Positive value for weight is in the direction of gravity (downwards). Positive values for eccentricity is in the direction of sweep.
Vertical Camber | Enter the camber and a camber multiplier, if applicable. This value should include formed camber when applicable.
Lateral Camber | Asymmetric girders can have lateral deflections due to the geometry of the section. Prestressing that is not symmetric with respect to the vertical axis of the girder can also have lateral deflection. Enter the lateral camber if applicable.
Height of lift point above top of girder | Enter the height of the lift point above the top of the girder at the lifted end
Lift Point Tolerance | Enter the lateral tolerance for placement of the lifting devices.
Haul Truck | Select a haul truck from the drop down list or select "Define haul truck" to input the haul truck parameters
Height of girder bottom above roadway | Enter the location of the bottom of the girder from the roadway surface directly above the roll center
Height of roll axis above roadway | Enter the location of the roll axis above the roadway surface
Total Truck Roll Stiffness | Enter the total roll stiffness of the haul truck
Fraction of roll stiffness at seated end | Enter the fraction of the total truck roll stiffness attributed to the seat end of the girder.
Truck Width | Enter the center to center width of the truck wheels
Sweep Tolerance | Enter the girder lateral sweep tolerance. Lateral sweep is added to lateral camber.
Support Placement Tolerance | Enter the lateral tolerance for placement of the bunking devices.

## Criteria
Define the analysis criteria

Item | Description
-----|----------
Factors of Safety | Enter the factors of safety against cracking and failure
Evaluate stresses | Select the method for computing and evaluating stresses in the girder. Stresses computed for a plumb girder do not account for the tilt of the girder into its equilibrium position. Stresses computed for a tilted girder account for the girder tilting into its equilibrium position.
Compression | Enter the coefficients for computing allowable compression. The general compression limit is used for compression stress in the general bending condition (girder weight plus prestress). The lateral bending compression limit is used for peak stresses at the section extremities when lateral bending due to girder tilt, wind, and centrifugal force are considered.
Tension | Enter the coefficients for computing allowable tension with and without sufficient bonded reinforcement.
