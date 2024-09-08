#include "generator.hh"
#include "G4RandomTools.hh"
MyPrimaryGenerator::MyPrimaryGenerator()
{

  fParticleGun = new G4ParticleGun(1);
  G4ParticleTable * particleTable = G4ParticleTable::GetParticleTable();

  //G4ParticleDefinition * particle = particleTable->FindParticle("geantino");
  G4ParticleDefinition * particle = particleTable->FindParticle("mu-");

  G4ThreeVector pos(0.,0.,-20.*mm);
  G4ThreeVector mom(0.,0.,1.); 

  fParticleGun->SetParticlePosition(pos);
  fParticleGun->SetParticleMomentumDirection(mom);
  fParticleGun->SetParticleMomentum(0.538*GeV);
  //fParticleGun->SetParticleMomentum(0.200*GeV);
  fParticleGun->SetParticleDefinition(particle);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event * anEvent)
{
  G4ParticleDefinition *particle = fParticleGun->GetParticleDefinition();

  if (particle == G4Geantino::Geantino())
  {
     G4int Z =27;
     G4int A =60;
     G4double charge = 0.*eplus;
     G4double energy = 0.*keV;

     G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z,A,energy);
     fParticleGun->SetParticleDefinition(ion);
     fParticleGun->SetParticleCharge(charge); 
  } 


      // Generate a random position within the 1m x 1m area
    G4double x = (G4UniformRand() - 0.5) * 1.0 * m; // -0.5m to 0.5m
    G4double y = (G4UniformRand() - 0.5) * 1.0 * m; // -0.5m to 0.5m
    G4double z = -20.0 * mm;

    G4ThreeVector pos(x, y, z);
    fParticleGun->SetParticlePosition(pos);

  fParticleGun->GeneratePrimaryVertex(anEvent);  

}