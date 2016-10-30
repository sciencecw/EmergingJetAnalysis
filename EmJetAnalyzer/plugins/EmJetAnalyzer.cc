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

// Useful keywords
// :MCONLY: Code that applies only when processing MC events
// :CUT: Code that applies cuts to objects/events
// :EVENTLEVEL: Code that calculates event-level quantities
// :JETLEVEL: Code that calculates jet-level quantities
// :JETTRACKLEVEL: Code that calculates jet-track-level quantities
// :JETSOURCE: Code that assigns "source" variable for a jet
// :TRACKSOURCE: Code that assigns "source" variable for a track
// :VERTEXSOURCE:


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

// Hit pattern
#include "PhysicsTools/RecoUtils/interface/CheckHitPattern.h"

// track association
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"

// Jet Tracks Association
#include "DataFormats/JetReco/interface/JetTracksAssociation.h"

#include "TTree.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TStopwatch.h"
#include "TParameter.h"

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

    void prepareJet(const reco::PFJet& ijet, Jet& ojet, int source);
    void prepareJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack, int source);
    void prepareJetVertex(const TransientVertex& ivertex, const Jet& ojet, Vertex& overtex, int source);
    void prepareJetVertexTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack, const TransientVertex& ivertex, int source, const edm::EventSetup& iSetup);
    void fillJet(const reco::PFJet& ijet, Jet& ojet);
    void fillJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack);
    void fillJetVertex(const TransientVertex& ivertex, const Jet& ojet, Vertex& overtex);
    bool selectTrack(const reco::TransientTrack& itrack, const Track& otrack);
    bool selectJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, const Track& otrack);
    bool selectJetTrackForVertexing(const reco::TransientTrack& itrack, const Jet& ojet, const Track& otrack);
    bool selectJetVertex(const TransientVertex& ivertex, const Jet& ojet, const Vertex& overtex);

    // Computation functions
    double compute_alphaMax(const reco::PFJet& ijet, reco::TrackRefVector& trackRefs) const;
    double compute_alphaMax(reco::TrackRefVector& trackRefs) const;
    int    compute_nDarkPions(const reco::PFJet& ijet) const;
    int    compute_nDarkGluons(const reco::PFJet& ijet) const;
    double compute_pt2Sum (const TransientVertex& ivertex) const;

    // Utility functions
    reco::TrackRefVector MergeTracks(reco::TrackRefVector trks1,  reco::TrackRefVector trks2);


    // Scanning functions (Called for specific events/objects)
    void jetdump(reco::TrackRefVector& trackRefs) const;


    // ----------member data ---------------------------
    bool isData_;

    edm::Service<TFileService> fs;
    edm::EDGetTokenT< reco::PFJetCollection > jetCollectionToken_;
    edm::EDGetTokenT<edm::View<reco::CaloJet> > jet_collT_;
    edm::EDGetTokenT<reco::JetTracksAssociationCollection> assocVTXToken_;
    edm::EDGetTokenT<reco::JetTracksAssociationCollection> assocCALOToken_;

    edm::ParameterSet         m_trackParameterSet;
    TrackDetectorAssociator   m_trackAssociator;
    TrackAssociatorParameters m_trackParameters;

    edm::ParameterSet         vtxconfig_;
    ConfigurableVertexReconstructor vtxmaker_;

    emjet::OutputTree otree_ ; // OutputTree object
    TTree* tree_;

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

    // Testing counters
    int pfjet;
    int pfjet_notracks;
    int pfjet_nopv;
    int pfjet_alphazero;
    int pfjet_alphaneg;
    int pfjet_alphazero_total;
    int calojet;
    int calojet_notracks;
    int calojet_nopv;
    int calojet_alphazero;
    int calojet_alphaneg;
    int calojet_alphazero_total;
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
  jet_collT_ (consumes<edm::View<reco::CaloJet> >(iConfig.getUntrackedParameter<edm::InputTag>("jets"))),
  assocVTXToken_ (consumes<reco::JetTracksAssociationCollection>(iConfig.getUntrackedParameter<edm::InputTag>("associatorVTX"))),
  assocCALOToken_ (consumes<reco::JetTracksAssociationCollection>(iConfig.getUntrackedParameter<edm::InputTag>("associatorCALO"))),
  vtxconfig_(iConfig.getParameter<edm::ParameterSet>("vertexreco")),
  vtxmaker_(vtxconfig_),
  event_  (),
  jet_    (),
  track_  (),
  vertex_ ()
{
  // Config-independent initialization
  {
    // Initialize tree
    std::string modulename = iConfig.getParameter<std::string>("@module_label");
    tree_           = fs->make<TTree>("emJetTree","emJetTree");
    otree_.Branch(tree_);
  }

  // Config-dependent initialization
  {
    // Save Adaptive Vertex Reco config parameters to tree_->GetUserInfo()
    {
      double primcut = vtxconfig_.getParameter<double>("primcut");
      tree_->GetUserInfo()->AddLast( new TParameter<double> ("primcut", primcut) );
      double seccut = vtxconfig_.getParameter<double>("seccut");
      tree_->GetUserInfo()->AddLast( new TParameter<double> ("seccut", seccut) );
      bool smoothing = vtxconfig_.getParameter<bool>("smoothing");
      tree_->GetUserInfo()->AddLast( new TParameter<bool> ("smoothing", smoothing) );
      double minweight = vtxconfig_.getParameter<double>("minweight");
      tree_->GetUserInfo()->AddLast( new TParameter<double> ("minweight", minweight) );
    }

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

    // Register hard-coded inputs
    consumes<reco::BeamSpot> (edm::InputTag("offlineBeamSpot"));
    consumes<reco::TrackCollection> (edm::InputTag("generalTracks"));
    consumes<reco::TrackCollection> (edm::InputTag("displacedStandAloneMuons"));
    consumes<reco::PFMETCollection> (edm::InputTag("pfMet"));
    consumes<reco::VertexCollection> (edm::InputTag("offlinePrimaryVerticesWithBS"));
    consumes<reco::VertexCollection> (edm::InputTag("offlinePrimaryVertices"));
    consumes<reco::VertexCollection> (edm::InputTag("inclusiveSecondaryVertices"));

    consumes<DTRecSegment4DCollection> (edm::InputTag("dt4DSegments"));
    consumes<CSCSegmentCollection> (edm::InputTag("cscSegments"));
    consumes<RPCRecHitCollection> (edm::InputTag("rpcRecHits"));

    consumes<reco::PFCandidateCollection> (edm::InputTag("particleFlow"));

    if (!isData_) { // :MCONLY:
      consumes<std::vector<PileupSummaryInfo> > (edm::InputTag("addPileupInfo"));
      consumes<std::vector<reco::GenMET> > (edm::InputTag("genMetTrue"));
      consumes<reco::GenParticleCollection> (edm::InputTag("genParticles"));
      consumes<reco::GenJetCollection> (edm::InputTag("ak4GenJets"));
    }

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
  // Reset Testing counters
  pfjet = 0;
  pfjet_notracks = 0;
  pfjet_nopv = 0;
  pfjet_alphazero = 0;
  pfjet_alphaneg = 0;
  calojet = 0;
  calojet_notracks = 0;
  calojet_nopv = 0;
  calojet_alphazero = 0;
  calojet_alphaneg = 0;

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
    // iEvent.getByLabel("offlinePrimaryVerticesWithBS", primary_verticesH_);
    iEvent.getByLabel("offlinePrimaryVertices", primary_verticesH_);
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

  // Retrieve event level GEN quantities
  if (!isData_) {
    // edm::Handle<std::vector<reco::GenMET> > genMetH;
    // iEvent.getByLabel("genMetTrue", genMetH);
    iEvent.getByLabel("genParticles", genParticlesH_);
    // iEvent.getByLabel("ak4GenJets",   genJets_);
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
  // Track dump
  {
    int itk=0;
    // std::cout << "Dumping tracks\n";
    for ( auto trk = genTrackH->begin(); trk != genTrackH->end(); trk++ ) {
      if (itk==10) break;
      reco::TrackRef trkref(genTrackH, itk);
      bool hasNonZeroWeight = false;
      for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
        double trackWeight = ipv->trackWeight(trkref);
        if (trackWeight>0) hasNonZeroWeight = true;
        // std::cout << "Track weight: " << trackWeight << std::endl;
      } // End of vertex loop
      // OUTPUT(hasNonZeroWeight);
      itk++;
    }

    edm::Handle<reco::PFCandidateCollection> pfcandidatesH;
    iEvent.getByLabel("particleFlow", pfcandidatesH);
    for ( auto cand = pfcandidatesH->begin(); cand != pfcandidatesH->end(); cand++) {
      bool matched = false;
      if (cand->charge()==0) continue;
      auto candtrkref = cand->trackRef();
      for ( auto trk = genTrackH->begin(); trk != genTrackH->end(); trk++ ) {
        reco::TrackRef trkref(genTrackH, itk);
        if (candtrkref==trkref) matched = true;
      }
      // OUTPUT(matched);
    }
  }
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


  // Kak: Jet prepared





  // Calculate Jet-level quantities and fill into jet_ :JETLEVEL:
  for ( reco::PFJetCollection::const_iterator jet = selectedJets_->begin(); jet != selectedJets_->end(); jet++ ) {
    // Fill Jet-level quantities
    prepareJet(*jet, jet_, 1); // source = 1 for PF jets :JETSOURCE:

    // Calculate Jet-Track-level quantities and fill into jet_ :JETTRACKLEVEL:
    for (std::vector<reco::TransientTrack>::iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
      if ( !selectJetTrack(*itk, jet_, track_) ) continue; // :CUT: Apply Track selection
      // Fill Jet-Track level quantities
      prepareJetTrack(*itk, jet_, track_, 1); // source = 1 for generalTracks :TRACKSOURCE:
      fillJetTrack(*itk, jet_, track_);
    }





    // Per-jet vertex reconstruction
    {
      // std::cout << "Starting vertex reconstruction\n";
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
        // Fill Jet-Vertex level quantities
        prepareJetVertex(vtx, jet_, vertex_, 1); // source = 1 for per-jet AVR vertices :VERTEXSOURCE:
        {
          // Fill original tracks from current vertex
          for (auto trk : vtx.originalTracks()) {
            // Fill Jet-Track level quantities (for Tracks from Vertices)
            prepareJetVertexTrack(trk, jet_, track_, vtx, 2, iSetup);
            track_.source = 2; // source = 2 for original tracks from per-jet AVR vertices :TRACKSOURCE:
            // Write current Track to Jet
            jet_.track_vector.push_back(track_);
          }
        }
        if (vtx.hasRefittedTracks()) {
          // Fill refitted tracks from current vertex
          for (auto trk : vtx.refittedTracks()) {
            // Fill Jet-Track level quantities (for Tracks from Vertices)
            prepareJetVertexTrack(trk, jet_, track_, vtx, 3, iSetup);
            // source = 3 for refitted tracks from per-jet AVR vertices :TRACKSOURCE:
            // Write current Track to Jet
            jet_.track_vector.push_back(track_);
          }
        }
        else {
          std::cout << "No refitted tracks for current vertex!\n";
        }
        fillJetVertex(vtx, jet_, vertex_);
      }
    }

    // Fill Jet-Vertex level quantities for globally reconstructed AVR vertices
    for (auto vtx : avrVertices_) {
      if ( !selectJetVertex(vtx, jet_, vertex_) ) continue; // :CUT: Apply Vertex selection
      // Fill Jet-Vertex level quantities
      prepareJetVertex(vtx, jet_, vertex_, 2); // source = 2 for global AVR vertices :VERTEXSOURCE:
      // Write current Vertex to Jet
      jet_.vertex_vector.push_back(vertex_);
      if (vtx.hasRefittedTracks()) {
        // Fill refitted tracks from current vertex
        for (auto trk : vtx.refittedTracks()) {
          // Fill Jet-Track level quantities
          prepareJetVertexTrack(trk, jet_, track_, vtx, 4, iSetup);
          // source = 4 for refitted tracks from global AVR vertices :TRACKSOURCE:
          // Write current Track to Jet
          jet_.track_vector.push_back(track_);
        }
      }
      else {
        std::cout << "No refitted tracks for current vertex!\n";
      }
    }
    fillJet(*jet, jet_);
  }






  // Testing CALO jet association
  {
    edm::Handle<edm::View<reco::CaloJet> > jet_coll;
    edm::Handle<reco::JetTracksAssociationCollection> JetTracksCALO;
    edm::Handle<reco::JetTracksAssociationCollection> JetTracksVTX;
    iEvent.getByToken(jet_collT_, jet_coll);
    iEvent.getByToken(assocCALOToken_, JetTracksCALO);
    iEvent.getByToken(assocVTXToken_, JetTracksVTX);
    for(size_t i=0; i<jet_coll->size()-1; i++){
      edm::RefToBase<reco::CaloJet> jet1_ref = jet_coll->refAt(i);
      if (!(jet1_ref->pt() > 50)) continue; // :CUT: Skip CALO jets with pt < 50
      // Retrieve Jet Tracks Association
      reco::TrackRefVector dijettrks_CALO  = reco::JetTracksAssociation::getValue(*JetTracksCALO, (edm::RefToBase<reco::Jet>)jet1_ref);
      reco::TrackRefVector dijettrks_VTX  = reco::JetTracksAssociation::getValue(*JetTracksVTX, (edm::RefToBase<reco::Jet>)jet1_ref);
      reco::TrackRefVector dijettrks = MergeTracks(dijettrks_CALO, dijettrks_VTX);
      for(size_t j = 0; j<dijettrks.size(); j++){
        const reco::TrackRef trk = dijettrks[j];
        if(!trk->quality(reco::TrackBase::highPurity)) continue;
        if(trk->pt() < 5) continue;
      }
      calojet++;
      double alphaMax = compute_alphaMax(dijettrks);
      if (alphaMax==0) {
        jetdump(dijettrks);
      }

      if (primary_verticesH_->size()==0) calojet_nopv++;
      if (dijettrks.size()==0) calojet_notracks++;
      if (alphaMax==0) calojet_alphazero++;
      if (alphaMax<0) calojet_alphaneg++;
    }
  }


  // Kak: Event summary printout
  if (pfjet_alphazero!=0 || pfjet_alphaneg!=0 || calojet_alphazero!=0 || calojet_alphaneg!=0) {
    std::cout << "Event summary:";
    OUTPUT(event_.run);
    OUTPUT(event_.lumi);
    OUTPUT(event_.event);
    OUTPUT(pfjet);
    OUTPUT(pfjet_notracks);
    OUTPUT(pfjet_nopv);
    OUTPUT(pfjet_alphazero);
    OUTPUT(pfjet_alphaneg);
    OUTPUT(calojet);
    OUTPUT(calojet_notracks);
    OUTPUT(calojet_nopv);
    OUTPUT(calojet_alphazero);
    OUTPUT(calojet_alphaneg);
    pfjet_alphazero_total += pfjet_alphazero;
    calojet_alphazero_total += calojet_alphazero;
    std::cout << "\n";
  }

  // Write current Event to OutputTree
  WriteEventToOutput(event_, &otree_);
  // Write OutputTree to TTree
  tree_->Fill();

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
  pfjet_alphazero_total = 0;
  calojet_alphazero_total = 0;
}

