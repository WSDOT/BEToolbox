Lifting View {#pgstable_lifting_view}
=====================
The Lifting View provides the parameters for lifting analysis as well as the lifting analysis report. Use the input fields on the left side of the view to define the specific parameters of a lifting analysis. The report on the right side of the view will automatically update as the analysis parameters change.

## Effective Prestress Force at Lifting (Fpe)
Enter the effective prestress force at the time of lifting. If the Prestress Definition in the Girder View is set to Simplified, enter the effective prestress force in the straight, harped, and temporary strands. If the effective Prestress Defintiion in the Girder View is set to Detailed, press [Edit Fpe] and enter the prestress force and its location relative to the top or bottom of the girder.

## Material
Enter the girder material at the time of lifting.

Item | Description
-----|---------------
f'ci | Concrete strength
Eci  | Modulus of elasticity. If this box is checked, enter a value, otherwise the modulus of elasticity will be computed
fr   | Modulus of rupture. Enter the coefficient for computing the modulus of rupture
K1, K2 | Averaging and bounding factor used when computing the modulus of elasticity

## Analysis Parameters
Enter the analysis parameters

Item | Description
-----|-----------------
Lift Point | Distance from the ends of the girder to the lift point
Impact Up/Down | Dynamic load allowance (impact)
Lift Angle from Horizontal | Inclination of lifting cables measured from horizontal.
Wind Loading | Select the wind loading type and enter the loading parameters. For Wind Speed, enter the wind velocity and wind pressure will be comptued according to LRFD 3.8.1.2. For Wind Pressure, enter a pressure loading applied to the vertical surface of the girder.
Camber | Select the method for accounting for camber. For the offset method, the center of gravity of the girder is adusted by a percentage to allow for camber. For the direct method, an actual camber is input and a camber multiplier can be specified
Lateral Camber | Asymmetric girders can have lateral deflections due to the geometry of the section. Prestressing that is not symmetric with respect to the vertical axis of the girder can also have lateral deflection. Enter the lateral camber if applicable.
Distance to roll axis above Top of Girder | Location of the roll axis above the top of the girder. The lifting device must be sufficiently rigid to raise the roll axis.
Sweep Tolerance | Enter the girder lateral sweep tolerance. Lateral sweep is added to lateral camber.
Support Placement Tolerance | Enter the lateral tolerance for placement of the lifting devices.

## Criteria
Define the analysis criteria

Item | Description
-----|----------
Factors of Safety | Enter the factors of safety against cracking and failure
Evaluate stresses | Select the method for computing and evaluating stresses in the girder. Stresses computed for a plumb girder do not account for the tilt of the girder into its equilibrium position. Stresses computed for a tilted girder account for the girder tilting into its equilibrium position.
Compression | Enter the coefficients for computing allowable compression. The general compression limit is used for compression stress in the general bending condition (girder weight plus prestress). The lateral bending compression limit is used for peak stresses at the section extremities when lateral bending due to girder tilt and wind are considered.
Tension | Enter the coefficients for computing allowable tension with and without sufficient bonded reinforcement