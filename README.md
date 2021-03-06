# LAMMPS-USER-CONP2
The updated constant potential plugin for LAMMPS

# Summary

The main fixes contained in this package are:

conp -- updates the original lammps-conp (https://github.com/zhenxingwang/lammps-conp)
to

(1) enforce electroneutrality of electrodes

(2) allow equal-style variables for time-varying cell potential difference

(3) precompute some constant vectors for some performance enhancement and reduced communication

(4) output total induced charge as a scalar

All of these conp improvements also apply to the next two versions.

conp/noslab -- removes slab correction terms from conp, primarily for implementing two-slab simulations, using the method inspired by Raiteri et al in [1].

conp/ff -- removes slab correction terms and calculates electrode induced charges using the finite field method introduced by Dufils et al in [2],
instead of directly assigning target potentials to electrode atoms. *WARNING: This fix requires "fix efield" to be manually added to the script, see below.

*Note: In [2], electrode atoms are mentioned as being "set at 0V". This is automatically
done inside the conp/ff code, and so the fix conp/ff command has the same syntax as the other conps.

# Conp/dyn2 -- dynamic timestepping

conp/dyn2 attempts to get away with explicitly re-evaluating the kspace and pair parts of the B-vector every N timesteps!

In between, it uses a quadratic extrapolation from the last two explicit calculations to predict the instantaneous B-vector. It accumulates these changes and then, at the next explicit evaluation, sees how far off it is -- if the error is less than some lower tolerance, the evaluation period increases from N to N+1, and otherwise the evaluation period either stays the same or gets cut in half (rounded upward) if the error is more than some upper tolerance.

The lower and upper tolerances are currently 1e-4 and 4e-4 respectively (total square deviation divided by total square of all vector entries). The ability to explicitly specify tolerances is on the cards as well.

Currently, the ff and noslab flavors of conp do not come with dyn2 support -- but they will very, very soon!

# Installation instructions

As per the old conp: Simply copy all .cpp and .h files to the lammps/src folder, and compile.

# Usage instructions

For all conp packages, the fix command is identical to the previous version:

```
fix [ID] all conp [Nevery] [η] [Molecule-ID 1] [Molecule-ID 2] [Potential 1] [Potential 2] [Method] [Log] [Matrix_Type] [Matrix]
```

**ID** = ID of FIX command

**Nevery** = Compute charge every this many steps (set to 1 for current version)

**η** = Parameter for Gaussian charge. The unit is is angstrom<sup>-1</sup> (see note below)

**Molecule-ID 1** = Molecule ID of first electrode (the second column in data file)

**Molecule-ID 2** = Molecule ID of second electrode

**Potential 1** = Potential on first electrode (unit: V)

**Potential 2** = Potential on second electrode

**Method** = Method for solving linear equations. "inv" for inverse matrix and "cg" for conjugate gradient

**Log** = Name of log file recording time usage of different parts

**Matrix_Type** = Optional argument stating what type of matrix is read in: "org" for A matrix and "inv" for A-inverse matrix

**Matrix** = Optional argument. File name of matrix to read in. If it is assigned, A (or A-inverse) matrix is read in instead of calculated

Note that Potential 1 and Potential 2 can be v_ style variables.

# Usage note for conp/ff

Note that conp/ff currently requires "fix efield" to be separately added to the script, for example:

```
fix CONP conp/ff ... v_vleft v_vright ...
...
fix EFIELD efield all 0.0 0.0 -(v_vright-v_vleft)/lz
```

With units real or metal, fix efield already takes units of V/*length*, so no further conversion is needed.
Applying the efield to "all" or to just solvent molecules gives equivalent results 
(fix conp does not communicate with other fixes to do its math ... maybe it should, but that's a topic for another day).

# Development: Other fixes included

Other fixes contained in this package, primarily as snapshots of incremental development:

conp/v0 -- the original lammps-conp (https://github.com/zhenxingwang/lammps-conp),
    maintained for comparison

conp/v1 -- lammps-conp, but now:
    - electroneutrality guaranteed via
        - constrained cg (see T. Gingrich thesis for pseudocode)
        - inv-a-matrix projected onto e-null space
        (using matrix "S" in Salanne 2020)
    - left- and right-voltages take equal-style variables for time-dependent change

conp/v2 --
    pre-calculates the "pure electrode" charge vector q_c such that Aq_c = 1V x d
    (1V = "evscale" in LAMMPS units), then solves for the charge vector as:
    q*(DV) = q*(0) + DV x q_c, where Aq*(0) = b

conq --
    imposes a _constant total charge_ constraint on the electrodes, where the _total_ charge
    is set to - and + v_left on left and right electrodes respectively, but individual charges dynamically update.
    This is done by calculating A per timestep such that e.(A(t)q_c + q*(0)) = q_target.
    This fix outputs a scalar, which is the V which would be required to achieve the stated total charge
    in a constant potential simulation with the given solvent conformation.
    The Hamiltonian is a Legendre transform of the constant potential Hamiltonian, 
    and the resulting ensemble also corresponds to imposing a constant displacement field instead of
    a constant electric field [3]. Applications currently very uncertain, but there is certainly at least one very interesting 
    paper to be written about this!
    
 # Citations
    
[1] Dufils et al, PRL **123** (19): 195501 (2019)
https://doi.org/10.1103/PhysRevLett.123.195501

[2] Raiteri et al, J Chem Phys **153**: 164714 (2020)
https://doi.org/10.1063/5.0027876

[3] Zhang et al, J Phys Energy **2**: 032005 (2020)
https://doi.org/10.1088/2515-7655/ab9d8c