// ------------ method called once each job just after ending the event loop  ------------
void
EmJetAnalyzer::endJob() {
  OUTPUT(pfjet_alphazero_total);
  OUTPUT(calojet_alphazero_total);
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
EmJetAnalyzer::prepareJet(const reco::PFJet& ijet, Jet& ojet, int source)
{

  pfjet++; // DEBUG
  ojet.Init();
  ojet.index = jet_index_;
  ojet.source = source;

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
    if (ojet.alphaMax<=0)
      {
        std::cout << "Dumping jet\n";
        // OUTPUT(event_.run);
        // OUTPUT(event_.lumi);
        // OUTPUT(event_.event);
        // OUTPUT(event_.nTrueInt);
        OUTPUT(primary_verticesH_->size());
        OUTPUT(trackRefs.size());
        // OUTPUT(ojet.pt);
        // OUTPUT(ojet.alphaMax);
        jetdump(trackRefs);
        std::cout << "\n";
      }
    if (primary_verticesH_->size()==0) pfjet_nopv++;
    if (trackRefs.size()==0) pfjet_notracks++;
    if (ojet.alphaMax==0) pfjet_alphazero++;
    if (ojet.alphaMax<0) pfjet_alphaneg++;
  }

  // Fill nDarkPions and nDarkGluons
  {
    ojet.nDarkPions = compute_nDarkPions(ijet);
    ojet.nDarkGluons = compute_nDarkGluons(ijet);
  }
}

