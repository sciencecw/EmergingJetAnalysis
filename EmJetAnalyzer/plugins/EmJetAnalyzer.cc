// -*- C++ -*-
//
// Package:    EmergingJetAnalysis/EmJetAnalyzer
// Class:      EmJetAnalyzer
//
/**\class EmJetAnalyzer EmJetAnalyzer.cc EmergingJetAnalysis/EmJetAnalyzer/plugins/EmJetAnalyzer.cc

 Description: Analyzer for Emerging Jet analysis, supercedes EmergingJetAnalyzer

 Implementation:
     Re-write of EmergingJetAnalyzer to be more modular.
*/
//
// Original Author:  Young Ho Shin
//         Created:  Tue, 05 Apr 2016 19:37:25 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "TrackingTools/IPTools/interface/IPTools.h"
#include "DataFormats/GeometrySurface/interface/Line.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "MagneticField/Engine/interface/MagneticField.h"

#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableVertexReconstructor.h"
#include "RecoVertex/TrimmedKalmanVertexFinder/interface/KalmanTrimmedVertexFinder.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexSmoother.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"

#include "DataFormats/JetReco/interface/GenJet.h"

// track association
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"

#include "TTree.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TStopwatch.h"

#include "EmergingJetAnalysis/EmJetAnalyzer/interface/OutputTree.h"
#include "EmergingJetAnalysis/EmJetAnalyzer/interface/EmJetEvent.h"

#ifndef OUTPUT
#define OUTPUT(x) std::cout<<#x << ": " << x << std::endl
#endif

//
// class declaration
//

using namespace emjet;

class EmJetAnalyzer : public edm::EDFilter {
  public:
    explicit EmJetAnalyzer(const edm::ParameterSet&);
    ~EmJetAnalyzer();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void beginJob() override;
    virtual bool filter(edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
    //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

    void fillJet(const reco::PFJet& ijet, Jet& ojet);
    void fillJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack);
    void fillJetVertex(const TransientVertex& ivertex, const Jet& ojet, Vertex& overtex);
    bool selectTrack(const reco::TransientTrack& itrack, const Track& otrack);
    bool selectJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, const Track& otrack);
    bool selectJetTrackForVertexing(const reco::TransientTrack& itrack, const Jet& ojet, const Track& otrack);
    bool selectJetVertex(const TransientVertex& ivertex, const Jet& ojet, const Vertex& overtex);

    // Computation functions
    double compute_alphaMax(const reco::PFJet& ijet, reco::TrackRefVector& trackRefs) const;
    double compute_pt2Sum (const TransientVertex& ivertex) const;


    // ----------member data ---------------------------
    bool isData_;

    edm::Service<TFileService> fs;
    edm::EDGetTokenT< reco::PFJetCollection > jetCollectionToken_;

    edm::ParameterSet         m_trackParameterSet;
    TrackDetectorAssociator   m_trackAssociator;
    TrackAssociatorParameters m_trackParameters;

    edm::ParameterSet         vtxconfig_;
    ConfigurableVertexReconstructor vtxmaker_;

    emjet::OutputTree otree_ ; // OutputTree object
    TTree* t_tree;

    emjet:: Event  event_  ; // Current event
    emjet:: Jet    jet_    ; // Current jet
    emjet:: Track  track_  ; // Current track
    emjet:: Vertex vertex_ ; // Current vertex
    int jet_index_    ; // Current jet index
    int track_index_  ; // Current track index
    int vertex_index_ ; // Current vertex index

    // Retrieve once per event
    // Intermediate objects used for calculations
    edm::Handle<reco::VertexCollection> primary_verticesH_;
    edm::Handle<reco::PFJetCollection> selectedJets_;
    edm::ESHandle<TransientTrackBuilder> transienttrackbuilderH_;
    std::vector<reco::TransientTrack> generalTracks_;
    edm::Handle<reco::GenParticleCollection> genParticlesH_;
    const reco::BeamSpot* theBeamSpot_;
    reco::VertexCollection selectedSecondaryVertices_;
    std::vector<TransientVertex> avrVertices_;
    edm::Handle<reco::GenJetCollection> genJets_;
    std::vector<GlobalPoint> dt_points_;
    std::vector<GlobalPoint> csc_points_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
