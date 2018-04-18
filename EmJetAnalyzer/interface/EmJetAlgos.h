// :VERTEXTESTING:
template <class T>
std::tuple< std::vector<double>, std::vector<double>, std::vector<double>, std::vector<double>  >
computeMinVertexDistance(const reco::VertexCollection* vertexVector_gen, const T* vertexVector_reco)
{
  // Gen vertex to Reco vertex distance
  vector<double> GenToReco;
  for (auto vtx_gen: *vertexVector_gen) {
    double minDistance = 999;
    for (auto vtx_reco: *vertexVector_reco) {
      double x = vtx_reco.position().x() - vtx_gen.position().x();
      double y = vtx_reco.position().y() - vtx_gen.position().y();
      double z = vtx_reco.position().z() - vtx_gen.position().z();
      TVector3 delta(x, y, z);
      double distance = delta.Mag();
      if (distance < minDistance) minDistance = distance;
    }
    GenToReco.push_back(minDistance);
  }
  // Reco vertex to Gen vertex distance
  vector<double> RecoToGen;
  for (auto vtx_reco: *vertexVector_reco) {
    double minDistance = 999;
    for (auto vtx_gen: *vertexVector_gen) {
      double x = vtx_reco.position().x() - vtx_gen.position().x();
      double y = vtx_reco.position().y() - vtx_gen.position().y();
      double z = vtx_reco.position().z() - vtx_gen.position().z();
      TVector3 delta(x, y, z);
      double distance = delta.Mag();
      if (distance < minDistance) minDistance = distance;
    }
    RecoToGen.push_back(minDistance);
  }
  // Gen vertex to Reco vertex distance
  vector<double> GenToReco2D;
  for (auto vtx_gen: *vertexVector_gen) {
    double minDistance = 999;
    for (auto vtx_reco: *vertexVector_reco) {
      double x = vtx_reco.position().x() - vtx_gen.position().x();
      double y = vtx_reco.position().y() - vtx_gen.position().y();
      double z = vtx_reco.position().z() - vtx_gen.position().z();
      TVector3 delta(x, y, z);
      double distance = delta.Perp();
      if (distance < minDistance) minDistance = distance;
    }
    GenToReco2D.push_back(minDistance);
  }
  // Reco vertex to Gen vertex distance
  vector<double> RecoToGen2D;
  for (auto vtx_reco: *vertexVector_reco) {
    double minDistance = 999;
    for (auto vtx_gen: *vertexVector_gen) {
      double x = vtx_reco.position().x() - vtx_gen.position().x();
      double y = vtx_reco.position().y() - vtx_gen.position().y();
      double z = vtx_reco.position().z() - vtx_gen.position().z();
      TVector3 delta(x, y, z);
      double distance = delta.Perp();
      if (distance < minDistance) minDistance = distance;
    }
    RecoToGen2D.push_back(minDistance);
  }
  // Return std::tuple
  return std::make_tuple(GenToReco, GenToReco2D, RecoToGen, RecoToGen2D);
}