void
EmJetAnalyzer::fillJet(const reco::PFJet& ijet, Jet& ojet)
{
  // Write current Jet to Event
  event_.jet_vector.push_back(ojet);

  jet_index_++;
}

void
EmJetAnalyzer::prepareJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack, int source)
{
  otrack.Init();
  otrack.index = track_index_;
  otrack.source = source;
  otrack.jet_index = jet_index_;
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
    GlobalPoint closestPoint;
    if (pca.isValid()) {
      closestPoint = pca.globalPosition();
    }
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

  otrack.quality             = itk->track().qualityMask();
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
EmJetAnalyzer::fillJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack)
{
  // Write current Track to Jet
  jet_.track_vector.push_back(track_);

  track_index_++;
}


void
EmJetAnalyzer::prepareJetVertexTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack, const TransientVertex& ivertex, int source, const edm::EventSetup& iSetup)
{
  static CheckHitPattern checkHitPattern;

  prepareJetTrack(itrack, ojet, otrack, source);
  otrack.vertex_index = vertex_index_;
  otrack.vertex_weight = ivertex.trackWeight(itrack);
  bool fixHitPattern = true;
  CheckHitPattern::Result hitInfo = checkHitPattern.analyze(iSetup, itrack.track(), ivertex.vertexState(), fixHitPattern);
  otrack.nHitsInFrontOfVert = hitInfo.hitsInFrontOfVert;
  otrack.missHitsAfterVert  = hitInfo.missHitsAfterVert;
}