EmJetAnalyzer::EmJetAnalyzer(const edm::ParameterSet& iConfig):
  // event_  (new Event  ()),
  // jet_    (new Jet    ()),
  // track_  (new Track  ()),
  // vertex_ (new Vertex ())
  vtxconfig_(iConfig.getParameter<edm::ParameterSet>("vertexreco")),
  vtxmaker_(vtxconfig_),
  event_  (),
  jet_    (),
  track_  (),
  vertex_ ()
{
  // Config-independent initialization
  {
    t_tree           = fs->make<TTree>("emJetTree","emJetTree");
    otree_.Branch(t_tree);
  }

  // Config-dependent initialization
  {
    isData_ = iConfig.getUntrackedParameter<bool>("isData");
    m_trackParameterSet = iConfig.getParameter<edm::ParameterSet>("TrackAssociatorParameters");
    if (isData_) {
      std::cout << "running on data" << std::endl;
    } else {
      std::cout << "running on MC"   << std::endl;
    }

    edm::ConsumesCollector iC = consumesCollector();
    m_trackParameters.loadParameters( m_trackParameterSet, iC );
    m_trackAssociator.useDefaultPropagator();

    jetCollectionToken_ = consumes< reco::PFJetCollection > (iConfig.getParameter<edm::InputTag>("srcJets"));
  }
}


EmJetAnalyzer::~EmJetAnalyzer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
EmJetAnalyzer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // Reset output tree to default values
  otree_.Init();
  // Reset Event variables
  vertex_.Init();
  jet_.Init();
  event_.Init();
  // Reset object counters
  jet_index_=0;
  track_index_=0;
  vertex_index_=0;

  event_.run   = iEvent.id().run();
  event_.event = iEvent.id().event();
  event_.lumi  = iEvent.id().luminosityBlock();
  event_.bx    = iEvent.bunchCrossing();

  // Retrieve offline beam spot (Used to constrain vertexing)
  {
    edm::Handle<reco::BeamSpot> theBeamSpotHandle;
    iEvent.getByLabel("offlineBeamSpot", theBeamSpotHandle);
    theBeamSpot_ = theBeamSpotHandle.product();
  }

  // Calculate basic primary vertex and pileup info :EVENTLEVEL:
  {
    iEvent.getByLabel("offlinePrimaryVerticesWithBS", primary_verticesH_);
    const reco::Vertex& primary_vertex = primary_verticesH_->at(0);
    if (!isData_) { // :MCONLY: Add true number of interactions
      edm::Handle<std::vector<PileupSummaryInfo> > PileupInfo;
      iEvent.getByLabel("addPileupInfo", PileupInfo);
      for (auto const& puInfo : *PileupInfo) {
        int bx = puInfo.getBunchCrossing();
        if (bx == 0) {
          event_.nTrueInt = puInfo.getTrueNumInteractions();
        }
      }
    }
    for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
      event_.nVtx ++;
      if ( (ipv->isFake()) || (ipv->ndof() <= 4.) || (ipv->position().Rho() > 2.0) || (fabs(ipv->position().Z()) > 24.0) ) continue; // :CUT: Primary vertex cut for counting
      event_.nGoodVtx++;
    }
  }

  // Calculate MET :EVENTLEVEL:
  {
    edm::Handle<reco::PFMETCollection> pfmet;
    iEvent.getByLabel("pfMet",pfmet);
    event_.met_pt = pfmet->begin()->pt();
    event_.met_phi = pfmet->begin()->phi();
  }

  // Retrieve selectedJets
  edm::Handle<reco::PFJetCollection> pfjetH;
  iEvent.getByToken(jetCollectionToken_, pfjetH);
  selectedJets_ = pfjetH;
  // Retrieve TransientTrackBuilder
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",transienttrackbuilderH_);
  // Retrieve generalTracks and build TransientTrackCollection
  edm::Handle<reco::TrackCollection> genTrackH;
  iEvent.getByLabel("generalTracks", genTrackH);
  generalTracks_ = transienttrackbuilderH_->build(genTrackH);

  // Reconstruct AVR vertices using all generalTracks passing basic selection
  avrVertices_.clear();
  ConfigurableVertexReconstructor avr (vtxconfig_);
  std::vector<reco::TransientTrack> tracks_for_vertexing;
  for (std::vector<reco::TransientTrack>::iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
    if ( selectTrack(*itk, track_) ) // :CUT: Apply basic track selection
      tracks_for_vertexing.push_back(*itk);
  }
  avrVertices_ = avr.vertices(tracks_for_vertexing);

  // Calculate Jet-level quantities and fill into jet_ :JETLEVEL:
  for ( reco::PFJetCollection::const_iterator jet = selectedJets_->begin(); jet != selectedJets_->end(); jet++ ) {
    jet_.Init();
    jet_.index = jet_index_;
    jet_.source = 1; // source = 1 for PF jets :JETSOURCE:
    // Fill Jet-level quantities
    fillJet(*jet, jet_);

    // Calculate Jet-Track-level quantities and fill into jet_ :JETTRACKLEVEL:
    for (std::vector<reco::TransientTrack>::iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
      if ( !selectJetTrack(*itk, jet_, track_) ) continue; // :CUT: Apply Track selection
      track_.Init();
      track_.index = track_index_;
      track_.source = 1; // source = 1 for generalTracks :TRACKSOURCE:
      track_.jet_index = jet_index_;
      // Fill Jet-Track level quantities
      fillJetTrack(*itk, jet_, track_);
      // Write current Track to Jet
      jet_.track_vector.push_back(track_);
      track_index_++;
    }

    // Per-jet vertex reconstruction
    {
      // Add tracks to be used for vertexing
      std::vector<reco::TransientTrack> tracks_for_vertexing;
      for (std::vector<reco::TransientTrack>::iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
        if ( selectJetTrackForVertexing(*itk, jet_, track_) ) // :CUT: Apply Track selection for vertexing
          tracks_for_vertexing.push_back(*itk);
      }

      // Reconstruct vertex from tracks associated with current jet
      std::vector<TransientVertex> vertices_for_current_jet;
      {
        vertices_for_current_jet = vtxmaker_.vertices(generalTracks_, tracks_for_vertexing, *theBeamSpot_);
      }
      // Fill Jet-Vertex level quantities for per-jet vertices
      for (auto vtx : vertices_for_current_jet) {
        if ( !selectJetVertex(vtx, jet_, vertex_) ) continue; // :CUT: Apply Vertex selection
        vertex_.Init();
        vertex_.index = vertex_index_;
        vertex_.source = 1; // source = 1 for per-jet AVR vertices
        // Fill Jet-Vertex level quantities
        fillJetVertex(vtx, jet_, vertex_);
        // Write current Vertex to Jet
        jet_.vertex_vector.push_back(vertex_);
        vertex_index_++;
      }
    }

    // Fill Jet-Vertex level quantities for globally reconstructed AVR vertices
    for (auto vtx : avrVertices_) {
      if ( !selectJetVertex(vtx, jet_, vertex_) ) continue; // :CUT: Apply Vertex selection
      vertex_.Init();
      vertex_.index = vertex_index_;
      vertex_.source = 2; // source = 2 for global AVR vertices
      // Fill Jet-Vertex level quantities
      fillJetVertex(vtx, jet_, vertex_);
      // Write current Vertex to Jet
      jet_.vertex_vector.push_back(vertex_);
      vertex_index_++;
    }


    // Write current Jet to Event
    event_.jet_vector.push_back(jet_);
    jet_index_++;
  }

  // Write current Event to OutputTree
  WriteEventToOutput(event_, &otree_);
  // Write OutputTree to TTree
  t_tree->Fill();

