# Momentum threshold study

## Overview

In order to properly define the signal, we need to put a cut on the momentum of our MCParticles such that they can be deemed detectable.
This is done by finding, for each particle type, the momentum at which the MCParticles start to become reconstructable, as defined by
Pandora's standard metrics.

## Relevant code

The algorithm to do this is the `MomentumThresholdAlgorithm`. This algorithm get the list of all neutrino final-state particles that are
"visible" - that is they are sufficiently long-lived to produce a track or will initiate an electromagnetic shower. With these particles we
get their: particle type, true momentum, number of hits in each view and if they are deemed reconstructable by Pandora.

The plotting macro `plots.C` will make histograms of the true particle momentum for each type under three conditions:
- All MCParticles
- MCParticles with at least one hit in any plane
- MCParticles that are deemed reconstructable by Pandora

## Usage

```
root -l -b
.L plots.C+
plots(inputFilePathString, treeNameString)
```