void
EmJetAnalyzer::prepareJetVertex(const TransientVertex& ivertex, const Jet& ojet, Vertex& overtex, int source)
{
  overtex.Init();
  overtex.index = vertex_index_;
  overtex.source = source;
  overtex.jet_index = jet_index_;

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

void
EmJetAnalyzer::fillJetVertex(const TransientVertex& ivertex, const Jet& ojet, Vertex& overtex)
{
  // Write current Vertex to Jet
  jet_.vertex_vector.push_back(vertex_);

  vertex_index_++;
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
    for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
      double trackWeight = ipv->trackWeight(*ijt);
      if (trackWeight > 0) vertex_pt_sum += (*ijt)->pt();
    } // End of track loop
    if (vertex_pt_sum > max_vertex_pt_sum) {
      max_vertex_pt_sum = vertex_pt_sum;
      ipv_chosen = ipv;
    }
  } // End of vertex loop
  // Calculate alpha
  alphaMax = max_vertex_pt_sum / jet_pt_sum;
  return alphaMax;
}

// Calculate jet alphaMax
double
EmJetAnalyzer::compute_alphaMax(reco::TrackRefVector& trackRefs) const
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
    for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
      double trackWeight = ipv->trackWeight(*ijt);
      if (trackWeight > 0) vertex_pt_sum += (*ijt)->pt();
    } // End of track loop
    if (vertex_pt_sum > max_vertex_pt_sum) {
      max_vertex_pt_sum = vertex_pt_sum;
      ipv_chosen = ipv;
    }
  } // End of vertex loop
  // Calculate alpha
  alphaMax = max_vertex_pt_sum / jet_pt_sum;
  return alphaMax;
}