#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif
  return true;
}

// ------------ method called once each job just before starting event loop  ------------
void
EmJetAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
EmJetAnalyzer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
EmJetAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
EmJetAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
EmJetAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
EmJetAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
EmJetAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void
EmJetAnalyzer::fillJet(const reco::PFJet& ijet, Jet& ojet)
{
  // Fill basic kinematic variables
  {
    ojet.pt  = ijet.pt()  ;
    ojet.eta = ijet.eta() ;
    ojet.phi = ijet.phi() ;
    ojet.p4.SetPtEtaPhiM(ojet.pt, ojet.eta, ojet.phi, 0.);
  }

  // Fill PF Jet specific variables
  {
    ojet.cef = ijet.chargedEmEnergyFraction()     ;
    ojet.nef = ijet.neutralEmEnergyFraction()     ;
    ojet.chf = ijet.chargedHadronEnergyFraction() ;
    ojet.nhf = ijet.neutralHadronEnergyFraction() ;
    ojet.phf = ijet.photonEnergyFraction()        ;
  }

  // Fill alphaMax
  {
    reco::TrackRefVector trackRefs = ijet.getTrackRefs();
    ojet.alphaMax = compute_alphaMax(ijet, trackRefs);
  }
}

void
EmJetAnalyzer::fillJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack)
{
  auto itk = &itrack;
  // Fill basic kinematic variables
  {
    otrack.pt  = itrack.track().pt()  ;
    otrack.eta = itrack.track().eta() ;
    otrack.phi = itrack.track().phi() ;
    otrack.p4.SetPtEtaPhiM(otrack.pt, otrack.eta, otrack.phi, 0.);
  }

  // Fill geometric variables
  {
    const reco::Vertex& primary_vertex = primary_verticesH_->at(0);
    GlobalVector direction(ojet.p4.Px(), ojet.p4.Py(), ojet.p4.Pz());
    TrajectoryStateOnSurface pca = IPTools::closestApproachToJet(itk->impactPointState(), primary_vertex, direction, itk->field());
    GlobalPoint closestPoint = pca.globalPosition();
    GlobalVector jetVector = direction.unit();
    Line::PositionType posJet(GlobalPoint(primary_vertex.position().x(),primary_vertex.position().y(),primary_vertex.position().z()));
    Line::DirectionType dirJet(jetVector);
    Line jetLine(posJet, dirJet);
    GlobalVector pcaToJet = jetLine.distance(closestPoint);
    TLorentzVector& trackVector = otrack.p4;
    trackVector.SetPxPyPzE(
                           closestPoint.x() - primary_vertex.position().x(),
                           closestPoint.y() - primary_vertex.position().y(),
                           closestPoint.z() - primary_vertex.position().z(),
                           itk->track().p());
    otrack.dRToJetAxis = trackVector.DeltaR(ojet.p4);
    // Calculate PCA coordinates
    otrack.pca_r   = closestPoint.perp();
    otrack.pca_eta = closestPoint.eta();
    otrack.pca_phi = closestPoint.phi();
    otrack.distanceToJet = pcaToJet.mag();

    auto dxy_ipv = IPTools::absoluteTransverseImpactParameter(*itk, primary_vertex);
    otrack.ipXY    = fabs(dxy_ipv.second.value());
    otrack.ipXYSig = fabs(dxy_ipv.second.significance());
  }

  otrack.algo                = itk->track().algo();
  otrack.originalAlgo        = itk->track().originalAlgo();
  otrack.nHits               = itk->numberOfValidHits();
  otrack.nMissInnerHits      = itk->hitPattern().numberOfLostTrackerHits(reco::HitPattern::MISSING_INNER_HITS);
  otrack.nTrkLayers          = itk->hitPattern().trackerLayersWithMeasurement();
  otrack.nMissTrkLayers      = itk->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::TRACK_HITS);
  otrack.nMissInnerTrkLayers = itk->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::MISSING_INNER_HITS);
  otrack.nMissOuterTrkLayers = itk->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::MISSING_OUTER_HITS);
  otrack.nPxlLayers          = itk->hitPattern().pixelLayersWithMeasurement();
  otrack.nMissPxlLayers      = itk->hitPattern().pixelLayersWithoutMeasurement(reco::HitPattern::TRACK_HITS);
  otrack.nMissInnerPxlLayers = itk->hitPattern().pixelLayersWithoutMeasurement(reco::HitPattern::MISSING_INNER_HITS);
  otrack.nMissOuterPxlLayers = itk->hitPattern().pixelLayersWithoutMeasurement(reco::HitPattern::MISSING_OUTER_HITS);
}

