from ISpy.Analyzers.ISpyEvent_cfi import *
from ISpy.Analyzers.ISpyBeamSpot_cfi import *
from ISpy.Analyzers.ISpyCaloCluster_cfi import *

from ISpy.Analyzers.ISpyEBRecHit_cfi import *
from ISpy.Analyzers.ISpyEERecHit_cfi import *
from ISpy.Analyzers.ISpyESRecHit_cfi import *

ISpyEBRecHit.iSpyEBRecHitTag = cms.InputTag('reducedEcalRecHitsEB')
ISpyEERecHit.iSpyEERecHitTag = cms.InputTag('reducedEcalRecHitsEE')
ISpyESRecHit.iSpyESRecHitTag = cms.InputTag('reducedEcalRecHitsES')

from ISpy.Analyzers.ISpyPATElectron_cfi import *
from ISpy.Analyzers.ISpyPATMuon_cfi import *
from ISpy.Analyzers.ISpyPATPhoton_cfi import *
from ISpy.Analyzers.ISpyPATMET_cfi import *
from ISpy.Analyzers.ISpyPATJet_cfi import *
#from ISpy.Analyzers.ISpyPATTau_cfi import *

ISpyPATElectron.iSpyPATElectronTag = cms.InputTag('slimmedElectrons')
ISpyPATMuon.iSpyPATMuonTag = cms.InputTag('slimmedMuons')
ISpyPATPhoton.iSpyPATPhotonTag = cms.InputTag('slimmedPhotons')
ISpyPATMET.iSpyPATMETTag = cms.InputTag('slimmedMETs')
ISpyPATJet.iSpyPATJetTag = cms.InputTag('slimmedJets')
#ISpyPATTau.iSpyPATTauTag = cms.InputTag('slimmedTaus')

from ISpy.Analyzers.ISpySuperCluster_cfi import *
from ISpy.Analyzers.ISpyTriggerEvent_cfi import *
from ISpy.Analyzers.ISpyVertex_cfi import *

ISpySuperCluster.iSpySuperClusterTag = cms.InputTag('reducedEgamma','reducedSuperClusters')

iSpy_sequence = cms.Sequence(ISpyEvent*
                             ISpyBeamSpot*
                             ISpyCaloCluster*
                             ISpyEBRecHit*
                             ISpyEERecHit*
                             ISpyESRecHit*
                             ISpyPATElectron*
                             ISpyPATJet*
                             ISpyPATMET*
                             ISpyPATMuon*
                             ISpyPATPhoton*
                             #ISpyPATTau*
                             ISpySuperCluster*
                             ISpyTriggerEvent*
                             ISpyVertex)