int
EmJetAnalyzer::compute_nDarkPions(const reco::PFJet& ijet) const
{
  TLorentzVector jetVector;
  jetVector.SetPtEtaPhiM(ijet.pt(),ijet.eta(),ijet.phi(),0.);
  // Count number of dark pions
  int nDarkPions = 0;
  double minDist = 9999.;
  {
    if (!isData_) {
      for (auto gp = genParticlesH_->begin(); gp != genParticlesH_->end(); ++gp) {
        if (fabs(gp->pdgId()) != 4900111) continue;
        TLorentzVector gpVector;
        gpVector.SetPtEtaPhiM(gp->pt(),gp->eta(),gp->phi(),gp->mass());
        double dist = jetVector.DeltaR(gpVector);
        if (dist < 0.4) {
          nDarkPions++;
        }
        if (dist < minDist) minDist = dist;
      }
    }
  }
  return nDarkPions;
}

int
EmJetAnalyzer::compute_nDarkGluons(const reco::PFJet& ijet) const
{
  TLorentzVector jetVector;
  jetVector.SetPtEtaPhiM(ijet.pt(),ijet.eta(),ijet.phi(),0.);
  // Count number of dark pions
  int nDarkPions = 0;
  double minDist = 9999.;
  {
    if (!isData_) {
      for (auto gp = genParticlesH_->begin(); gp != genParticlesH_->end(); ++gp) {
        if (fabs(gp->pdgId()) != 4900021) continue;
        TLorentzVector gpVector;
        gpVector.SetPtEtaPhiM(gp->pt(),gp->eta(),gp->phi(),gp->mass());
        double dist = jetVector.DeltaR(gpVector);
        if (dist < 0.4) {
          nDarkPions++;
        }
        if (dist < minDist) minDist = dist;
      }
    }
  }
  return nDarkPions;
}