void
EmJetAnalyzer::fillJetVertex(const TransientVertex& ivertex, const Jet& ojet, Vertex& overtex)
{
  auto vtx = reco::Vertex(ivertex);
  const reco::Vertex& primary_vertex = primary_verticesH_->at(0);
  TLorentzVector vertexVector;
  vertexVector.SetXYZT(vtx.x(), vtx.y(), vtx.z(), 0.0);
  double deltaR = vertexVector.DeltaR(ojet.p4);
  double Lxy = 0;
  float dx = primary_vertex.position().x() - vtx.position().x();
  float dy = primary_vertex.position().y() - vtx.position().y();
  Lxy = TMath::Sqrt( dx*dx + dy*dy );
  float mass = vtx.p4().mass();
  float pt2sum = compute_pt2Sum(ivertex);
  overtex.x      = ( vtx.x()      );
  overtex.y      = ( vtx.y()      );
  overtex.z      = ( vtx.z()      );
  overtex.xError = ( vtx.xError() );
  overtex.yError = ( vtx.yError() );
  overtex.zError = ( vtx.zError() );
  overtex.deltaR = ( deltaR       );
  overtex.Lxy    = ( Lxy          );
  overtex.mass   = ( mass         );
  overtex.chi2   = ( vtx.chi2()   );
  overtex.ndof   = ( vtx.ndof()   );
  overtex.pt2sum = ( pt2sum       );
}