template <class T>
std::tuple< std::vector<double>, std::vector<double>, std::vector<double>, std::vector<double>  >
computeVertexDistance(const reco::VertexCollection* vertexVector_gen, const T* vertexVector_reco)
{
  // Gen vertex to Reco vertex distance
  vector<double> GenToReco;
  for (auto vtx_gen: *vertexVector_gen) {
    for (auto vtx_reco: *vertexVector_reco) {
      double x = vtx_reco.position().x() - vtx_gen.position().x();
      double y = vtx_reco.position().y() - vtx_gen.position().y();
      double z = vtx_reco.position().z() - vtx_gen.position().z();
      TVector3 delta(x, y, z);
      double distance = delta.Mag();
      GenToReco.push_back(distance);
    }
  }
  // Reco vertex to Gen vertex distance
  vector<double> RecoToGen;
  for (auto vtx_reco: *vertexVector_reco) {
    for (auto vtx_gen: *vertexVector_gen) {
      double x = vtx_reco.position().x() - vtx_gen.position().x();
      double y = vtx_reco.position().y() - vtx_gen.position().y();
      double z = vtx_reco.position().z() - vtx_gen.position().z();
      TVector3 delta(x, y, z);
      double distance = delta.Mag();
      RecoToGen.push_back(distance);
    }
  }
  // Gen vertex to Reco vertex distance
  vector<double> GenToReco2D;
  for (auto vtx_gen: *vertexVector_gen) {
    for (auto vtx_reco: *vertexVector_reco) {
      double x = vtx_reco.position().x() - vtx_gen.position().x();
      double y = vtx_reco.position().y() - vtx_gen.position().y();
      double z = vtx_reco.position().z() - vtx_gen.position().z();
      TVector3 delta(x, y, z);
      double distance = delta.Perp();
      GenToReco2D.push_back(distance);
    }
  }
  // Reco vertex to Gen vertex distance
  vector<double> RecoToGen2D;
  for (auto vtx_reco: *vertexVector_reco) {
    for (auto vtx_gen: *vertexVector_gen) {
      double x = vtx_reco.position().x() - vtx_gen.position().x();
      double y = vtx_reco.position().y() - vtx_gen.position().y();
      double z = vtx_reco.position().z() - vtx_gen.position().z();
      TVector3 delta(x, y, z);
      double distance = delta.Perp();
      RecoToGen2D.push_back(distance);
    }
  }
  // Return std::tuple
  return std::make_tuple(GenToReco, GenToReco2D, RecoToGen, RecoToGen2D);
}

template <class T>
double computeGenTrackDistance (const reco::GenParticle* gp, const T* track)
{
  double distance = 999999;
  if ( gp->charge() != track->charge() ) {
    distance = 999999;
    return distance;
  }
  else if ( gp->status() != 1 ) {
    distance = 999999;
    return distance;
  }
  // else if ( gp->numberOfDaughters() != 0 ) {
  //   distance = 999999;
  //   return distance;
  // }
  else {
    float c_pt = 1.0;
    double d_pt = ( track->pt() - gp->pt() ) / gp->pt();
    double d_eta =  track->eta() - gp->eta() ;
    double d_phi =  track->phi() - gp->phi() ;
    TLorentzVector d;
    d.SetPtEtaPhiE(c_pt*d_pt, d_eta, d_phi, 0);
    // d.SetPtEtaPhiE(c_pt*d_pt, 0, 0, 0);
    d.SetPtEtaPhiE(0, d_eta, d_phi, 0);
    distance = d.Vect().Mag();
    return distance;
  }
}

template <class T>
const reco::GenParticle* findMinDistanceGenParticle (const vector<reco::GenParticle>* gps, const T* track)
{
  double minDistance = 999999;
  const reco::GenParticle* minDistance_gp = NULL;
  for (auto gp : *gps) {
    // if ( gp.charge()==0 ) continue;
    double distance = computeGenTrackDistance(&gp, track);
    if (distance < minDistance) {
      minDistance = distance;
      minDistance_gp = &gp;
    }
  }
  return minDistance_gp;
}

template <class T>
const reco::GenParticle* findMinDistanceTrack (const reco::GenParticle* gp, const vector<T>* tracks)
{
  double minDistance = 999999;
  const T* minDistance_track = NULL;
  for (auto track : *tracks) {
    // if ( gp.charge()==0 ) continue;
    double distance = computeGenTrackDistance(gp, &track);
    if (distance < minDistance) {
      minDistance = distance;
      minDistance_track = &track;
    }
  }
  return minDistance_track;
}

template <class T>
const T* findMinDistanceTransientTrack (const reco::GenParticle* gp, const vector<T>* ttracks)
{
  double minDistance = 999999;
  const T* minDistance_ttrack = NULL;
  for (auto ttrack : *ttracks) {
    auto track = ttrack.track();
    // if ( gp.charge()==0 ) continue;
    double distance = computeGenTrackDistance(gp, &track);
    if (distance < minDistance) {
      minDistance = distance;
      minDistance_ttrack = &ttrack;
    }
  }
  return minDistance_ttrack;
}