void
EmJetAnalyzer::jetdump(reco::TrackRefVector& trackRefs) const
{
  // auto ipv_chosen = primary_verticesH_->end(); // iterator to chosen primary vertex
  // double max_vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
  // // Loop over all PVs and choose the one with highest scalar pt contribution to jet
  // std::cout << "Track qualities:\n";
  // for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
  //   // OUTPUT((*ijt)->qualityName(reco::TrackBase::highPurity));
  //   // OUTPUT((*ijt)->qualityName(reco::TrackBase::loose));
  //   // OUTPUT((*ijt)->qualityName(reco::TrackBase::tight));
  //   // OUTPUT((*ijt)->qualityName(reco::TrackBase::goodIterative));
  //   OUTPUT((*ijt)->quality(reco::TrackBase::highPurity));
  //   OUTPUT((*ijt)->quality(reco::TrackBase::loose));
  //   OUTPUT((*ijt)->quality(reco::TrackBase::tight));
  //   OUTPUT((*ijt)->quality(reco::TrackBase::goodIterative));
  // }
  // for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
  //   std::cout << "New vertex\n";
  //   for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
  //     double trackWeight = ipv->trackWeight(*ijt);
  //     std::cout << "Track weight: " << trackWeight << std::endl;
  //   } // End of track loop
  // } // End of vertex loop
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

// Merge two TrackRefVector objects
reco::TrackRefVector
EmJetAnalyzer::MergeTracks(reco::TrackRefVector trks1,  reco::TrackRefVector trks2){
  reco::TrackRefVector mergedtrks;
  mergedtrks = trks1;
  for(size_t i = 0; i<trks2.size(); i++){
    bool new_trk = true;
    for(size_t j = 0; j< trks1.size(); j++){
      if(trks2[i].index()==trks1[j].index()){
        new_trk = false;
      }
    }
    if(new_trk) mergedtrks.push_back(trks2[i]);
  }
  return mergedtrks;

}


//define this as a plug-in
DEFINE_FWK_MODULE(EmJetAnalyzer);