bool
EmJetAnalyzer::selectTrack(const reco::TransientTrack& itrack, const Track& otrack)
{
  auto itk = &itrack;
  // Skip tracks with pt<1 :CUT:
  if (itk->track().pt() < 1.) return false;
  return true;
}

bool
EmJetAnalyzer::selectJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, const Track& otrack)
{
  auto itk = &itrack;
  if (!selectTrack(itrack, otrack)) return false; // :CUT: Require track to pass basic selection

  // Skip tracks with invalid point-of-closest-approach :CUT:
  const reco::Vertex& primary_vertex = primary_verticesH_->at(0);
  GlobalVector direction(ojet.p4.Px(), ojet.p4.Py(), ojet.p4.Pz());
  TrajectoryStateOnSurface pca = IPTools::closestApproachToJet(itk->impactPointState(), primary_vertex, direction, itk->field());
  if (!pca.isValid()) return false;
  GlobalPoint closestPoint = pca.globalPosition();

  // Skip tracks if point-of-closest-approach has -nan or nan x/y/z coordinates :CUT:
  if ( !( std::isfinite(closestPoint.x()) && std::isfinite(closestPoint.y()) && std::isfinite(closestPoint.z()) ) ) return false;

  // Skip tracks with deltaR > 0.4 w.r.t. current jet :CUT:
  TLorentzVector trackVector;
  trackVector.SetPxPyPzE(
                         closestPoint.x() - primary_vertex.position().x(),
                         closestPoint.y() - primary_vertex.position().y(),
                         closestPoint.z() - primary_vertex.position().z(),
                         itk->track().p());
  float deltaR = trackVector.DeltaR(ojet.p4);
  // if (itrack==1) std::cout << "deltaR: " << deltaR << std::endl;
  if (deltaR > 0.4) return false;
  return true;
}

bool
EmJetAnalyzer::selectJetTrackForVertexing(const reco::TransientTrack& itrack, const Jet& ojet, const Track& otrack)
{
  return selectJetTrack(itrack, ojet, otrack);
}

bool
EmJetAnalyzer::selectJetVertex(const TransientVertex& ivertex, const Jet& ojet, const Vertex& overtex)
{
  auto vtx = reco::Vertex(ivertex);

  TLorentzVector vertexVector;
  vertexVector.SetXYZT(vtx.x(), vtx.y(), vtx.z(), 0.0);
  double deltaR = vertexVector.DeltaR(ojet.p4);
  if (deltaR > 0.4) return false; // Ignore vertices outside jet cone :CUT:
  return true;
}

// Calculate jet alphaMax
double
EmJetAnalyzer::compute_alphaMax(const reco::PFJet& ijet, reco::TrackRefVector& trackRefs) const
{
  double alphaMax = -1.;
  // Loop over all tracks and calculate scalar pt-sum of all tracks in current jet
  double jet_pt_sum = 0.;
  for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
    jet_pt_sum += (*ijt)->pt();
  } // End of track loop

  auto ipv_chosen = primary_verticesH_->end(); // iterator to chosen primary vertex
  double max_vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
  // Loop over all PVs and choose the one with highest scalar pt contribution to jet
  for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
    double vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
    // std::cout << "New vertex\n";
    for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
      double trackWeight = ipv->trackWeight(*ijt);
      // std::cout << "Track weight: " << trackWeight << std::endl;
      if (trackWeight > 0) vertex_pt_sum += (*ijt)->pt();
    } // End of track loop
    if (vertex_pt_sum > max_vertex_pt_sum) {
      max_vertex_pt_sum = vertex_pt_sum;
      ipv_chosen = ipv;
      // Calculate alpha
      alphaMax = vertex_pt_sum / jet_pt_sum;
    }
  } // End of vertex loop
  return alphaMax;
}

double
EmJetAnalyzer::compute_pt2Sum (const TransientVertex& ivertex) const {
  auto vtx = reco::Vertex(ivertex);
  // Modified from reco::Vertex::p4()
  double sum = 0.;
  double pt = 0.;
  if(vtx.hasRefittedTracks()) {
    for(std::vector<reco::Track>::const_iterator iter = vtx.refittedTracks().begin();
        iter != vtx.refittedTracks().end(); ++iter) {
      pt = iter->pt();
      sum += pt*pt;
    }
  }
  else
    {
      for(std::vector<reco::TrackBaseRef>::const_iterator iter = vtx.tracks_begin();
          iter != vtx.tracks_end(); iter++) {
        pt = (*iter)->pt();
        sum += pt*pt;
      }
    }
  return sum;
}

//define this as a plug-in
DEFINE_FWK_MODULE(EmJetAnalyzer);